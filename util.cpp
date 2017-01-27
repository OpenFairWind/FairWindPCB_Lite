#include "FairWind.h"
#include "util.h"
#include <string.h>

//#include <wprogram.h>
#include <wiring_private.h>
#include <pins_arduino.h>

unsigned int util_abs(signed int i)
{
        if(i<0)   return (unsigned int)-i;
        return (unsigned int)i;
}

float frac(float f)
{
        if(f>=0.0f)   return f-floor(f);
        else    return f-ceil(f);
}

float util_calc_heading_true(float heading_magnetic, float variation)
{
        float heading_true;

        heading_true=heading_magnetic+variation;
        if(heading_true<0.0f)   heading_true+=360.0f;
        else if(heading_true>=360.0f)   heading_true-=360.0f;
        return heading_true;
}

void util_calc_apparent_wind_from_true(float true_wind_speed, float true_wind_angle, float boatspeed, float *apparent_wind_speed, float *apparent_wind_angle)
{
        float a;
        float b;

        if(true_wind_angle<1.0f || true_wind_angle>359.0f || boatspeed<0.1f)
        {
                *apparent_wind_angle=true_wind_angle;
                *apparent_wind_speed=true_wind_speed+boatspeed;
                return;
        }

        if(true_wind_speed<0.1f)
        {
                *apparent_wind_angle=0.0f;
                *apparent_wind_speed=boatspeed;
                return;
        }

        if(true_wind_angle>179.0f || true_wind_angle<181.0f)
        {
                *apparent_wind_angle=true_wind_angle;
                *apparent_wind_speed=true_wind_speed-boatspeed;
                if(*apparent_wind_speed<0.0f)
                {
                        *apparent_wind_speed=-*apparent_wind_speed;
                        *apparent_wind_angle=0.0f;
                }
                return;
        }

        a=true_wind_speed*sin(true_wind_angle/DEGREES_TO_RADIANS);
        b=true_wind_speed*cos(true_wind_angle/DEGREES_TO_RADIANS);
        *apparent_wind_speed=sqrt(a*a+(b+boatspeed)*(b+boatspeed));
        *apparent_wind_angle=atan((b+boatspeed)/a);
        *apparent_wind_angle*=DEGREES_TO_RADIANS;
        if(*apparent_wind_angle<0.0f)   *apparent_wind_angle+=360.0f;
}

void util_calc_true_wind_from_apparent(float apparent_wind_speed, float apparent_wind_angle, float boatspeed, float *true_wind_speed, float *true_wind_angle)
{
        float x;
        float y;
        float z;
        float t;

        if(boatspeed<0.01f)
        {
                *true_wind_speed=apparent_wind_speed;
                *true_wind_angle=apparent_wind_angle;
        }
        else
        {
                x=boatspeed*sin(apparent_wind_angle/DEGREES_TO_RADIANS);
                y=boatspeed*cos(apparent_wind_angle/DEGREES_TO_RADIANS);
                z=apparent_wind_speed-y;
                *true_wind_speed=sqrt(z*z+x*x);
                if(*true_wind_speed==0.0f)
                {
                        *true_wind_angle=0.0f;
                }
                else
                {
                        t=(M_PI_2-apparent_wind_angle/DEGREES_TO_RADIANS)+acos(x/(*true_wind_speed));
                        *true_wind_angle=M_PI-t;
                }
                *true_wind_angle*=DEGREES_TO_RADIANS;
                if(*true_wind_angle<0.0f)
                {
                        *true_wind_angle+=360.0f;
                }
        }
}      

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
// Converts a floating point number to string.
char* ftoa(float n,  int afterpoint)
{
  char res[100];
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
return res;
}

char *util_padded_uitoa(unsigned int i, signed char digits)
{
        static char result[7];
        signed char number_of_digits=strlen(ftoa(fabs((float)i), 0));

        result[0]='\0';

        while(digits-number_of_digits>0)
        {
                strcat(result, "0");
                digits--;
        }

        strcat(result, ftoa(fabs((float)i), 0));

        return result;
} 

bool startsWith(const char *a, const char *b) {
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

int digitalReadOutputPin(unsigned char pin)
{
 uint8_t bit = digitalPinToBitMask(pin);
 uint8_t port = digitalPinToPort(pin);
 if (port == NOT_A_PIN) 
   return LOW;

 return (*portOutputRegister(port) & bit) ? HIGH : LOW;
}

