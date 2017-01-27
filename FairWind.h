


#ifndef FairWind_H_
#define FairWind_H_

typedef unsigned char byte;

#include "Arduino.h"
#include "FairWind.h"
#include <EEPROM.h>
#include <MultiSerial.h>
#include "Seatalk.h"
#include "NmeaEx.h"
#include "FairWindModel.h"






#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
}
#endif
//void check_mem();
//int freeRam();
//void process(char * s, char parser);
//byte getChecksum(char* str);
#endif 

