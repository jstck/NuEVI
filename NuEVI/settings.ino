#include "settings.h"
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



void readSettings(struct config* c){
  EEPROM.get(0, c*);
}


void dumpSettings(const config* c) {
  byte sysex_data[];

  //Build an array of all the config prefixed with manufacturer id + "nuevi config dump" signal
  //Array is size of config + 11 bytes, 3 bytes id and 8 bytes header
  sysex_data = malloc(11 + sizeof(c*));

  const unsigned char header = 'NuEVIc00'; //NuEVI config dump 00

  memcpy(sysex_data, sysex_id, 3);
  memcpy(sysex_data+3, header, 8);
  memcpy(sysex_data+11, c, sizeof(c*);)

  usbMidi.sendSysex(sysex_data);
}