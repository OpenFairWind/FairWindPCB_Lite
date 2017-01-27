#ifndef FAIRWINDCOMMANDS_H_
#define FAIRWINDCOMMANDS_H_

#include "Arduino.h"
#include "FairWindConstants.h"
#include "FairWindModel.h"
#include "NMEAEx.h"



class FairWindModel;

class FairWindCommands {
  
public:
  FairWindCommands(FairWindModel *model, NMEABase* nmeaIn);

  bool process(NMEABase* nmeaOut);

  void fillWho(NMEABase* nmeaOut);
  void fillStatus(NMEABase* nmeaOut);
  void fillFirmwareVersion(NMEABase* nmeaOut);
  void fillConfiguration(NMEABase* nmeaOut);
  void setConfiguration(NMEABase* nmeaOut);
  void setOut(NMEABase* nmeaOut);
  void fillOut(NMEABase* nmeaOut);
  void fillOut(NMEABase* nmeaOut, int outId);
  void fillInp(NMEABase* nmeaOut);
  void fillInp(NMEABase* nmeaOut, int inpId);

  void manageDemoMode(NMEABase* nmeaOut);
  
  private:
    FairWindModel *_model;
    NMEABase* _nmeaIn;
};

#endif
