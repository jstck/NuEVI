#include <EEPROM.h>



void writeSetting(byte address, unsigned short value){
  union {
    byte v[2];
    unsigned short val;
  } data;
  data.val = value;
  EEPROM.write(address, data.v[0]);
  EEPROM.write(address+1, data.v[1]);
}

unsigned short readSetting(byte address){
  union {
    byte v[2];
    unsigned short val;
  } data;
  data.v[0] = EEPROM.read(address);
  data.v[1] = EEPROM.read(address+1);
  return data.val;
}


static const unsigned char sysex_id[] = { 0x00, 0x3e, 0x5a };


struct config {
  uint16_t version;         //00
  uint16_t breath_thr;
  uint16_t breath_max;
  uint16_t portam_thr;
  uint16_t portam_max;
  uint16_t pitchb_thr;      //10
  uint16_t pitchb_max;
  uint16_t transpose;
  uint16_t midi_channel;
  uint16_t breath_cc;
  uint16_t breath_at;       //20
  uint16_t velocity;
  uint16_t portamento;
  uint16_t pb_depth;
  uint16_t extra_ct;
  uint16_t vibrato;         //30
  uint16_t deglitch;
  uint16_t extrac_thr;
  uint16_t extrac_max;
  uint16_t patch;
  uint16_t octave;          //40
  uint16_t ctouch_thr;
  uint16_t breathcurve;
  uint16_t vel_smp_dl;
  uint16_t vel_bias;
  uint16_t pinky_setting;   //50
  uint16_t fastpatch1;
  uint16_t fastpatch2;
  uint16_t fastpatch3;
  uint16_t fastpatch4;
  uint16_t fastpatch5;      //60
  uint16_t fastpatch6;
  uint16_t fastpatch7;
  uint16_t dipsw_bits;
  uint16_t parallel;
  uint16_t rotations1;      //
  uint16_t rotations2;
  uint16_t rotations3;
  uint16_t rotations4;
  uint16_t priority;
  uint16_t vib_sens;        //80
  uint16_t vib_retn;
  uint16_t vib_squelch;
  uint16_t vib_direction;
};

config c;


void readSettings(struct config *nc){
  EEPROM.get(0, *nc);
}


void setup() {
        usbMIDI.sendControlChange(123, 0, 1);
        Serial.println("Startup");
        delay(500);

        //factoryEEPROM();

        readSettings(&c);

        
}

void dump_config(struct config *c) {
  
  uint16_t config_size;

  uint16_t *eeprom_buffer;
  uint8_t *buffer;


  uint8_t msb, lsb;

  config_size = sizeof(*c);

  buffer = (uint8_t*)malloc(11+config_size); //Fit 3-byte vendor code + 8 byte header + config;

  memcpy(buffer, sysex_id, 3);
  memcpy(buffer+3, "NuEVIc00", 8);

  eeprom_buffer = (uint16_t*)c;

  //Copy and transform all the config pieces, one item at a time
  for(uint16_t n=0; n<config_size/2; n++) {
    uint16_t e_val = eeprom_buffer[n];
    
    //Copy to output buffer in big-endian byte order, only using 7 bits per byte (all that fits in sysex data)
    msb = (e_val & 0x3F80) >> 7; //Bits 8-14
    lsb = e_val & 0x7F; //Bits 1-7
  
    buffer[11+2*n] = msb;
    buffer[11+2*n+1] = lsb;
  }

  usbMIDI.sendSysEx(11+config_size, buffer);

  free(buffer);
  buffer=NULL;
}


int8_t p=0;
void loop() {
  //usbMIDI.sendSysEx(sizeof(struct config), (uint8_t*)&c);
  Serial.print("Version "); Serial.println(c.version);
  Serial.print("ROTN1 "); Serial.println(c.rotations1);
  Serial.print("vibdir "); Serial.println(c.vib_direction);
  dump_config(&c);

  usbMIDI.sendPitchBend(p++, 1);

  delay(2000);
}

