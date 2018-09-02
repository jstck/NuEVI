#include "midi.h"

#ifndef MIDI_CHANNEL
#define MIDI_CHANNEL 1
#endif

void setupMidi() {
#ifdef SEND_SERIAL_MIDI
  Serial3.begin(31250);   // start serial with midi baudrate 31250
  Serial3.flush();
#endif
}

//Separate MIDI channels for USB and serial. Serial is 0-based, one less than everything else.
byte activeMIDIchannel=MIDI_CHANNEL;
byte serialMIDIchannel=MIDI_CHANNEL-1;

void setMidiChannel(byte channel) {
  activeMIDIchannel=channel;
  serialMIDIchannel=channel-1;
}

byte getMidiChannel() {
  return activeMIDIchannel;
}

void midiSendNoteOn(byte note, int velocity) {
#ifdef SEND_USB_MIDI
  usbMIDI.sendNoteOn(note, velocitySend, activeMIDIchannel);
#endif
  midiSend3B((0x90 | serialMIDIchannel), note, velocity);
}

void midiSendNoteOff(byte note, int velocity) {
#ifdef SEND_USB_MIDI
  usbMIDI.sendNoteOff(note, velocitySend, activeMIDIchannel);
#endif
  midiSend3B((0x80 | serialMIDIchannel), note, velocity);
}

void midiSendProgramChange(byte value) {
#ifdef SEND_USB_MIDI
  usbMIDI.sendProgramChange(value, activeMIDIchannel);
#endif
  midiSend2B((0xC0 | serialMIDIchannel), value);
}

void midiSendControlChange(byte ccNumber, int ccValue) {
#ifdef SEND_USB_MIDI
  usbMIDI.sendControlChange(ccNumber, ccValue, activeMIDIchannel);
#endif
  midiSend3B((0xB0 | serialMIDIchannel), ccNumber, ccValue);
}

void midiSendPitchBend(int pbValue) {

#ifdef SEND_USB_MIDI
  // Newer teensyduino (>=1.4.1 have a signed value, 1.4.0 use an midi wire format (unsigned, with no bend = 8192)
  #ifdef NEWTEENSYDUINO
  usbMIDI.sendPitchBend(pbValue-8192, activeMIDIchannel);
  #else
  usbMIDI.sendPitchBend(pbValue, activeMIDIchannel);
  #endif //NEWTEENSYDUINO
#endif //SEND_USB_MIDI

  //Split 14-bit pitch bend value into two 7-bit values
  byte pitchLSB = pbValue & 0x007F;
  byte pitchMSB = (pbValue >>7) & 0x007F;
  midiSend3B((0xE0 | serialMIDIchannel), pitchLSB, pitchMSB);
}

//  Send aftertouch
void midiSendAfterTouch(byte value) {
#ifdef SEND_USB_MIDI
  usbMIDI.sendAfterTouch(value, activeMIDIchannel);
#endif
  midiSend2B((0xD0 | serialMIDIchannel), value);
}

// all notes off
void midiPanic() {
  midiSendControlChange(123, 0);
}

// reset controllers
void midiReset() {
  midiSendControlChange(7, 100);
  midiSendControlChange(11, 127);
}

//  Send a three byte serial midi message
void midiSend3B(byte midistatus, byte data1, byte data2) {

#ifdef SEND_SERIAL_MIDI
  Serial3.write(midistatus);
  Serial3.write(data1);
  Serial3.write(data2);
#endif
}

//  Send a two byte serial midi message
void midiSend2B(byte midistatus, byte data) {
#ifdef SEND_SERIAL_MIDI
  Serial3.write(midistatus);
  Serial3.write(data);
#endif
}
