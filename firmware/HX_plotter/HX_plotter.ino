//
//    FILE: HX_plotter.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711


#include "HX711.h"

HX711 scale;

//  adjust pins if needed
uint8_t dataPin = 6;
uint8_t clockPin = 7;


void setup()
{
  Serial.begin(115200);
  //  Serial.println(__FILE__);
  //  Serial.print("HX711_LIB_VERSION: ");
  //  Serial.println(HX711_LIB_VERSION);
  //  Serial.println();

  scale.begin(dataPin, clockPin);

  //  TODO find a nice solution for this calibration..
  //  load cell factor 20 KG
  //  scale.set_scale(127.15);
  //  load cell factor 5 KG
  scale.set_scale(1);       // TODO you need to calibrate this yourself
  scale.set_offset(0);
  //  reset the scale to zero = 0
  scale.tare();
}


void loop()
{
  //  continuous scale 4x per second
  int f = scale.get_units();
  
  Serial.println(f);
}


//  -- END OF FILE --

