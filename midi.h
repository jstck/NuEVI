#ifndef _MIDI_H
#define _MIDI_H

void setupMidi();
void setMidiChannel(byte channel);
byte getMidiChannel();
void midiSendNoteOn(byte note, int velocity);
void midiSendNoteOff(byte note, int velocity);
void midiSendProgramChange(byte value);
void midiSendControlChange(byte ccNumber, int ccValue);
void midiSendPitchBend(int pbValue);
void midiSendAfterTouch(byte value);
void midiPanic();
void midiReset();

#endif //_MIDI_H
