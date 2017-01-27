
#ifndef SEATALK_H_
#define SEATALK_H_

#include "Arduino.h"
#include "SeaTalkConstants.h"
#include <PString.h>
#include <QueueArray.h>
#include <SimpleTimer.h>
#include "NMEAEx.h"


// The struct for the outcoming SeaTalk message
typedef struct stmsg_tag { unsigned char c[SEATALK_MAX_MESSAGE_SIZE]; long time; } stmsg;

/***************************************
 * The SeaTalk class
 */
class Seatalk {

  public:
    Seatalk( HardwareSerial* serial,  char *label, long interval, long serialBaud, NMEAEx *nmeaEx);

    // Process a seatalk byte
    int processSeaTalkByte();

    // Get the sentence representation of a seatalk message    
    void message2nmea(byte* message, char* sentence);

    // Send a seatalk message from a seatalk over nmea sentence
    bool nmea2message(NMEABase* nmea, byte* message);

    // Send a message to seatalk port
    void send(unsigned char *message);

    // Add a seatalk message to the sending queue
    void queue_message_to_send(unsigned char *message);

    // Get the next message from the sending queue
    unsigned char* dequeue_message_to_send();

    // Get the last received byte
    //byte getLastByte() { return _lastByte; }

    // Get the seatalk message
    byte *getMessage() { return _message; }
      
    // Get the serial port pointer
    HardwareSerial* getSerial() {return _serial;}
  
    // Get the label
    char *getLabel() { return _label; }

private:
  // The sending queue
  QueueArray <stmsg> _messages_out;

  // The sending queue timer
  SimpleTimer _timer;

  // The sending queue timer interval
  long _interval=100;

  // Obsolescense tine in mills
  long _obs=2500;
  

  // The label
  char _label[8];
        
  // The datagram byte current position
  int _seaTalkPos;

  // The datagram is 3-18 chars
  byte _seatalkStream[SEATALK_MAX_MESSAGE_SIZE]; 
  byte _message[SEATALK_MAX_MESSAGE_SIZE]; 

  // The serial port 
  HardwareSerial* _serial;

  // NMEA Ex pointer
  NMEAEx* _nmeaEx;
  
};

void seatalk_send(Seatalk *seatalk);

#endif

