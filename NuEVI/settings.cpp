#include <EEPROM.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>


#include "settings.h"
#include "midi.h"
#include "menu.h"
#include "config.h"


extern Adafruit_SSD1306 display;

void writeSetting(uint8_t address, uint16_t value){
  union {
    byte v[2];
    uint16_t val;
  } data;
  data.val = value;
  EEPROM.write(address, data.v[0]);
  EEPROM.write(address+1, data.v[1]);
}

uint16_t readSetting(uint8_t address){
  union {
    byte v[2];
    uint16_t val;
  } data;
  data.v[0] = EEPROM.read(address);
  data.v[1] = EEPROM.read(address+1);
  return data.val;
}


void readSettings(nueviconfig &c){
  EEPROM.get(0, c);
}

void writeSettings(nueviconfig &c){
  EEPROM.put(0, c);
}


uint32_t crc32(uint8_t *message, size_t length) {
   size_t pos=0;
   uint32_t crc=0xFFFFFFFF;

   while (pos<length) {
      crc ^= message[pos++]; //Get next byte and increment position
      for (uint8_t j=0; j<8; ++j) { //Mask off 8 next bits
         crc = (crc >> 1) ^ (0xEDB88320 &  -(crc & 1));
      }
   }
   return ~crc;
}

//void dumpSettings(__unused const nueviconfig &c) {
void dumpSettings() {
  uint8_t *sysex_data;

  const char *header = "NuEVIc01"; //NuEVI config dump 01

  //Build an array of all the config prefixed with manufacturer id + "nuevi config dump"
  //Array is size of 3+header+config
//  size_t config_size = sizeof(nueviconfig);
  size_t config_size = 98;
  size_t sysex_size = 3 + strlen(header) + 2 + config_size + 4;

  //Positions (offsets) of parts of sysex buffer
  int header_pos = 3;
  int size_pos = header_pos + strlen(header);
  int payload_pos = size_pos + 2;
  int checksum_pos = payload_pos + config_size;


  sysex_data = (uint8_t*)malloc(sysex_size);

  //SysEX manufacturer ID
  memcpy(sysex_data, sysex_id, 3);

  //Header / "command"
  memcpy(sysex_data+header_pos, header, strlen(header));

  //Payload length
  *(uint16_t*)(sysex_data+size_pos) = midi16to14(config_size);
  
  //Config data
  uint16_t* config_buffer_start = (uint16_t*)(sysex_data+payload_pos);
  
/*
  uint16_t* config_data = (uint16_t*)(&c);
  for(uint16_t idx=0; idx<config_size/2; idx++) {
    config_buffer_start[idx] = midi16to14(config_data[idx]);
  }
*/
  for(uint16_t idx=0; idx<config_size/2; idx++) {
    uint16_t eepromval = readSetting(idx*2);
    config_buffer_start[idx] = midi16to14(eepromval);
  }
  //memcpy(sysex_data+5+strlen(header), &c, config_size);

  uint32_t checksum = crc32(sysex_data, checksum_pos);

  printf("CRC len: %d\n", checksum_pos);
  printf("CRC32: %X | %u\n", checksum, checksum);

  *(uint32_t*)(sysex_data+checksum_pos) = midi32to28(checksum);

  usbMIDI.sendSysEx(sysex_size, sysex_data);

  free(sysex_data);
}

//Send a simple 3-byte message code as sysex
void sendSysexMessage(const char* messageCode) {
  char sysexMessage[] = "vvvNuEVIccc"; //Placeholders for vendor and code

  memcpy(sysexMessage, sysex_id, 3);
  memcpy(sysexMessage+8, messageCode, 3);

  usbMIDI.sendSysEx(11, (const uint8_t *)sysexMessage);
}

//Send EEPROM and firmware versions
void sendSysexVersion() {
  char sysexMessage[] = "vvvNuEVIc04eevvvvvvvv"; //Placeholders for vendor and code

  memcpy(sysexMessage, sysex_id, 3);
  memcpy(sysexMessage+13, FIRMWARE_VERSION, strlen(FIRMWARE_VERSION));

  *(uint16_t*)(sysexMessage+11) = midi16to14(VERSION);

  uint8_t message_length = 13+strlen(FIRMWARE_VERSION);

  usbMIDI.sendSysEx(message_length, (const uint8_t *)sysexMessage);
}


void configInitScreen() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(0);

  display.println("Config management");
  display.println("Power off NuEVI to exit");
  display.display();
}

void configShowMessage(const char* message) {
  display.fillRect(0,32,128,64,BLACK);
  display.fillRect(63,11,64,52,BLACK);
  display.print(message);
  display.display();
}

void handleSysex(uint8_t *data, uint8_t length) {

  //Too short to even contain a 3-byte vendor id is not for us.
  if(length<3) return;

  //Verify vendor
  if(strncmp((char*)data, sysex_id, 3)) return; //Silently ignore different vendor id

  //Verify header. Min length is 3+5+3 bytes (vendor+header+message code)
  if(length<11 || strncmp((char*)(data+3), "NuEVI", 3)) {
    configShowMessage("Invalid message received.");
    sendSysexMessage("e00");
    return;
  }

  //Get message code
  char messageCode[3];
  strncpy(messageCode, (char*)(data+8), 3);

  if(!strncmp(messageCode, "c00", 3)) { //Config dump request
    configShowMessage("Sending config...");
    dumpSettings();
    configShowMessage("Config sent.");
  } else if(!strncmp(messageCode, "c03", 3)) { //Version info request
    configShowMessage("Sending version.");
    sendSysexVersion();
  } else {
    configShowMessage("Unknown message received.");
    sendSysexMessage("e01"); //Unimplemented message code
  }
}

//"Main loop" when in config management mode. Never exits and just lets handleSysex() do its thing, so NuEVI has to be reset when done
void configLoop() {
  usbMIDI.setHandleSystemExclusive(handleSysex); 
  configInitScreen();
  configShowMessage("Ready.");

  delay(1000);

  return;

  while(true) {
    usbMIDI.read();
  }
}