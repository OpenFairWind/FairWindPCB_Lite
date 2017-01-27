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

#ifndef NmeaEx_h
#define NmeaEx_h


#include "FairWindConstants.h"
#include <MultiSerial.h>
#include <SimpleTimer.h>
#include <QueueArray.h>

typedef struct nmeamsg_tag { char c[NMEA_MAX_MESSAGE_SIZE]; long time; } nmeamsg;

class NMEABase
{
  public:
    NMEABase();
    NMEABase(char* sentence);
    
    int decode(char c);  
    void fillSentence(char *s); // fill the last received full sentence as zero terminated string
                                                 
    int terms();                                                        // returns number of terms (including data type and checksum) in last received full sentence
    char* term(int t);                                    // returns term t of last received full sentence as zero terminated string
    float term_decimal(int t);    // returns the base-10 converted value of term[t] in last full sentence received

                                  
    // Set the nmea sentence
    void set(char* s); 

    // Check if the sentence matches a key (key = XXYYY)
    bool check(int i, char* key);
    
    // Calc the nmea checksum
    static byte calc_checksum(char* str);
 
    char    f_sentence[100];
    
  private:

    char   f_term[20][15];
    int             f_terms;
    int             _terms;
    char    _sentence[100];
    char   _term[20][15];
    int             n;
    
    int             _state;
    int             _parity;
    int             _nt;
    
    
    int _dehex(char a);
    float _decimal(char* s);
    void init();
};

class NMEAEx: public NMEABase {
  
  public:
    
    NMEAEx(HardwareSerial* serial, char *label,long interval, long serialBaud);  
    NMEAEx(MultiSerial* mserial, char *label, long interval, long serialBaud);

    
    
    int decode(); 
                                                 
    void send(char* sentence);
                                  
    
    
    void queue_message_to_send(char *sentence);
    char* dequeue_message_to_send();

    // Get the serial port pointer
    HardwareSerial* getSerial() {return _serial;}
    
    // Get the serial port pointer
    MultiSerial* getMultiSerial() {return _mserial;}
    
    // Get the label
    char *getLabel() { return _label; }

    // Print debug message
    void printDebug(char* msg);

    
    
  private:
    HardwareSerial* _serial;
    MultiSerial* _mserial;

    // The sending queue
    QueueArray <nmeamsg> _messages_out;
  
    // The sending queue timer
    SimpleTimer _timer;
  
    // The sending queue timer interval
    long _interval=100;
  
    // Obsolescense tine in mills
    long _obs=5000;
  
    // The label
    char _label[8];

};

void nmea_send(NMEAEx *nmeaEx);
#endif

