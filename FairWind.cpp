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




