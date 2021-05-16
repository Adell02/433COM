#include <TimeLib.h>

#include <RCSwitch.h>

#include "struct_binTime.h"
#include "Arduino.h"

#ifndef COM_CODIFICATED_HH
#define COM_CODIFICATED_HH


class com_codificated{
private:

  static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);

public:

    String create_IV(String h, String m, String s);
    
    binTime first_com (time_t t, RCSwitch& transmitter);    
    
    bool second_com (String c1, String c2,String c3, RCSwitch receiver);

    String codificate (String c, String m);

    static String invertCadena(String s);

    static String decimalToBinary(long value);

    static void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol);

     static long binaryToDecimal(String value);
    
};

#endif
