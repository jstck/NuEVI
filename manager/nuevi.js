const factory_eeprom_data = [
0x00, 0x3E, 0x7F,
0x4E, 0x75, 0x45, 0x56, 0x49, 0x63, 0x30, 0x30,
0x00, 0x58,
0x00, 0x1F, 0x0A, 0x78, 0x1F, 0x20, 0x14, 0x28, 0x19, 0x64, 0x0A, 0x78, 0x11, 0x7C, 0x00, 0x0C,
0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x04,
0x00, 0x14, 0x09, 0x30, 0x12, 0x60, 0x00, 0x01, 0x00, 0x03, 0x00, 0x7D, 0x00, 0x04, 0x00, 0x0F,
0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x13, 0x00, 0x0E, 0x00, 0x11, 0x00, 0x0A, 0x00, 0x00,
0x00, 0x06, 0x00, 0x02, 0x00, 0x0F, 0x00, 0x00,
0x5B, 0x0A, 0x56, 0x1D];


const nuevi_vendor = [ 0x00, 0x3e, 0x7f ];

//Addresses of all items in EEPROM data

const VERSION_ADDR = 0;
const BREATH_THR_ADDR = 2;
const BREATH_MAX_ADDR = 4;
const PORTAM_THR_ADDR = 6;
const PORTAM_MAX_ADDR = 8;
const PITCHB_THR_ADDR = 10;
const PITCHB_MAX_ADDR = 12;
const TRANSP_ADDR = 14;
const MIDI_ADDR = 16;
const BREATH_CC_ADDR = 18;
const BREATH_AT_ADDR = 20;
const VELOCITY_ADDR = 22;
const PORTAM_ADDR = 24;
const PB_ADDR = 26;
const EXTRA_ADDR = 28;
const VIBRATO_ADDR = 30;
const DEGLITCH_ADDR = 32;
const EXTRAC_THR_ADDR = 34;
const EXTRAC_MAX_ADDR = 36;
const PATCH_ADDR = 38;
const OCTAVE_ADDR = 40;
const CTOUCH_THR_ADDR = 42;
const BREATHCURVE_ADDR = 44;
const VEL_SMP_DL_ADDR = 46;
const VEL_BIAS_ADDR = 48;
const PINKY_KEY_ADDR = 50;
const FP1_ADDR = 52;
const FP2_ADDR = 54;
const FP3_ADDR = 56;
const FP4_ADDR = 58;
const FP5_ADDR = 60;
const FP6_ADDR = 62;
const FP7_ADDR = 64;
const DIPSW_BITS_ADDR = 66;
const PARAL_ADDR = 68;
const ROTN1_ADDR = 70;
const ROTN2_ADDR = 72;
const ROTN3_ADDR = 74;
const ROTN4_ADDR = 76;
const PRIO_ADDR = 78;
const VIB_SENS_ADDR = 80;
const VIB_RETN_ADDR = 82;
const VIB_SQUELCH_ADDR = 84;
const VIB_DIRECTION_ADDR = 86;

function midi14to16(mididata) {
	return mididata[0]<<7 | mididata[1];
}

function readSetting(mididata, pos) {
	return midi14to16(mididata.slice(pos, pos+2));
}

function equalArray(a1, a2) {
	if(a1.length != a2.length) return false;

	for(i=0; i<a1.length; ++i) {
		if(a1[i] != a2[i]) return false;
	}

	return true;
}



function readConfig(mididata) {
  var nueviConfig = new Object();

  nueviConfig.version      = readSetting(mididata, VERSION_ADDR);
  nueviConfig.adjustBreath = [readSetting(mididata, BREATH_THR_ADDR), readSetting(mididata, BREATH_MAX_ADDR)];
  nueviConfig.adjustPorta  = [readSetting(mididata, PORTAM_THR_ADDR), readSetting(mididata, PORTAM_MAX_ADDR)];
  nueviConfig.adjustPitchB = [readSetting(mididata, PITCHB_THR_ADDR), readSetting(mididata, PITCHB_MAX_ADDR)];
  nueviConfig.adjustExtraC = [readSetting(mididata, EXTRAC_THR_ADDR), readSetting(mididata, EXTRAC_MAX_ADDR)];
  
  nueviConfig.transpose    = readSetting(mididata, TRANSP_ADDR);
  nueviConfig.MIDIchannel  = readSetting(mididata, MIDI_ADDR);
  nueviConfig.breathCC     = readSetting(mididata, BREATH_CC_ADDR);
  nueviConfig.breathAT     = readSetting(mididata, BREATH_AT_ADDR);
  nueviConfig.velocity     = readSetting(mididata, VELOCITY_ADDR);
  nueviConfig.portamento   = readSetting(mididata, PORTAM_ADDR);
  nueviConfig.PBdepth      = readSetting(mididata, PB_ADDR);
  nueviConfig.extraCT      = readSetting(mididata, EXTRA_ADDR);
  nueviConfig.vibrato      = readSetting(mididata, VIBRATO_ADDR);
  nueviConfig.deglitch     = readSetting(mididata, DEGLITCH_ADDR);

  nueviConfig.patch        = readSetting(mididata, PATCH_ADDR);
  nueviConfig.octave       = readSetting(mididata, OCTAVE_ADDR);
  nueviConfig.ctouchThrVal = readSetting(mididata, CTOUCH_THR_ADDR);
  nueviConfig.curve        = readSetting(mididata, BREATHCURVE_ADDR);
  nueviConfig.velSmpDl     = readSetting(mididata, VEL_SMP_DL_ADDR);
  nueviConfig.velBias      = readSetting(mididata, VEL_BIAS_ADDR);
  nueviConfig.pinkySetting = readSetting(mididata, PINKY_KEY_ADDR);

  nueviConfig.fastPatch    = [readSetting(mididata, FP1_ADDR),
							  readSetting(mididata, FP2_ADDR),
							  readSetting(mididata, FP3_ADDR),
							  readSetting(mididata, FP4_ADDR),
							  readSetting(mididata, FP5_ADDR),
							  readSetting(mididata, FP6_ADDR),
							  readSetting(mididata, FP7_ADDR)];

  nueviConfig.dipSwBits    = readSetting(mididata, DIPSW_BITS_ADDR);
  nueviConfig.parallel     = readSetting(mididata, PARAL_ADDR);

  nueviConfig.rotations    = [readSetting(mididata, ROTN1_ADDR),
  							  readSetting(mididata, ROTN2_ADDR),
  							  readSetting(mididata, ROTN3_ADDR),
  							  readSetting(mididata, ROTN4_ADDR)];
  							  
  nueviConfig.priority     = readSetting(mididata, PRIO_ADDR);
  nueviConfig.vibSens      = readSetting(mididata, VIB_SENS_ADDR);
  nueviConfig.vibRetn      = readSetting(mididata, VIB_RETN_ADDR);
  nueviConfig.vibSquelch   = readSetting(mididata, VIB_SQUELCH_ADDR);
  nueviConfig.vibDirection = readSetting(mididata, VIB_DIRECTION_ADDR);

  return nueviConfig;
}



function parseSysex(data) {
	var vendor = data.slice(0,3);

	if(!equalArray(vendor, nuevi_vendor)) {
		console.log("Unknown vendor: " + vendor);
		return;
	}


	var header = data.slice(3,11).map(x => String.fromCharCode(x)).join("");

	if(header != "NuEVIc00") {
		console.log("Unknown header: '" + header + "'");
		return;
	}

	var payload_length = midi14to16(data.slice(11,13));

	if(payload_length != 88) {
		console.log("Payload expected to be 88 bytes, was " + payload_length + " bytes.");
		return;
	}

	if(data.length < 13+payload_length) {
		console.log("Data too short to fit payload, only " + data.length + " bytes found.");
		return;	
	}

	var payload = data.slice(13, 13+payload_length);
	var checksum = data.slice(-4);
	var checksum_data = data.slice(0, -4);



	
	crc = CRC32.buf(checksum_data);

	//Mask off msb of each byte to match 7-bit midi data format
	if(!equalArray(checksum, [ crc>>24 & 0x7f, crc>>16 & 0x7f, crc>>8 & 0x7f, crc & 0x7f])) {
		console.log("Checksum mismatch");
		return;
	}

	nc = readConfig(payload);
	//console.log(nc);


	if(nc.version != 31) {
		console.log("Unknown NuEVI config version: '" + nc.version + "'.");
	}
	
	return nc;

}



