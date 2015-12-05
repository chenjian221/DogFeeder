#include "DogFeeder.h"

/* ======= Initialization ==========*/
 void setup() {
  request.hostname = "www.cs.codes";
  request.port = 80;

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.display();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Welcome\nPress * to continue");
  display.display();

  pinMode(D3, OUTPUT);// WHITE 4
  pinMode(D4, OUTPUT);// YELLOW 3
  pinMode(D5, OUTPUT);// BLACK 2
  pinMode(D6, OUTPUT);// RED 1

  rtc.begin(&UDPClient, "north-america.pool.ntp.org");
  rtc.setTimeZone(-5); // gmt offset

}


/* ======= Life Cycle ==========*/
void loop() {
  keypressed = keypad.getKey();
  switch(state)
  {
    case INIT :
      if(keypressed == '*')
        askForID();
      break;
    case LOGIN :
      loginToGetInfo();
      break;
    case WAIT:
      checkKeypadInput();
      checkTimer();
  }
}
 /*=============== Functions ================*/
void dispense(int size)
{
  if(size > 4) {
    dispense(4);
    dispense(size - 4);
  }
  digitalWrite(D6, size==1?LOW:HIGH);
  digitalWrite(D5, size==2?LOW:HIGH);
  digitalWrite(D4, size==3?LOW:HIGH);
  digitalWrite(D3, size==4?LOW:HIGH);
  delay(size*1650);
}

void askForID()
{
 display.clearDisplay();
 display.setCursor(0,20);
 display.print("Please enter pet id:");
 display.display();
 id = "";
 state = LOGIN;
}
void loginToGetInfo()
{
 if(keypressed == '#')
 {
   bool exist = false;
   request.path = "/dogfeeder_php/userinfo.php?f=getfood&id="+id;
   http.get(request, response, headers);
   display.clearDisplay();
   if(response.body == "false"){
     display.setCursor(0,0);
     display.print("Your id is invaild\n");
     askForID();
   } else {
     display.setCursor(0,20);
     char  s[100];
     response.body.toCharArray(s,100);
     JsonObject& root = jsonBuffer.parseObject(s);
     const char * f = root["freq"];
     const char * si = root["per"];
     freqency = atoi(f);
     foodSize = atoi(si);
     display.print("You pet are feeding "
       + String(freqency )+ " per date, with food size of "
       + String(foodSize) + " cup(s) per meal.");
     display.display();
     delay(1000);
     display.clearDisplay();
     display.setCursor(0,20);
     display.print("Initializing Dispensor...\nPlease Wait");
     dispense(3);
     state = WAIT;
     currentTime = rtc.now();
     calculateNextTime(currentTime);
   }
   display.display();
   // delay(1000);
 }
 else if(keypressed != NO_KEY)
 {
   id = String (id + String(keypressed));
   display.print('*');
   display.display();
 }
}
void checkKeypadInput()
{
 switch (keypressed)
 {
   case 'A':
     dispense(1);
     break;
   case 'B':
     dispense(2);
     break;
   case 'C':
     dispense(3);
     break;
   case 'D':
     dispense(4);
 }


}
void checkTimer()
{
  currentTime = rtc.now();
  display.setCursor(0,0);
  unsigned long timeLeft = currentTime - nextTime;

  if (timeLeft < 2) {
      calculateNextTime(currentTime);
      dispense(foodSize);
    }

    timeStr = "Current Time:\n";
    timeStr += rtc.hour12String(currentTime);
    timeStr += ":";
    timeStr += rtc.minuteString(currentTime);
    timeStr += ":";
    timeStr += rtc.secondString(currentTime);
    timeStr += " ";
    timeStr += rtc.AMPMString(currentTime);
    timeLeftStr = "Next Feeding Time:\n";
    timeLeftStr += rtc.hour12String(nextTime);
    timeLeftStr += ":";
    timeLeftStr += rtc.minuteString(nextTime);
    timeLeftStr += ":";
    timeLeftStr += rtc.secondString(nextTime);
    timeLeftStr += " ";
    timeLeftStr += rtc.AMPMString(nextTime);

    display.clearDisplay();
    display.println(timeStr);
    display.println(timeLeftStr);
    display.display();
  }
void calculateNextTime(uint32_t tnow){

  tnow += (3600UL * 5);
  if (tnow < 28800UL || tnow > 72000UL || freqency < 2) nextTime = 28800UL + 3600UL * 5;
  else nextTime = tnow + (43200UL) / (freqency - 1);
}
