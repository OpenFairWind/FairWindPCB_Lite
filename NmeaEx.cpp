#include "Arduino.h"
#include "NmeaEx.h"

void nmea_send(long tmp) {
  NMEAEx* nmeaEx=(NMEAEx*)tmp;
  char *sentence=nmeaEx->dequeue_message_to_send();
  if (sentence!=NULL) {
    nmeaEx->send(sentence);
  }
}

void NMEAEx::send(char* sentence) {
  if (_serial) {
      _serial->println(sentence);
      _serial->flush();
  } else {
    _mserial->println(sentence);
    _mserial->flush();
  }
}

void NMEABase::init() {
  _terms = 0;
  n = 0;
  _state = 0;
  _parity = 0;
  _nt = 0;

  f_sentence[0] = 0;
  f_terms = 0;
}

NMEABase::NMEABase() {
  init();
}

NMEABase::NMEABase(char* sentenceIn)  {
  init();
  set(sentenceIn);
}

NMEAEx::NMEAEx(HardwareSerial* serial, char *label, long interval, long serialBaud)  {
  NMEABase();

  _serial=serial;
  _mserial=NULL;
  _serial->begin(serialBaud, SERIAL_8N1);
  _interval=interval;
  if (_interval > 0 ) {
    _timer.setInterval(_interval, nmea_send,(long)this);
  }
  strcpy(this->_label,label);
}

NMEAEx::NMEAEx(MultiSerial* mserial, char *label,long interval, long serialBaud)  {
  NMEABase();
  
  _serial=NULL;
  _mserial=mserial;
  _mserial->begin(serialBaud, SERIAL_8N1);
  _interval=interval;
  if (_interval > 0 ) {
    _timer.setInterval(_interval, nmea_send,(long)this);
  }
  strcpy(this->_label,label);
}

//
// public methods
//


/****/
int NMEABase::decode(char c) {
  // avoid runaway sentences (>99 chars or >20 terms) and terms (>14 chars)
  if ((n >= 100) || (_terms >= 20) || (_nt >= 15)) { _state = 0; }
  // LF and CR always reset parser
  if ((c == 0x0A) || (c == 0x0D)) { _state = 0; }
  // '$' always starts a new sentence
  if (c == '$') {
    //_gprmc_tag = 0;
    _parity = 0;
    _terms = 0;
    _nt = 0;
    _sentence[0] = c;
    n = 1;
    _state = 1;
    return 0;
  }
  // parse other chars according to parser state
  switch(_state) {
    case 0:
      // waiting for '$', do nothing
      break;
      
    case 1:
      // decode chars after '$' and before '*' found
    
      // add received char to sentence
      //Serial.println(c);
      _sentence[n++] = c;
      switch (c) {
        
        case ',':
          // ',' delimits the individual terms
          (_term[_terms++])[_nt] = 0;
          _nt = 0;
          _parity = _parity ^ c;
          break;
          
        case '*':
          // '*' delimits term and precedes checksum term
          (_term[_terms++])[_nt] = 0;
          _nt = 0;
          _state++;
          break;
            
        default:
          // all other chars between '$' and '*' are part of a term
          (_term[_terms])[_nt++] = c;
          _parity = _parity ^ c;
          break;
      }
          
      break;
  
    case 2:
      // first char following '*' is checksum MSB
      _sentence[n++] = c;
      (_term[_terms])[_nt++] = c;
      _parity = _parity - (16 * _dehex(c));               // replace with bitshift?
      _state++;
      break;
      
    case 3:
      
      // second char after '*' completes the checksum (LSB)
      _sentence[n++] = c;
      _sentence[n++] = 0;
            
      (_term[_terms])[_nt++] = c;
      (_term[_terms++])[_nt] = 0;
      _state = 0;
      _parity = _parity - _dehex(c);
      
      // when parity is zero, checksum was correct!
      if (_parity == 0) {
        
        // accept all sentences, or only GPRMC datatype?
        
        // copy _sentence[] to f_sentence[]
        strcpy(f_sentence,_sentence);
        
        // copy all _terms[] to f_terms[]
        for (f_terms=0; f_terms<_terms; f_terms++) {
          _nt = 0;
          while ((_term[f_terms])[_nt]) {
            (f_term[f_terms])[_nt] = (_term[f_terms])[_nt];
            _nt++;
          }
          (f_term[f_terms])[_nt] = 0;
        }
                  
        // sentence accepted!
        
        return 1;
      
      }
      break;
      
    default:
      _state = 0;
      break;
  }
  return 0;
}

/***/

int NMEAEx::decode() {
  char c=0x00;
  if (_serial) { 
    c=_serial->read();
  } else if (_mserial) {
    c=_mserial->read();
  } 
  
  return NMEABase::decode(c);
}

void NMEABase::set(char* s) {
  
  unsigned int i=0;
  do {
    if (decode(s[i])) {
      break;
    }
    i++;
  } while (s[i]!=0x00); 
  
}

bool NMEABase::check(int i, char* key) {
  if (strcmp(key,term(i))==0) return true;
  return false; 
}

void NMEAEx::queue_message_to_send(char *sentence) {
  if (_interval>0) {
    nmeamsg msg;
    memcpy(msg.c,sentence,NMEA_MAX_MESSAGE_SIZE);
    msg.time=millis();
    _messages_out.enqueue(msg);
  } else {
    send(sentence);
  }
}

char* NMEAEx::dequeue_message_to_send() {
  nmeamsg msg=_messages_out.dequeue();
  if ((millis()-msg.time)<_obs) {
    return msg.c;
  }
  return NULL;
}

// Calculate the NMEA checksum
byte NMEABase::calc_checksum(char* str) {
  byte cs = 0; //clear any old checksum
  for (unsigned int n = 1; n < strlen(str) - 1; n++) {
    cs ^= str[n]; //calculates the checksum
  }
  return cs;
}

void NMEAEx::printDebug(char* msg) {
  char debugSentence[100];
  sprintf(debugSentence,"$FWDBG,%s,\"%s\",A*",_label,msg);
  byte cs = calc_checksum(debugSentence); //clear any old checksum
  sprintf(debugSentence,"%s%02X",debugSentence,cs);
  queue_message_to_send(debugSentence);
}

void NMEABase::fillSentence(char *s) {
  // returns last received full sentence as zero terminated string
  strcpy(s,f_sentence);
  Serial.println(s);
}


int NMEABase::terms() {
        // returns number of terms (including data type and checksum) in last received full sentence
  return f_terms;
}

char* NMEABase::term(int t) {
        // returns term t of last received full sentence as zero terminated string
  return f_term[t];
}

float NMEABase::term_decimal(int t) {
        // returns value of decimally coded term t
  return _decimal(f_term[t]);
}

int NMEABase::_dehex(char a) {
        // returns base-16 value of chars '0'-'9' and 'A'-'F';
        // does not trap invalid chars!
  if (int(a) >= 65) {
    return int(a)-55;
  }
  else {
    return int(a)-48;
  }
}

float NMEABase::_decimal(char* s) {
        // returns base-10 value of zero-termindated string
        // that contains only chars '+','-','0'-'9','.';
        // does not trap invalid strings!
  long  rl = 0;
  float rr = 0.0;
  float rb = 0.1;
  boolean dec = false;
  int i = 0;

  if ((s[i] == '-') || (s[i] == '+')) { i++; }
  while (s[i] != 0) {
    if (s[i] == '.') {
      dec = true;
    }
    else{
      if (!dec) {
        rl = (10 * rl) + (s[i] - 48);
      }
      else {
        rr += rb * (float)(s[i] - 48);
        rb /= 10.0;
      }
    }
    i++;
  }
  rr += (float)rl;
  if (s[0] == '-') {
    rr = 0.0 - rr;
  }
  return rr;
}

