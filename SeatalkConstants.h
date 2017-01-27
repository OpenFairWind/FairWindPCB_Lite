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
