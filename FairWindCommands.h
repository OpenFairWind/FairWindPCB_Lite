/*
 * Copyright (C) 2014 University of Napoli Parthenope
 * Department of Science and Technologies
 * High Performance Scientific Computing Smart-Lab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Raffaele Montella (raffaele.montella@uniparthenope.it)
 *
 * Notes: This software is part of the FairWind
 *        (Smart, Cloud-Enabled, Navigation System for Boats and Yachts)
 * please visit http://fairwind.uniparthenope.it for more details.
 * The main Android application is availabile here:
 * https://play.google.com/store/apps/details?id=it.uniparthenope.fairwind
 * Contact the author if you plan to use any part of FairWind on a real boat.
 */

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
