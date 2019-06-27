#include <EEPROM.h>
#include <Arduino.h>

#include "settings.h"
#include "midi.h"

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

void dumpSettings(const nueviconfig &c) {
  uint8_t *sysex_data;

  const char *header = "NuEVIc00"; //NuEVI config dump 00

  //Build an array of all the config prefixed with manufacturer id + "nuevi config dump"
  //Array is size of 3+header+config
  size_t config_size = sizeof(nueviconfig);
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
  uint16_t* config_data = (uint16_t*)(&c);
  for(uint16_t idx=0; idx<config_size/2; idx++) {
    config_buffer_start[idx] = midi16to14(config_data[idx]);
  }

  //memcpy(sysex_data+5+strlen(header), &c, config_size);

  //Todo:  Checksum
  uint32_t checksum = 0xDEADBEEF;
  *(uint32_t*)(sysex_data+checksum_pos) = checksum & 0x7F7F7F7F; //Don't bother bitshifting, just mask bits off.

  usbMIDI.sendSysEx(sysex_size, sysex_data);

  free(sysex_data);
}