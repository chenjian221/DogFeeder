
/* ============== OLED =====================*/
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
 //Use I2C with OLED RESET pin on D4
 #define OLED_RESET D4
 Adafruit_SSD1306 display(OLED_RESET);


 /*=============== Keypad================*/

  #include "Keypad.h"

  const byte ROWS = 4; //four rows
  const byte COLS = 4; //four columns
  char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
  };
  byte rowPins[ROWS] = {A0, D2, A2, A3}; //connect to the row pinouts of the keypad
  byte colPins[COLS] = {A4, A5, A6, A7}; //connect to the column pinouts of the keypad

  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 /*=============== HTTP Client================*/
#include "HttpClient.h"
HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;
 /*=============== JSON ================*/
#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;

 /*=============== Variables ================*/
enum States {INIT = 0,LOGIN,WAIT};
static States state = INIT;
static char keypressed;
static String id = "";
static long freqency;
static long foodSize;


 /*=============== Time ================*/
 #include "SparkTime.h"
UDP UDPClient;
SparkTime rtc;

static unsigned long currentTime;
static unsigned long nextTime;
String timeStr;
String timeLeftStr;
