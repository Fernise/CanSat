//////////////////////////////////////////////
//       Arduino Color Sensor Tutorial      //
//          with ST7735 TFT LCD             //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

#include <MD_TCS230.h>
#include <FreqCount.h>


// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define GREY     0xC618

// Pin definitions
#define  S2_OUT  2
#define  S3_OUT  3
#define  OE_OUT   6    // LOW = ENABLED 
#define S0 4
#define S1 5

int R = 0;
int G = 0;
int B = 0;

MD_TCS230 CS(S2_OUT, S3_OUT, OE_OUT);

int state = 0;


void setup() {

  Serial.begin(57600);

}

void loop() {
  readSensor();

}


void readSensor()
{
  colorData  rgb;

  Serial.print("RGB [");
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(",");
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(",");
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.println("]");

  R = rgb.value[TCS230_RGB_R];
  G = rgb.value[TCS230_RGB_G];
  B = rgb.value[TCS230_RGB_B];

Serial.print(R);
  delay(100);
}
