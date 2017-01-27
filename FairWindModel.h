#ifndef FAIRWINDMODEL_H_
#define FAIRWINDMODEL_H_

#include "Arduino.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <PString.h>
#include "FairWindConstants.h"
#include "FairWindCommands.h"
#include "math.h"
#include "NMEAEx.h"
#include "Seatalk.h"
#include "util.h"



#define EEPROM_VER 13
#define EEPROM_DATA 4

// The struct for the configuration
typedef struct config_tag { 
  char id[8];
  char serialLabel[5][8];   // 40 bytes
  long serialBaud[5]; // 20 bytes
  byte seaTalk=0x00; // 1 bytes
  byte outs[20];
  byte inps[20];
  long status_interval;
} Config;

class FairWindCommands;

class FairWindModel {
public:
  FairWindModel();

  char* getSerialLabel(int i);
  void setSerialLabel(int i, char *label);
      
  long getSerialBaud(int i);
  void setSerialBaud(int i, long serialBaud);
       
        
  int getSeaTalk();
  void setSeaTalk(int seaTalk);
  bool isSeaTalk(int serialNumber);
  HardwareSerial* getSerial(int serialNumber); 

  Config *getConfig() { return &_config; }
  void saveConfig();
  void readConfig();
        
  MultiSerial *getMultiSerial() { return &_mSerial1; };
  NMEAEx *getNMEAHost() { return _nmea[0]; }
  NMEAEx *getNMEA(int i) { return _nmea[i]; }
  Seatalk *getSeatalk(int i) { return _seatalk[i]; }

  void serialEvent();
  void serialEvent1();
  void serialEvent2();
  void serialEvent3();
  void serialEvent4();

  void sayFairWind();

  void check();

  // bool processCommand(NMEABase* nmeaIn, NMEABase* nmeaOut);

  void fillWho(NMEABase* nmeaIn, NMEABase* nmeaOut);
  void fillFirmwareVersion(NMEABase* nmeaIn, NMEABase* nmeaOut);
  void fillConfiguration(NMEABase* nmeaIn, NMEABase* nmeaOut);
  void setConfiguration(NMEABase* nmeaIn, NMEABase* nmeaOut);
  void setSwitch(NMEABase* nmeaIn, NMEABase* nmeaOut);
  void fillSwitch(NMEABase* nmeaIn, NMEABase* nmeaOut);

  

  void demoOn() { _demo_mode=true; }
  void demoOff() { _demo_mode=false; }
  bool isDemo() { return _demo_mode;}
  
  private:
    Config _config;
    int _version;
    byte _last_inp_states[20];
    long _last_millis;
    
    
    
    NMEAEx *_nmea[5]={ NULL,NULL,NULL,NULL,NULL };
    Seatalk *_seatalk[3]={NULL, NULL, NULL};
    MultiSerial _mSerial1;
        
    boolean _inputSerialComplete[5] = { false, false, false, false, false };

    FairWindCommands* _commands=NULL;

    boolean _demo_mode;
    char** _demo_current;
    char* _demo[4]={
      "$IIGGA,093153,4050.031,N,01415.276,E,1,05,2,3,M,47,M,,*40",
      "$IIGLL,4050.031,N,01415.276,E,093154,A*36",
      "$IIZDA,093201,21,06,2014,,*54",
      NULL
    };
    /*
    char* _demo[18]={
      "$IIVHW,157.5,T,156.5,M,0.00,N,0.00,K*54",
      "$IIVPW,0.00,N,0.00,M*52",
      "$IIVWR,013.5,R,8.6,N,4.4,M,15.9,K*7D",
      "$IIVWT,013.5,R,8.6,N,4.4,M,15.9,K*7B",
      "$IIXTE,A,A,0.00,R,N*67",
      "$IIAPB,A,A,0.00,R,N,,,,,0,220.0,M,,*02",
      "$IIBWC,093153,,,,,221.0,T,220.0,M,4.20,N,0*25",
      "$IIDBT,8.2,f,2.50,M,1.36,F*18",
      "$IIGGA,093153,4050.031,N,01415.276,E,1,05,2,3,M,47,M,,*40",
      "$IIGLL,4050.031,N,01415.276,E,093154,A*36",
      "$IIHDG,155.5,,,1,E*39",
      "$IIHDM,155.5,M*26",
      "$IIHDT,156.5,T*25",
      "$IIRSA,-12,A,,V*79",
      "$IIMWV,018.5,R,8.4,N,A*3D",
      "$IIMWV,018.5,T,8.4,N,A*3B",
      "$IIZDA,093201,21,06,2014,,*54",
      NULL
    };
    */
  
  
};
#endif /* FAIRWINDMODEL_H_ */
