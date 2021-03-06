#include <LOLIN_HP303B.h>
#include <stdio.h> 

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS 14         //for D32 Pro
#define TFT_DC 27         //for D32 Pro
#define TFT_RST 33        //for D32 Pro
#define TS_CS  12         //for D32 Pro

// HP303B Opject
LOLIN_HP303B HP303BPressureSensor;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TS_CS);

int32_t pressure;
int32_t temperature;
int16_t oversampling = 7;
int16_t ret;
float inHg;
float v_alt_corr;
float v_alt;                     
float mmhg_conversion= 3386.388140071641;            // convert hPa to inches Hg
unsigned long entry;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  //Call begin to initialize HP303BPressureSensor
  //The default address is 0x77 and does not need to be given.
  HP303BPressureSensor.begin();
  Serial.println("Sensor initialized complete!");

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  
  ts.begin();
  
  v_alt_corr = 0; 
}

void loop()
{
  // Höhenkorrektur kann auf den Touchscreen angepasst werden 
  // links - 10 Meter     rechts + 10 Meter
   if (ts.touched()) {
        TS_Point p = ts.getPoint();
        if (p.x < 1800) 
          {
            v_alt_corr = v_alt_corr + 10;
            }
        else
          {
            v_alt_corr = v_alt_corr - 10;
          }
   }
// Perform single measurement with optional parameter oversampling (avg of 2^x measurements) for one result with
// higher the precision
//   ret = HP303BPressureSensor.measureBothOnce(temperature, oversampling, pressure, oversampling);     
   ret = HP303BPressureSensor.measureTempOnce(temperature, oversampling);     // in Celsius
   ret = HP303BPressureSensor.measurePressureOnce(pressure, oversampling);    // in Pascal e.g 101325
   if (ret != 0)
   {
      //Something went wrong.
      Serial.print("FAIL! ret = ");
      Serial.println(ret);
   }
   else
   {
      Serial.print("perssure als Messwert");
      Serial.println(pressure);
      
      inHg = float (pressure * 10.0 / mmhg_conversion);
      v_alt = float ((101300 - pressure) * 0.08 + v_alt_corr ); // 8 Meter pro hPa
      Serial.print("Meter als Messwert");
      Serial.println(v_alt);

      display();
   }
}

void display( void ) {
  long       M;
  int        temp1, temp2;                                        // working vars for type conversion

  M = pressure / 100L;                                    // scale to hPa
  tft.setTextSize(3);                                     // each Character has 15 Pix
  tft.setTextColor( ILI9341_YELLOW, ILI9341_BLACK );  
  tft.setCursor(0, 1); tft.print("Pres: ");
  tft.setCursor(135, 1); tft.print(M);   
  tft.setCursor(255, 1); tft.print("hPa");

  tft.setCursor(115, 35); 
  temp1 = inHg / 10;                                      // Integer component
  tft.print(temp1); tft.print(".");                       // decimal place
  temp2 = ((inHg / 10) - (float) temp1 ) * 100;           // 2 decimal places
  tft.print(temp2);                                 
  tft.setCursor(235, 35); tft.print("inHg");

  tft.setCursor(0, 70); tft.print("Temp: ");
  tft.setCursor(170, 70); tft.print(temperature);  
  tft.drawCircle(275, 70, 3, ILI9341_YELLOW);
  tft.setCursor(285, 70); tft.print("C");
  
  tft.setCursor(0, 105); tft.print("Alt : ");       
  tft.setCursor(115, 105);
  temp1 = v_alt;                                        // Integer component
  tft.print(temp1); tft.print(".");                       // decimal place
  temp2 = (v_alt - (float) temp1 ) * 100;               // 2 decimal places
  tft.print(temp2);                                       // print it as an integer
  tft.setCursor(285, 105); tft.print("M"); 

  //tft.setCursor(0, 105); tft.print("Alt : ");       
  tft.setCursor(150, 140);
  temp1 = v_alt * 3.3;                                  // Feet no decimals
  tft.print(temp1); tft.print("   ");                     // 
  tft.setCursor(270, 140); tft.print("ft"); 
}
