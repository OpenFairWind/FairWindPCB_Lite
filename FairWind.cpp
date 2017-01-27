#include "FairWind.h"

//fairwind model
FairWindModel *model=NULL;

/*
 * The Arduino setup hook
 */
void setup() {
  
  digitalWrite(6, HIGH);
  delay(200);   
  pinMode(6, OUTPUT); 
  delay(2000);
  
  // Create the model object
  model = new FairWindModel();
  
}

void serialEvent() {
  model->serialEvent();
  delay(10);
}

// Serial1 event manager
void serialEvent1() {
  model->serialEvent1();
  delay(10);
}

// Serial2 event manager
void serialEvent2() {
  model->serialEvent2();
  delay(10);
}

// Serial3 event manager
void serialEvent3() {
  model->serialEvent3();
  delay(10);
}

// Serial4 event manager
void serialEvent4() {
  model->serialEvent4();
  delay(10);
}
  
void loop() {
  
  // put your main code here, to run repeatedly:
  //check SPI for incoming
  serialEvent4();
  model->check();
  
  delay(10);
  
}




