#include "settings.h"


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

void factoryReset() {

  //Current format of settings on EEPROM
  unsigned short currentVersion = readSetting(VERSION_ADDR);

  //Only set sensor calibration values if upgrading from a version when they
  //did not yet exist, otherwise they are left in place.
  if (currentVersion < 24) {
    writeSetting(VERSION_ADDR,VERSION);
    writeSetting(BREATH_THR_ADDR,BREATH_THR_FACTORY);
    writeSetting(BREATH_MAX_ADDR,BREATH_MAX_FACTORY);
    writeSetting(PORTAM_THR_ADDR,PORTAM_THR_FACTORY);
    writeSetting(PORTAM_MAX_ADDR,PORTAM_MAX_FACTORY);
    writeSetting(PITCHB_THR_ADDR,PITCHB_THR_FACTORY);
    writeSetting(PITCHB_MAX_ADDR,PITCHB_MAX_FACTORY);
    writeSetting(EXTRAC_THR_ADDR,EXTRAC_THR_FACTORY);
    writeSetting(EXTRAC_MAX_ADDR,EXTRAC_MAX_FACTORY);
    writeSetting(CTOUCH_THR_ADDR,CTOUCH_THR_FACTORY);
  }

  writeSetting(TRANSP_ADDR,TRANSP_FACTORY);
  writeSetting(MIDI_ADDR,MIDI_FACTORY);
  writeSetting(BREATH_CC_ADDR,BREATH_CC_FACTORY);
  writeSetting(BREATH_AT_ADDR,BREATH_AT_FACTORY);
  writeSetting(VELOCITY_ADDR,VELOCITY_FACTORY);
  writeSetting(PORTAM_ADDR,PORTAM_FACTORY);
  writeSetting(PB_ADDR,PB_FACTORY);
  writeSetting(EXTRA_ADDR,EXTRA_FACTORY);
  writeSetting(VIBRATO_ADDR,VIBRATO_FACTORY);
  writeSetting(DEGLITCH_ADDR,DEGLITCH_FACTORY);
  writeSetting(PATCH_ADDR,PATCH_FACTORY);
  writeSetting(OCTAVE_ADDR,OCTAVE_FACTORY);
  writeSetting(BREATHCURVE_ADDR,BREATHCURVE_FACTORY);
  writeSetting(VEL_SMP_DL_ADDR,VEL_SMP_DL_FACTORY);
  writeSetting(VEL_BIAS_ADDR,VEL_BIAS_FACTORY);
  writeSetting(PINKY_KEY_ADDR,PINKY_KEY_FACTORY);
  writeSetting(FP1_ADDR,0);
  writeSetting(FP2_ADDR,0);
  writeSetting(FP3_ADDR,0);
  writeSetting(FP4_ADDR,0);
  writeSetting(FP5_ADDR,0);
  writeSetting(FP6_ADDR,0);
  writeSetting(FP7_ADDR,0);
  writeSetting(DIPSW_BITS_ADDR,DIPSW_BITS_FACTORY);
  writeSetting(PARAL_ADDR,PARAL_FACTORY);
  writeSetting(ROTN1_ADDR,ROTN1_FACTORY);
  writeSetting(ROTN2_ADDR,ROTN2_FACTORY);
  writeSetting(ROTN3_ADDR,ROTN3_FACTORY);
  writeSetting(ROTN4_ADDR,ROTN4_FACTORY);
  writeSetting(PRIO_ADDR,PRIO_FACTORY);
  writeSetting(VIB_SENS_ADDR,VIB_SENS_FACTORY);
  writeSetting(VIB_RETN_ADDR,VIB_RETN_FACTORY);
  writeSetting(VIB_SQUELCH_ADDR,VIB_SQUELCH_FACTORY);
  writeSetting(VIB_DIRECTION_ADDR,VIB_DIRECTION_FACTORY);
}
