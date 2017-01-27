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

#ifndef SeatalkConstants_h
#define SeatalkConstants_h

#import "util.h"


#define SEATALK_MAX_MESSAGE_SIZE 18

// these are Seatalk message command identifiers
#define SEATALK_ID_DEPTH                   0x00
#define SEATALK_ID_BOATSPEED               0x20
#define SEATALK_ID_COMP_RUDD_AUTO          0x84
#define SEATALK_ID_COMP_RUDD               0x9c
#define SEATALK_ID_VARIATION               0x99
#define SEATALK_ID_TEMPERATURE             0x27
#define SEATALK_ID_TRIP                    0x21
#define SEATALK_ID_LOG                     0x22
#define SEATALK_ID_TRIPLOG                 0x25
#define SEATALK_ID_APPARENT_WIND_ANGLE     0x10
#define SEATALK_ID_APPARENT_WIND_SPEED     0x11
#define SEATALK_ID_SOG                     0x52
#define SEATALK_ID_COG                     0x53
#define SEATALK_ID_LATITUDE                0x50
#define SEATALK_ID_LONGITUDE               0x51
#define SEATALK_ID_GMT                     0x54
#define SEATALK_ID_DATE                    0x56
#define SEATALK_ID_SATINFO                 0x57
#define SEATALK_ID_UNKNOWN                 0xff

#endif
