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

#include "SeatalkConstants.h"
#include "Seatalk.h"
#include "util.h"

// Send the next valid SeaTalk message
void seatalk_send(long tmp) {
  Seatalk* seatalk=(Seatalk*)tmp;
  unsigned char *message=seatalk->dequeue_message_to_send();
  if (message!=NULL) {
    seatalk->send(message);
  }
}

void Seatalk::send(unsigned char *message) {
  int len=(message[1]&0x0f)+3;
  _serial->write(message[0] | 0x100);
  for (unsigned int i=1;i<len;i++) {
    _serial->write(message[i]);
  }
  _serial->flush();
}

// Enqueue a SeaTalk message to the sending queue
void Seatalk::queue_message_to_send(unsigned char *message) {
  // Check if the queue is active
  if (_interval>0) {
  // Allocate the message statically
  stmsg msg;

  // Copy the message data
  memcpy(msg.c,message,SEATALK_MAX_MESSAGE_SIZE);

  // Set the message time
  msg.time=millis();
  
  // Enqueue
  _messages_out.enqueue(msg);
  } else {
    // Send the message without the sending queue
    send(message);  
  }
}


// Dequeue a SeaTalk message from the sending queue
unsigned char* Seatalk::dequeue_message_to_send() {
  // Check if the queue is not empty
  if (!_messages_out.isEmpty()) {
    // Dequeue the message
    stmsg msg=_messages_out.dequeue();
  
    // Check if the message is not obsolete
    if ((millis()-msg.time)<_obs) {
      // Return the message content
      return msg.c;
    }
  }
  // No message if available.
  return NULL;
}

// Class constructor
Seatalk::Seatalk( HardwareSerial* serial,  char *label, long interval, long serialBaud, NMEAEx* nmeaEx) {
  // Set the hardware serial
  _serial=serial;
  _serial->begin(serialBaud,SERIAL_9N1);

  
  // Set the interval
  _interval=interval;
  
  // Set the sending queue timer if needed
  if (_interval > 0) {
    _timer.setInterval(_interval, seatalk_send,(long)this);
  }
  // Set the label
  strcpy(this->_label,label);

  

  // Set the NMEAEx pointer
  this->_nmeaEx=nmeaEx;

  // Set the read byte initial position
  _seaTalkPos = 0;  
}

// Process the incoming byte
int Seatalk::processSeaTalkByte() {
  uint16_t inByte=_serial->read();
  int result=0;
  if (inByte >= 256) {
    memcpy(_message,_seatalkStream,(_seatalkStream[1]&0x0f)+3);
    
    _seaTalkPos = 0;
    // Serial.print("Command:");
    inByte = inByte - 256;
    result=1;
  }
  _seatalkStream[_seaTalkPos] = inByte;
  if (_seaTalkPos < SEATALK_MAX_MESSAGE_SIZE) {
    _seaTalkPos++;
  }
  return result;
}
  
// Set a seatalk message from a seatalk over nmea sequence
bool Seatalk::nmea2message(NMEABase* nmea, byte* message) {

  // Check if the sentence is seatalk over nmea
  if (strcmp("STALK",nmea->term(0))==0) {
    // Check the target
    //if (strcmp(_label,nmea->term(1))==0 || strcmp("",nmea->term(1))==0) {
      // for each term from the third to the last minus one...
      
      for (int i=1;i<nmea->terms();i++) {
        // Get the byte and set the message
        sscanf(nmea->term(i),"%02X",&message[i-1]);
      }
      // Return with a valid message
      return true;
      
    //}
  }
  // No fat for cats
  return false;
}

// Get the nmea representation of a seatalk message
void Seatalk::message2nmea(byte* message, char *sentenceOut) {
  
  char buf[4];
  int c;
  
  // Initialize the string builder
  strcpy(sentenceOut,"$STALK");
  

  // Add the label
  //strcat(sentenceOut,_label);

  // Calculate the message lenght
  int len=(message[1]&0x0f)+3;

  
  
  // For each byte in the messagge, add a term
  for (int i=0;i<len;i++) {
    sprintf(buf,",%02X",message[i] & 0xff);
    strcat(sentenceOut,buf);
  }
  strcat(sentenceOut,"*");

  // Add the checksum
  // Calculate the NMEA checksum
  byte cs = 0; //clear any old checksum
  for (unsigned int n = 1; n < strlen(sentenceOut) - 1; n++) {
    cs ^= sentenceOut[n]; //calculates the checksum
  }
  sprintf(buf,"%02X",cs);
  strcat(sentenceOut,buf);
  
  
}



