#include <RCSwitch.h>

#include "com_codificated.h"

String com_codificated::invertCadena(String s) {
  String temporal = "";
  for (int m = s.length() - 1; m >= 0; m--)
    temporal += s[m];
  return temporal;
}

String com_codificated::decimalToBinary(long value){
  String result = "";
  for (int i = 0; i < 8; ++i){
    result = value%2 +result;
    value = value / 2;
  }
  return result;
}

long com_codificated::binaryToDecimal(String value){
    int base = 2; 
    int length = value.length() +1;
    char valueAsArray[length];
    value.toCharArray(valueAsArray, length);
    
    return strtol(valueAsArray, NULL, base);
}

char * com_codificated::dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}

void com_codificated::output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {
  String b = decimalToBinary(decimal);
  
  Serial.print("Decimal: ");
  Serial.print(decimal);
  Serial.print(" (");
  Serial.print( length );
  Serial.print("Bit) Binary: ");
  Serial.print( b );
  Serial.print(" PulseLength: ");
  Serial.print(delay);
  Serial.print(" microseconds");
  Serial.print(" Protocol: ");
  Serial.println(protocol);
  
  Serial.print("Raw data: ");
  for (unsigned int i=0; i<= length*2; i++) {
    Serial.print(raw[i]);
    if (i != length*2)Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

String com_codificated::create_IV(String h, String m, String s){

  String c0 = "", ret1 = "";
  int i, j;

  h = invertCadena(h);
  s = invertCadena(s);
  
  for (i = 0; i < 8; i++){
    if ((h[i] == '1' && m[i] == '0')||(h[i] == '0' && m[i] == '1'))
      ret1 += '1';

    else
      ret1 += '0';
  }

  for (j = 0; j < 8; j++){
    if ((ret1[j] == '1' && s[j] == '0')||(ret1[j] == '0' && s[j] == '1'))
      c0 += '1';

    else
      c0 += '0';
  }

  return c0;
}

binTime com_codificated::first_com (time_t t, RCSwitch& transmitter){
    binTime bin;
        
    //Guardamos tiempo y lo enviamos alternando con señal falso
    int hora = hour(t);
    int mins = minute(t);
    int sec = second(t);

    bin.b_h = decimalToBinary(hora);
    bin.b_m = decimalToBinary(mins);
    bin.b_s = decimalToBinary(sec);

    transmitter.send(hora, 8);
    Serial.print("Hora: ");
    Serial.print(hora);
    Serial.print("  ");
    Serial.print(bin.b_h);

    delay(500);
    transmitter.send(random(0,100), 8);

    delay(500);
    transmitter.send(mins, 8);
    Serial.print("  Minut: ");
    Serial.print(mins);
    Serial.print("  ");
    Serial.print(bin.b_m);

    delay(500);
    transmitter.send(random(0,100), 8);

    //delay(300);
    transmitter.send(sec, 8);
    Serial.print("  Segs: ");
    Serial.print(sec);
    Serial.print("  ");
    Serial.print(bin.b_s);
    Serial.println();

    return bin;
}

bool com_codificated::second_com (String c1, String c2,String c3, RCSwitch receiver){
    int i = 0;      
    bool correcte = receiver.getReceivedProtocol() == 1;
    String C[3] = {c1, c2, c3};
    while (correcte and i < 3){
        correcte = receiver.getReceivedValue() == binaryToDecimal(C[i]);
        Serial.print ("CIPHERS desencripted key: ");
        Serial.println(receiver.getReceivedValue());
        receiver.resetAvailable();
        delay(500);
         ++i;
    }
    if (correcte) Serial.println("status: cipher message OK");
    else Serial.println ("ERROR: wrong cipher key");
    return correcte;
}

String com_codificated::codificate (String c, String m){
        int i,j;
    String encod = "",encod1="",encod2="";
    String key[] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    
    for (i = 0; i < 8; i++){
        if ((c[i] == '1' && m[i] == '0')||(c[i] == '0' && m[i] == '1'))
        encod += '1';

        else
        encod += '0';
    }

    for(i=0;i<8;i++)
    {
        if(i<4){
        encod1 += encod[i];
        }
        else if(i>=4){
        encod2 += encod[i];
        }
        
    }
    
    if(key[0]==encod1 ) encod1 = key[7];
    
    else if(key[1]==encod1 ) encod1 = key[8];

    else if(key[2]==encod1 ) encod1 = key[4];
    
    else if(key[3]==encod1 ) encod1 = key[6];
    
    else if(key[4]==encod1 ) encod1 = key[9];
    
    else if(key[5]==encod1 ) encod1 = key[11];

    else if(key[6]==encod1 ) encod1 = key[13];
    
    else if(key[7]==encod1 ) encod1 = key[15];
    
    else if(key[8]==encod1 ) encod1 = key[1];
    
    else if(key[9]==encod1 ) encod1 = key[14];
    
    else if(key[10]==encod1 ) encod1 = key[0];
    
    else if(key[11]==encod1 ) encod1 = key[3];
    
    else if(key[12]==encod1 ) encod1 = key[2];
    
    else if(key[13]==encod1 ) encod1 = key[10];
    
    else if(key[14]==encod1 ) encod1 = key[12];

    else if(key[15]==encod1 ) encod1 = key[5];



    if(key[0]==encod2 ) encod2 = key[7];
    
    else if(key[1]==encod2 ) encod2 = key[8];

    else if(key[2]==encod2 ) encod2 = key[4];

    else if(key[3]==encod2 ) encod2 = key[6];

    else if(key[4]==encod2 ) encod2 = key[9];

    else if(key[5]==encod2 ) encod2 = key[11];

    else if(key[6]==encod2 ) encod2 = key[13];

    else if(key[7]==encod2 ) encod2 = key[15];

    else if(key[8]==encod2 ) encod2 = key[1];
    
    else if(key[9]==encod2 ) encod2 = key[14];

    else if(key[10]==encod2 ) encod2 = key[0];
    
    else if(key[11]==encod2 ) encod2 = key[3];
    
    else if(key[12]==encod2 ) encod2 = key[2];
    
    else if(key[13]==encod2 ) encod2 = key[10];
    
    else if(key[14]==encod2 ) encod2 = key[12];

    else if(key[15]==encod2 ) encod2 = key[5];

    encod = encod1 + encod2;

    return encod;
}
