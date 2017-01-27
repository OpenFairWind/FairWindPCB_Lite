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

#include "FairWindModel.h"

/*
 * The FairWind model constructor
 */
FairWindModel::FairWindModel() {
   
  _demo_mode=false;
  _demo_current=&_demo[0];

  

  strcpy(_config.id,"fw0000");
  
  // Set the default baud rates
  _config.serialBaud[0]  = 115200l; //main
  //_config.serialBaud[0]  = 9600l; //main
  _config.serialBaud[1] = 4800l; //seatalk
  _config.serialBaud[2] = 4800l; //seatalk
  _config.serialBaud[3] = 4800l; //nmea
  _config.serialBaud[4] = 4800l; //nmea

  // Set the default labels
  strcpy(_config.serialLabel[0],"com0up");
  strcpy(_config.serialLabel[1],"nmea1i");
  strcpy(_config.serialLabel[2],"nmea2o");
  strcpy(_config.serialLabel[3],"com3in");
  strcpy(_config.serialLabel[4],"com4ot");

  // Set the ports 1 and 2 as Seatalk
  _config.seaTalk = 0b100;

  // Set switch table
  for (int i=0;i<20;i++) {
    _config.outs[i]=13;
    _config.inps[i]=12;
  }

  // Set the default update interval
  _config.status_interval=2000;

  // Set the EEPROM version
  _version = EEPROM_VER;

  
  
  // Read the configuration 
  saveConfig();
  //readConfig();

  

  // Set the outs and inps configuration
  for (int i=0;i<20;i++) {
    pinMode(_config.outs[i], OUTPUT); 
    pinMode(_config.inps[i], INPUT);
    delay(10);
    digitalWrite(_config.outs[i],LOW); // By default all switches off -- I dont like it! Use the EEPROM! 
  }

  //Serial.begin(115200);
  //Serial.println("ok");
  

  // Main serial host
  _nmea[0] = new NMEAEx(&Serial,getSerialLabel(0),0,getSerialBaud(0));

  

  // Hardware serial 3
  _nmea[3]=new NMEAEx(&Serial3,getSerialLabel(3),0,getSerialBaud(3));
  
 
  
  // Software serial 4
  pinMode(CS_PIN, OUTPUT);
  delay(100);
  digitalWrite(CS_PIN,HIGH);
  _mSerial1=MultiSerial(CS_PIN,1);
  _nmea[4]=new NMEAEx(&_mSerial1,getSerialLabel(4),125,getSerialBaud(4));

   

  // initialize  serial ports 1 and 2
  // Serial 1      
  if (isSeaTalk(1)) {
    
    // Create the Seatalk object
    _seatalk[1]=new Seatalk(&Serial1, getSerialLabel(1),250,getSerialBaud(1),_nmea[0]);
    //Serial.println("$FWMSG,01,SEATALK");
  } else {
    // Create the NMEAEx object
    _nmea[1]=new NMEAEx(&Serial1,getSerialLabel(1),0,getSerialBaud(1));
    //Serial.println("$FWMSG,01,NMEA");
  }
delay(10);
  
  
  // Serial 2     
  if (isSeaTalk(2)) {
    
    // Create the Seatalk object
    _seatalk[2]=new Seatalk(&Serial2, getSerialLabel(2),250,getSerialBaud(2),_nmea[0]);
    //Serial.println("$FWMSG,02,SEATALK");
  } else {
    
    // Create the NMEAEx object
    _nmea[2]=new NMEAEx(&Serial2,getSerialLabel(2),0,getSerialBaud(2));
    //Serial.println("$FWMSG,02,NMEA");
  }
  delay(10);

  // Set the debug led
  //pinMode(5,OUTPUT);
  //pinMode(4,OUTPUT);
  //digitalWrite(4,LOW);
  //digitalWrite(5,HIGH);

  _commands=new FairWindCommands(this,_nmea[0]);

  sayFairWind();

  // For each input... 
  for(int i=0;i<20;i++) {
    // Set the current state
    _last_inp_states[i]=digitalRead(_config.inps[i]);
  }
  _last_millis=millis();
}



template<class T> int EEPROM_writeAnything(int ee, T& value) {
        unsigned char* p = (unsigned char*) (void*) &value;
        unsigned int i;
        for (i = 0; i < sizeof(value); i++)
                EEPROM.write(ee++, *p++);
        return i;
}
//saving
template<class T> int EEPROM_readAnything(int ee, T& value) {
        unsigned char* p = (unsigned char*) (void*) &value;
        unsigned int i;
        for (i = 0; i < sizeof(value); i++)
                *p++ = EEPROM.read(ee++);
        return i;
}

/*
 * Save the Configuration on the EEPROM
 */
void FairWindModel::saveConfig() {
  
  //write out a current version
  EEPROM_writeAnything(0, _version);
  
  //write data
  EEPROM_writeAnything(EEPROM_DATA, _config);
}

/*
 * Read the Configuration from the EEPROM
 */
void FairWindModel::readConfig() {
  
  //check versions here
  int ver;
  EEPROM_readAnything(0, ver);
  if (ver != _version) {
    
    //save a default config, since we cant read the old one safely
    saveConfig();
  }

  //now we know its compatible
  EEPROM_readAnything(EEPROM_DATA, _config);

}

/*
 * Get the serial label setup given an index
 */
char* FairWindModel::getSerialLabel(int i) {
  return _config.serialLabel[i];
}

/*
 * Set the serial label setup given an index and the value
 */
void FairWindModel::setSerialLabel(int i, char* serialLabel) {
  strcpy(_config.serialLabel[i],serialLabel);
}

/*
 * Get the serial baud setup given an index
 */
long FairWindModel::getSerialBaud(int i) {
  return _config.serialBaud[i];
}

/*
 * Set the serial baud setup given an index and the value
 */
void FairWindModel::setSerialBaud(int i, long serialBaud) {
  _config.serialBaud[i] = serialBaud;
}


/*
 * Get the seatalk mask
 * 0x00 - no seatalk, ports 0 and 1 set as nmea
 * 0x01 - seatalk on port 0, nmea on port 1
 * 0x02 - seatalk on port 1, nmea on port 0
 * 0x03 - seatalk on both ports 0 and 1
 */
int FairWindModel::getSeaTalk() {
  return _config.seaTalk;
}

/*
 * Set the seatalk mask
 * 0b00000 - no seatalk, ports 1..4 set as nmea
 * 0b00010 - seatalk on port 1, nmea on port 2..4
 * 0b00100 - seatalk on port 2, nmea on port 1,3,4
 * 0b00110 - seatalk on both ports 1 and 2
 */
void FairWindModel::setSeaTalk(int seaTalk) {
  _config.seaTalk = seaTalk;
}

/*
 * Check if a given serial port is configured as SeaTalk
 */
bool FairWindModel::isSeaTalk(int serialNumber) {
  
  // Check if the serial number is 1 or 2
  if (serialNumber == 1 || serialNumber == 2) {
    // Check if the given serial is used as SeaTalk
    if (CHECK_BIT(_config.seaTalk,serialNumber)) return true;
  }
  return false;
}

/*
 * Say the FairWind version as Hello
 */
void FairWindModel::sayFairWind() {
  NMEABase nmeaOut;
  _commands->fillFirmwareVersion(&nmeaOut);
  _nmea[0]->queue_message_to_send(nmeaOut.f_sentence);
}

/*
 * Check if the status of any input changes
 */
void FairWindModel::check() {
  NMEABase nmeaOut;

  // For each input...
  for(int i=0;i<20;i++) {
    
    // Check if the state differs from its last one
    if (digitalRead(_config.inps[i])!=_last_inp_states[i]) {

      // Fill the sentence
      _commands->fillInp(&nmeaOut,i+1);

      // Enque the sentence to Serial0
      _nmea[0]->queue_message_to_send(nmeaOut.f_sentence);
    }

    // Update the last state
    _last_inp_states[i]=digitalRead(_config.inps[i]);
  }

  //Serial.println(millis()-_last_millis); 

  if ((millis()-_last_millis)>_config.status_interval) {  
     
    // Version
    // Fill the sentence
    _commands->fillStatus(&nmeaOut);

    // Enque the sentence to Serial0
    _nmea[0]->queue_message_to_send(nmeaOut.f_sentence);

    // Check if demo mode
    if (_demo_mode) {
      
      _nmea[0]->queue_message_to_send(*_demo_current);
      _demo_current++;
      if (*_demo_current==NULL) {
        _demo_current=&_demo[0];
      }
    }
    _last_millis=millis();
  }
  
}

/*
 * Serial event manager for host (serial0)
 */
void FairWindModel::serialEvent() {
  byte messageIn[SEATALK_MAX_MESSAGE_SIZE];
  NMEABase nmeaOut;
  
  // While serial data is available...
  while (getSerial(0)->available()) {
    
    // Decode the nmea sequence from the talker
    _inputSerialComplete[0] = _nmea[0]->decode();
    delay(10);
    // Check if the input from serial1 is complete
    if (_inputSerialComplete[0]) {
  
      // Check if it is a command for the board
      if (_commands->process(&nmeaOut)) {
        // Send back the command result
        _nmea[0]->queue_message_to_send(nmeaOut.f_sentence);
      } else {
        //Serial.println("..");
        //Serial.println(nmeaIn.f_sentence);
        //Serial.println("..");

        // Check if the Serial1 is configured as SeaTalk
        if (isSeaTalk(1)) {
          //Serial.println("1");
          // Create a seatalk message from nmeaIn
          if(_seatalk[1]->nmea2message(_nmea[0],messageIn)) {
            //Serial.println("2");
            // Write the NMEA sentence of SeaTalk
            _seatalk[1]->queue_message_to_send(messageIn);
            //Serial.println("3");
          }
        } else {
          // Write the NMEA Sentence on Serial1
          _nmea[1]->queue_message_to_send(_nmea[0]->f_sentence);
        }

        // Check if the Serial2 is configured as SeaTalk
        if (isSeaTalk(2)) {
          
          // Write the NMEA sentence of SeaTalk
          _seatalk[2]->nmea2message(_nmea[0],messageIn);
          _seatalk[2]->queue_message_to_send(messageIn);
        } else {
          
          // Write the NMEA Sentence on Serial2
          _nmea[2]->queue_message_to_send(_nmea[0]->f_sentence);
        }
  
        // Write the NMEA sentence of Serial3
        _nmea[3]->queue_message_to_send(_nmea[0]->f_sentence);
    
        // Write the NMEA sentence of Serial4
        _nmea[4]->queue_message_to_send(_nmea[0]->f_sentence);
        
        break;
      }
         
    }
    
  }
}

HardwareSerial* FairWindModel::getSerial(int serialNumber) {
  if (serialNumber>=1 && serialNumber<=2) {
    if (isSeaTalk(serialNumber)) return _seatalk[serialNumber]->getSerial();
  }
  return _nmea[serialNumber]->getSerial();
}

/*
 * Serial event manager for nmea or seatalk (serial1)
 */
void FairWindModel::serialEvent1() {
  char sentenceOut[NMEA_MAX_MESSAGE_SIZE];
  
  // While serial data is available...
  while (getSerial(1)->available()) {
    
    
    // Check if the serial is configured as SeaTalk
    if (isSeaTalk(1)) {
       
      // Process the SeaTalk data
      _inputSerialComplete[1] = _seatalk[1]->processSeaTalkByte();
       
    } else {
      
      // Decode the nmea sequence from the talker
      _inputSerialComplete[1] = _nmea[1]->decode();
    }
    //delay(10);
    
    // Check if the input from serial1 is complete
    if (_inputSerialComplete[1]) {
      
      // Check if the serial is configured as SeaTalk
      if (isSeaTalk(1)) {
        
        // Set the talker sequence from SeaTalk
        _seatalk[1]->message2nmea(_seatalk[1]->getMessage(),sentenceOut); 
        _nmea[0]->queue_message_to_send(sentenceOut);
        
      } else {
        // Send the sentence on serial0
        _nmea[0]->queue_message_to_send(_nmea[1]->f_sentence);
      }
      //loop every sentence
      break;
    }
  }
  
}

/*
 * Serial event manager for nmea or seatalk (serial2)
 */
void FairWindModel::serialEvent2() {
  char sentenceIn[NMEA_MAX_MESSAGE_SIZE];

  
  
  // While serial data is available...
  while (getSerial(2)->available()) {
    
    // Check if the serial is configured as SeaTalk
    if (isSeaTalk(2)) {
      
      // Process the SeaTalk data
      _inputSerialComplete[2] = _seatalk[2]->processSeaTalkByte();
    } else {
      
      // Decode the nmea sequence from the talker
      _inputSerialComplete[2] = _nmea[2]->decode();
    }
    //delay(10);
    
    // Check if the input from serial1 is complete
    if (_inputSerialComplete[2]) {
      
      // Check if the serial is configured as SeaTalk
      if (isSeaTalk(2)) {
        
        // Set the talker sequence from SeaTalk
        _seatalk[2]->message2nmea(_seatalk[2]->getMessage(),sentenceIn);
        _nmea[0]->queue_message_to_send(sentenceIn);
      } else {
        
        // Send the sentence on serial0
        _nmea[0]->queue_message_to_send(_nmea[2]->f_sentence);
      }
      //loop every sentence
      break;
    }
  }
}

/*
 * Serial event manager for nmea (serial3)
 */
void FairWindModel::serialEvent3() {
  
  // While serial data is available...
  while (getSerial(3)->available()) {
    
    // Decode the nmea sequence from the talker
    _inputSerialComplete[3] = _nmea[3]->decode();
    //delay(10);
    
    // Check if the input from serial1 is complete
    if (_inputSerialComplete[3]) {
      
      // Send the sentence on serial0
      _nmea[0]->queue_message_to_send(_nmea[3]->f_sentence);
      
      break;
    }
  }
}

/*
 * Serial event manager for nmeaa (serial4)
 * Poll it in the loop!
 */
void FairWindModel::serialEvent4() {
  
  // While serial data is available...
  while (_mSerial1.available()) {
    
    // Decode the nmea sequence from the talker
    _inputSerialComplete[4] = _nmea[4]->decode();
    //delay(10);
    
    // Check if the input from serial1 is complete
    if (_inputSerialComplete[4]) {
      
      // Send the sentence on serial0
      _nmea[0]->queue_message_to_send(_nmea[4]->f_sentence);
      
      break;
    }
  }
}


    
    
 




