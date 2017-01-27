#ifndef FAIRWINDCONSTANTS_H_
#define FAIRWINDCONSTANTS_H_

#define FW_FIRMWARE_VERSION "0.22b"

//debug
#define DEBUG true
#define MUX true

//SPI
#define MISO_PIN 50
#define M0SI_PIN 51
#define SLCK_PIN 52
#define SS_PIN 53 //should always be OUTPUT as arduino is master only
#define CS_PIN 67 //analog A13

//misc output - AltSoftSerial
//mux NMEA output
#define nmeaRxPin 48
#define nmeaTxPin 46

// 0.00053995680 nautical miles per meter
//decimal lat lon is in degrees, and we have 60 NM per degree so degrees per meter
#define  LLMTRS .00000899928
#define  MINMTRS        LLMTRS*5.0

//fairwind model structs
#define CONFIG_T 'C'
#define DYNAMIC_T 'D' //all dynamically changing params, gps ,speed, wind, compass, etc
#define SIMPLE_T 'S' //basic string format
//nav values
        /** version */
#define VER  "VER"
        /** Roll degrees */
#define RLL  "RLL"      //navigation.roll
        /** Pitch degrees */
#define PCH  "PCH" //navigation.pitch
        /** Yaw degrees*/
#define YAW  "YAW" //navigation.rateOfTurn
        /** IMU Health?*/
#define IMUH  "IMUH"
        /** Mag X - roll*/
#define MGX  "MGX"
        /** Mag Y - pitch*/
#define MGY  "MGY"
        /** Mag Z - heading */
#define MGZ  "MGZ"
        /** Mag Heading */
#define MGH  "MGH" //navigation.headingMagnetic
        /** Latitude*/
#define LAT  "LAT" //navigation.position.latitude
        /** Longitude*/
#define LON  "LON" //navigation.position.longitude
        /**Altitude */
#define ALT  "ALT" //navigation.position.altitude
        /**CourseOverGround - true*/
#define COG  "COG" //navigation.courseOverGroundTrue

        /** Speed over ground */
#define SOG  "SOG" //navigation.speedOverGround
        /** GPS fix true/false*/
#define FIX  "FIX" //environmental.fix
        /** satellites*/
#define SAT  "SAT" //environmental.sat
        /** Time of week*/
#define TOW  "TOW" //environmental.timeOfWeek
        /**mag declination*/
#define DECL  "DEC" //navigation.declination
        /**Wind speed apparent*/
#define WSA  "WSA" //environmental.wind.speedApparent
        /**Wind speed true*/
#define WST  "WST" //environmental.wind.speedTrue
        /**Wind dir apparent*/
#define WDA  "WDA" //environmental.wind.directionApparent
        /**Wind dir true*/
#define WDT  "WDT" //environmental.wind.directionTrue
        /**Wind speed units*/
#define WSU  "WSU"
/**Wind alarm speed (knots) */
#define WSK  "WSK" //environmental.wind.speedAlarm
        /**Wind speed alarm state */
#define WSX  "WSX" //alarms.windAlarmState

        /** Autopilot state (on/off)*/
#define APX  "APX" //0off, 1on steering.autopilot.state
        /** Autopilot offset from source*/
#define APT  "APT" //offset from source +/- deg steering.autopilot.offset
        /** Autopilot source*/
#define APS  "APS" //W wind or C compass steering.autopilot.headingSource
        /** Autopilot current Heading*/
#define APC  "APC" //0-360  steering.autopilot.currentHeading
        /** Autopilot rudder angle*/ //steering.rudderAngle
#define APR  "APR" // +/- deg, +ve is rudder to starboard, vessel turns to starboard

        /**Anchor alarm state*/
#define AAX  "AAX" //alarms.anchorAlarmState
        /**Anchor alarm radius*/
#define AAR  "AAR" //alarms.anchorAlarmRadius
        /**Anchor distance*/
#define AAD  "AAD" //alarms.anchorDistance
        /**Anchor alarm Lat*/
#define AAN  "AAN" //alarms.anchorAlarmLat
        /**Anchor alarm Lon*/
#define AAE  "AAE" //alarms.anchorAlarmLon

//attached device types
#define UID  "UID"
#define IMU  "IMU"
#define MEGA  "MEGA"

//Commands
/*
#define ANCHOR_ALARM_STATE "#AAX"
#define ANCHOR_ALARM_RADIUS "#AAR"
#define ANCHOR_ALARM_ADJUST "#AAJ"
#define ANCHOR_ALARM_LAT "#AAN"
#define ANCHOR_ALARM_LON "#AAE"

#define WIND_SPEED_ALARM_STATE "#WSX"
#define WIND_ALARM_KNOTS "#WSK"
#define WIND_ZERO_ADJUST "#WZJ"

#define GPS_MODEL "#GPS"
*/
#define SERIAL_BAUD0 "#SB0"
#define SERIAL_BAUD1 "#SB1"
#define SERIAL_BAUD2 "#SB2"
#define SERIAL_BAUD3 "#SB3"
#define SEATALK "#STK"
/*
#define LEVEL1_UPPER_ALARM "#LU1"
#define LEVEL2_UPPER_ALARM "#LU2"
#define LEVEL3_UPPER_ALARM "#LU3"
#define LEVEL1_LOWER_ALARM "#LU1"
#define LEVEL2_LOWER_ALARM "#LU2"
#define LEVEL3_LOWER_ALARM "#LU3"
*/
#define CONFIG "#CFG"

#define NMEA_MAX_MESSAGE_SIZE 100

#endif

