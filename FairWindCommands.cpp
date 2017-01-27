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

#include "FairWindCommands.h"
#include "util.h"

/*
 * The FairWind command handler constructor
 */
FairWindCommands::FairWindCommands(FairWindModel *model, NMEABase* nmeaIn) {
  // The model
  _model=model;

  // The nmea input object
  _nmeaIn=nmeaIn;
}

void FairWindCommands::manageDemoMode(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  char *demoStatus[2]={"off","on"};
  bool done=false;

  // Check if 
  if (_nmeaIn->check(3,"on")) {
    _model->demoOn();
    done=true;
  }
  else if (_nmeaIn->check(3,"off")) {
    _model->demoOff();   
    done=true;
  }
  else if (_nmeaIn->check(3,"status")) {
    done=true;
  } 

  if (done) {
    // Prepare the output
    sprintf(sentenceOut,"$FWACK,%s,demo,%s*",_model->getConfig()->id,demoStatus[_model->isDemo()]);
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
}

void FairWindCommands::fillStatus(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;
  byte i[3],o[3];
  int pos=0,idx=0;
  for(int id=1;id<=20;id++) {
    if (id>=1 && id<=8) idx=0;
    else if (id>=9 && id<=16) idx=1;
    else idx=2;
    if (digitalRead(_model->getConfig()->inps[id-1])) SET_BIT(i[idx],pos); else CLEAR_BIT(i[idx],pos);
    if (digitalReadOutputPin(_model->getConfig()->outs[id-1])) SET_BIT(o[idx],pos); else CLEAR_BIT(o[idx],pos);
    pos++;
    if (pos==8) pos=0;
  }
  sprintf(sentenceOut,"$FWSUP,%s,inp,%02X,%02X,%02X,out,%02X,%02X,%02X*",
    _model->getConfig()->id,
    i[0],i[1],i[2],o[0],o[1],o[2]
    );
  done=true;

  if (done) {
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
}

/*
 * Process set configuration 
 */
void FairWindCommands::setConfiguration(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];

  // Save and reboot flag
  bool save=false;
  bool reboot=false;

  // The config structure
  Config config;
  
  // Copy the config status
  memcpy(&config,_model->getConfig(),sizeof(Config));

   // Check if the target is "id"
  if (_nmeaIn->check(3,"id")) {

    // Set the label
    strcpy(config.id,_nmeaIn->term(4));

    // Prepare the output
    sprintf(sentenceOut,"$FWACK,%s,id,%s*",
      config.id,config.id
    );

    // Set the save and reboot flag
    save=true;
    
  } else
  
  // Check if the target is "out"
  if (_nmeaIn->check(3,"out")) {
    
    // Get the out id
    int outId=(int)(_nmeaIn->term_decimal(4));

    // Check if the switch id is correct
    if (outId>=1 && outId<=20) {

      // Get the output pin
      int outPin=(int)(_nmeaIn->term_decimal(5));
  
      // Update the out table
      config.outs[outId-1]=outPin;
      
      // Make the setup effective since now
      pinMode(outPin,OUTPUT);
      //pinMode(inpPin,INPUT);
      delay(10);
      digitalWrite(outPin,LOW);

      // Prepare the output
      sprintf(sentenceOut,"$FWACK,%s,out,%d,%d,%d*",
        config.id,outId,
        config.outs[outId-1],
        digitalReadOutputPin(config.outs[outId-1])
      );
      
      // Set the save and reboot flag
      save=true;
    }
  } else
  // Check if the target is "inp"
  if (_nmeaIn->check(3,"inp")) {
    
    // Get the inp id
    int inpId=(int)(_nmeaIn->term_decimal(4));

    // Check if the switch id is correct
    if (inpId>=1 && inpId<=20) {

      // Get the input pin
      int inpPin=(int)(_nmeaIn->term_decimal(5));
  
     
      // Update the out table
      config.inps[inpId-1]=inpPin;
      
      // Make the setup effective since now
      pinMode(inpPin,INPUT);
      delay(10);
      
      // Prepare the output
      sprintf(sentenceOut,"$FWACK,%s,inp,%d,%d,%d*",
        config.id,inpId,
        config.inps[inpId-1],
        digitalRead(config.inps[inpId-1])
      );
      
      // Set the save and reboot flag
      save=true;
    }
  } else 
  // Check if the target is nmea or seatalk
  if (_nmeaIn->check(3,"nmea") || _nmeaIn->check(3,"seatalk")) {
    // $FWCMD,,conf_set,nmea,0,host,115200*52

    // The serial port number
    int port=(int)(_nmeaIn->term_decimal(4));

    // Check if the port number is correct
    if (port>=0 || port<=4) {

      // Set the label
      strcpy(config.serialLabel[port],_nmeaIn->term(5));

      // Check if is a nmea port
      if (_nmeaIn->check(3,"nmea")) {

        // set the baud rate
        config.serialBaud[port]=(long)(_nmeaIn->term_decimal(6));
        
        // Unset the seatalk port
        config.seaTalk &= ~(1 << port);
  
        // Set the save and reboot flag
        reboot=save=true;
      } else if (_nmeaIn->check(3,"seatalk")) {
    
        // Check if the port nunber is correct
        if (port==1 || port==2) {
          
          // Set the label
          strcpy(config.serialLabel[port],_nmeaIn->term(5));
    
          // Set the baud rate at 4800
          config.serialBaud[port]=4800l;
          
          // Set the seatalk port
          config.seaTalk |= 1 << port;
    
          // Set the save and reboot flag
          reboot=save=true;
        }
      }   
    }
  }
  
  // Check the save flag 
  if (save) {
    // Copy the config status
    memcpy(_model->getConfig(),&config,sizeof(Config));
    
    // Save on the EEPROM
    _model->saveConfig();
    delay(250);
  }

  // Check the reboot flag
  if (reboot) {
    // Reboot
    digitalWrite(6,LOW);
    Serial.println("Reset");
  }

  // Check the save flag
  if (save) {
    // Create the ack string
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));   
    nmeaOut->set(sentenceOut);
  }
}

/*
 * Process get configuration
 */
void FairWindCommands::fillConfiguration(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;

  // Initialize the ouput sentence
  strcpy(sentenceOut,"");

  // Check if the target is "ports"
  if (_nmeaIn->check(3,"ports")) {

    // Prepare the ACK string header
    sprintf(sentenceOut,"$FWACK,%s,conf,ports",_model->getConfig()->id);

    // For each port...
    for (int i=0;i<5;i++) {

      // Check the SeaTalk bit
      if (CHECK_BIT(_model->getConfig()->seaTalk,i)) {
        
        // Add the seatalk string
        strcat(sentenceOut,",seatalk");
      } else {

        // Add the nmea string
        strcat(sentenceOut,",nmea");
      }
    }

    // Complete the sequence with the asterisk
    strcat(sentenceOut,"*");

    // Set the done flag
    done=true;
    
  } else
  // Check if the target is nmea or seatalk
  if (_nmeaIn->check(3,"nmea") || _nmeaIn->check(3,"seatalk")) {
    // $FWCMD,,conf_get,nmea,0*41

    // The serial port number
    int port=(int)(_nmeaIn->term_decimal(4));
    
    // Check if the port number is correct
    if (port>=0 || port<=4) {

      // Check if the target and the seatalk flag are consistent
      if ((_nmeaIn->check(3,"nmea") && !_model->isSeaTalk(port))  || (_nmeaIn->check(2,"seatalk")) && _model->isSeaTalk(port)) {

        // Prepare the ACK string
        sprintf(sentenceOut,"$FWACK,%s,conf,%s,%d,%s,%lu*",
          _model->getConfig()->id,
          _nmeaIn->term(3),port,
          _model->getConfig()->serialLabel[port],_model->getConfig()->serialBaud[port]);

        // Set the done flag
        done=true;
      }
    }
  } else
  // Check if the target is "out"
  if (_nmeaIn->check(3,"out")) {
    
    // Get the out id
    int outId=(int)(_nmeaIn->term_decimal(4));

    // Check if the id is correct
    if (outId>=1 && outId<=20) {

      // Prepare the ACK string
      sprintf(sentenceOut,"$FWACK,%s,out,%d,%d*",
        _model->getConfig()->id,outId,
        _model->getConfig()->outs[outId-1]
      );
  
      // Set the done flag
      done=true;
    }
  } else
  // Check if the target is "inp"
  if (_nmeaIn->check(3,"inp")) {
    
    // Get the out id
    int inpId=(int)(_nmeaIn->term_decimal(4));

    // Check if the id is correct
    if (inpId>=1 && inpId<=20) {

      // Prepare the ACK string
      sprintf(sentenceOut,"$FWACK,%s,inp,%d,%d*",
        _model->getConfig()->id,inpId,
        _model->getConfig()->inps[inpId-1]
      );
  
      // Set the done flag
      done=true;
    }
  }

  // Check the done flag
  if (done) {

    // Set the output string
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut)); 
    nmeaOut->set(sentenceOut);
  } 
}

/*
 * Process get configuration
 */


void FairWindCommands::fillWho(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;
  
  sprintf(sentenceOut,"$FWACK,%s*",_model->getConfig()->id);
  done=true;

  if (done) {
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
}

void FairWindCommands::fillFirmwareVersion(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;
  
  sprintf(sentenceOut,"$FWACK,%s,ver,%s*",_model->getConfig()->id,FW_FIRMWARE_VERSION);
  done=true;

  if (done) {
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
}

/*
 * Set the output pin
 */
void FairWindCommands::setOut(NMEABase* nmeaOut) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;
  
  // Get the output id
  int outId=(int)(_nmeaIn->term_decimal(3));

  // Check if the id is correct
  if (outId>=1 && outId<=20) {

    // Check if the value is positive
    if ((int)(_nmeaIn->term_decimal(4))>0) {

      // Set the status to HIGH
      digitalWrite(_model->getConfig()->outs[outId-1],HIGH);
    } else {

      // Set the status to LOW
      digitalWrite(_model->getConfig()->outs[outId-1],LOW);
    }

    // Wait... no too much rush!
    delay(10);

    // Prepare the sentence
    sprintf(sentenceOut,"$FWACK,%s,out,%d,%d*",
      _model->getConfig()->id,
      outId,
      digitalReadOutputPin(_model->getConfig()->outs[outId-1]));

    // Set the done flag
    done=true;
  }
  
  if (done) {
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
  
}

/* 
 *  Get the status of the output pin
 */

void FairWindCommands::fillOut(NMEABase* nmeaOut) {
  // Get the output id
  int outId=(int)(_nmeaIn->term_decimal(3));

  fillOut(nmeaOut,outId);
}
void FairWindCommands::fillOut(NMEABase* nmeaOut, int outId) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;
  
  

  // Check if the id is correct
  if (outId>=1 && outId<=20) {

    // Prepare the sentence
    sprintf(sentenceOut,"$FWACK,%s,out,%d,%d*",
      _model->getConfig()->id,
      outId,
      digitalReadOutputPin(_model->getConfig()->outs[outId-1]));
    done=true;
  }
  
  if (done) {
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
  
}

/*
 * Get the status of the input pin
 */

void FairWindCommands::fillInp(NMEABase* nmeaOut) {
  // Get the input id
  int inpId=(int)(_nmeaIn->term_decimal(3));

  fillInp(nmeaOut,inpId);
}
void FairWindCommands::fillInp(NMEABase* nmeaOut, int inpId) {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  bool done=false;
  
  

  // Check if the input id is correct
  if (inpId>=1 && inpId<=20) {

    // Prepare the sentence
    sprintf(sentenceOut,"$FWACK,%s,inp,%d,%d*",
      _model->getConfig()->id,
      inpId,
      digitalRead(_model->getConfig()->inps[inpId-1]));

    // Set the done flag
    done=true;
  }
  
  if (done) {
    sprintf(sentenceOut,"%s%02X",sentenceOut,nmeaOut->calc_checksum(sentenceOut));
    nmeaOut->set(sentenceOut);
  }
  
}

/*
 * Process a command for the board if any
 * 
 * 
 * $FWWHO*41                              Board ID
 * $FWCMD,,ver*3A                         Firmware version
 * $FWCMD,,conf_get,ports*30              Port configuration
 * $FWCMD,,conf_get,nmea,0*41             Port configuration of nmea 0

 * $FWCMD,,conf_set,nmea,0,host,115200*52 Set the serial 0 as nmea named host
 * $FWCMD,,conf_set,out,1,4*25            Set the out 1 using the pin 4 as output
 * $FWCMD,,conf_set,out,1,24*17           Set the out 1 usibf the pin 24 as output
 * $FWCMD,,out_set,1,1*08                 Set the out 1 to on
 * $FWCMD,,out_get,1*01                   Return the state of the out 1
 * $FWCMD,,out_set,1,0*09                 Set the out 1 to off
 * $FWCMD,,conf_set,inp,1,12*0B           Set the inp 1 uning the pin 12 as input
 * $FWCMD,,inp_get,1*30                   Return the state of the inp 1
 * $FWCMD,,conf_get,out,1*55              Get the setup for the out 1
 * $FWCMD,,conf_get,inp,1*55              Get the setup for the input 1
 * $FWCMD,,conf_set,id,feder0*2F          Set the board id as feder0

 *      
 */
bool FairWindCommands::process(NMEABase* nmeaOut) {

  if (_nmeaIn->check(0,"FWWHO")) {
    fillWho(nmeaOut);
    return true;
  }

  // Check if the setting is for this board
  if (_nmeaIn->check(0,"FWCMD") && (_nmeaIn->check(1,_model->getConfig()->id) || _nmeaIn->check(1,"")) ) {

    // Manage the demo mode
    if (_nmeaIn->check(2,"demo")) {
      
      manageDemoMode(nmeaOut);
      return true;
    }
    // Check if is a config custom sentence
    else if (_nmeaIn->check(2,"conf_set")) {
      
      setConfiguration(nmeaOut);
      return true;
    }
    else if (_nmeaIn->check(2,"conf_get")) {
      fillConfiguration(nmeaOut);
      return true;
    }
    else if (_nmeaIn->check(2,"ver")) {
      fillFirmwareVersion(nmeaOut);
      return true;
    } 
    else if (_nmeaIn->check(2,"out_set")) {
      setOut(nmeaOut);
      return true;
    }
    else if (_nmeaIn->check(2,"out_get")) {
      fillOut(nmeaOut);
      return true;
    }
    else if (_nmeaIn->check(2,"inp_get")) {
      fillInp(nmeaOut);
      return true;
    }
  }

  // It is not a command
  return false;
}

