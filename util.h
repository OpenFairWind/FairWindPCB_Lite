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
