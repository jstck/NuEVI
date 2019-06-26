#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdint.h>
#include <stdio.h>

struct EEPROMClass
{
    EEPROMClass();
    //Basic user access methods.
   //  EERef operator[]( const int idx )    { return idx; }
    uint8_t read( int idx ); //            { return EERef( idx ); }
    void write( int idx, uint8_t val ); //   { (EERef( idx )) = val; }
    void update( int idx, uint8_t val ); //  { EERef( idx ).update( val ); }

    //STL and C++11 iteration capability.
    // EEPtr begin()                        { return 0x00; }
    // EEPtr end()                          { return length(); } //Standards requires this to be the item after the last valid entry. The returned pointer is invalid.
    uint16_t length(); //                    { return E2END + 1; }


 //   template< typename T > T &get( int idx, T &t );
 //   template< typename T > const T &put( int idx, const T &t );

//Keep these in header due to template stuff
    template< typename T > T &get( int idx, T &t ){
    fseek(storage, idx, SEEK_SET);
    fread((void*)t, sizeof(T), 1, storage);
    return t;
}

template< typename T > const T &put( int idx, const T &t ){
    fseek(storage, idx, SEEK_SET);
    fwrite((void*)t, sizeof(T), 1, storage);
    return t;
}

    //Make EEPROM persistent by storing to a file
    int16_t setStorage(const char* filename, bool write);
    void closeStorage();

private:
    uint8_t someFakeEEPROM_memory[2048]; //Teensy 3.2 size
    FILE *storage;
    bool autoUpdate;

};

extern EEPROMClass EEPROM  __unused;


#endif
