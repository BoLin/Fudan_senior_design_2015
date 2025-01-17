#include <Wire.h>
#include "TSL2561.h"
TSL2561 tsl(TSL2561_ADDR_FLOAT); 

void setup(void) {
  Serial.begin(9600);
 
  pinMode(9, OUTPUT);  // initialize the ledPin as an output:
  pinMode(10, OUTPUT);  // initialize the ledPin as an output:
  
  if (tsl.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor?");
    while (1);
  }
    
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  tsl.setGain(TSL2561_GAIN_0X);         // set no gain (for bright situtations)
  //tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
  tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
  
  // Now we're ready to get readings!
}

void loop(void) {
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  
  Serial.println(x, DEC);

  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");
  
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
  
  uint16_t l = tsl.calculateLux(full, ir);
   Serial.print("l: "); Serial.print(l);Serial.print("\t");
  byte brightness;
  int set=5000;//set test lux
  byte odd;
  odd=set-l+29.7*brightness;
  brightness=0.03367*odd;
   
  Serial.print("Brightness: "); Serial.print(brightness); Serial.print("\n\n");
   
  byte darkness;
  darkness=255-brightness;
  analogWrite(10, darkness);
     
  
  delay(1000); 
}
