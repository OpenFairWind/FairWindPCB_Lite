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

#ifndef _UTIL_H
#define _UTIL_H


#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define SET_BIT(var,pos) ((var) |= (1<<(pos))) 
#define CLEAR_BIT(var,pos) ((var) &= ~(1<<(pos))) 
#define TOGGLE_BIT(var,pos) ((var) ^= (1<<(pos))) 

#define M_PI                                    3.1416f
#define M_PI_2                                  1.5708f
#define DEGREES_TO_RADIANS                      57.259f

float util_calc_heading_true(float heading_magnetic, float variation);
float frac(float f);
unsigned int util_abs(signed int i);
void util_calc_apparent_wind_from_true(float true_wind_speed, float true_wind_angle, float boatspeed, float *apparent_wind_speed, float *apparent_wind_angle);
void util_calc_true_wind_from_apparent(float apparent_wind_speed, float apparent_wind_angle, float boatspeed, float *true_wind_speed, float *true_wind_angle);
char *util_padded_uitoa(unsigned int i, signed char p);
char* ftoa(float n, int afterpoint);
bool startsWith(const char *a, const char *b);
int digitalReadOutputPin(unsigned char pin);
#endif
