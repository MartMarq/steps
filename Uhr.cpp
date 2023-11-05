#include "Uhr.h"


RTC_DATA_ATTR int Welche = 1;
RTC_DATA_ATTR uint16_t s7 = 7;
RTC_DATA_ATTR uint16_t s6 = 6;
RTC_DATA_ATTR uint16_t s5 = 5;
RTC_DATA_ATTR uint16_t s4 = 4;
RTC_DATA_ATTR uint16_t s3 = 3;
RTC_DATA_ATTR uint16_t s2 = 2;
RTC_DATA_ATTR uint16_t s1 = 1;

#define DARKMODE false

void Uhr::drawWatchFace() { 


    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

  // reset step counter at midnight
    if (currentTime.Hour == 00 && currentTime.Minute == 00) {

      s7=s6;
      s6=s5;
      s5=s4;
      s4=s3;
      s3=s2;
      s2=s1;

        sensor.resetStepCounter();
        //turn off radios
        WiFi.mode(WIFI_OFF);
        btStop();
    }

    //time adjustment start
    //if you don't need a time adjustment, just delete all lines between time adjustment start and end
    //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

    if (currentTime.Hour == 00 && currentTime.Minute == 30) {

        RTC.read(currentTime);
        int8_t sekunde = currentTime.Second;
        int8_t minute = currentTime.Minute;
        int8_t hour = currentTime.Hour;
        int8_t day = currentTime.Day;
        int8_t month = currentTime.Month;
        int8_t year = tmYearToY2k(currentTime.Year);

        delay(15500);

        tmElements_t tm;
        tm.Month = month;
        tm.Day = day;
        tm.Year = y2kYearToTm(year);
        tm.Hour = hour;
        tm.Minute = minute;
        tm.Second = sekunde;

        RTC.set(tm);
    }
    // time adjustment end

    s1 = sensor.getCounter();
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 58);
    float VBAT = getBatteryVoltage();
    uint32_t  Durch;
    Durch = (s7 + s6 + s5 + s4 + s3 + s2 + s1) / 7;
    
    if (Welche == 1) {
        
        uint8_t displayHour;
        displayHour = currentTime.Hour;
        if (displayHour < 10) {
            display.print("0");
        }
        display.print(displayHour);
        display.print(":");
        if (currentTime.Minute < 10) {
            display.print("0");
        }
        display.println(currentTime.Minute);


        display.setFont(&FreeSansBold15pt7b);

        int16_t  x1, y1;
        uint16_t w, h;

        String TagA = dayShortStr(currentTime.Wday);
        int b = currentTime.Day;
        String TagC = monthShortStr(currentTime.Month);
        String Datum = TagA + " " + "  " + " " + TagC;
        display.getTextBounds(Datum, 0, 0, &x1, &y1, &w, &h);
        display.setCursor(100 - (w / 2), 90);
        display.println(TagA + " " + b + " " + TagC);


        uint8_t Protz = min<int>(100 - ((4.20 - VBAT) * 100), 100);
        Protz = max<int>(Protz, 0);
        display.drawBitmap(40, 100, akku, 30, 20, GxEPD_BLACK);
        display.setCursor(80, 120);
        display.print(Protz);
        display.print(" %");
        display.drawBitmap(40, 130, steps, 19, 23, GxEPD_BLACK);
        display.setCursor(80, 150);
        display.println(s1);
        display.drawBitmap(40, 160, durch, 30, 32, GxEPD_BLACK);
        display.setCursor(80, 185);
        display.println(Durch);
    }
    else
    {
        display.setFont(&FreeSansBold15pt7b);
        
        display.setCursor(0, 25);
        display.print("today    ");
        display.println(s1);
        display.setCursor(20, 49);
        display.print("t - 1     ");
        display.println(s2);
        display.setCursor(20, 73);
        display.print("t - 2     ");
        display.println(s3);
        display.setCursor(20, 97);
        display.print("t - 3     ");
        display.println(s4);
        display.setCursor(20, 121);
        display.print("t - 4     "); 
        display.println(s5);
        display.setCursor(20, 145);
        display.print("t - 5     "); 
        display.println(s6);
        display.setCursor(20, 169);
        display.print("t - 6     ");
        display.println(s7);
        display.writeFastHLine(0, 175, 200, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        
        display.drawBitmap(30, 177, xdurch, 20, 21, GxEPD_BLACK);
        display.setCursor(90, 199);
        display.println(Durch);

    }
   
  
}


void Uhr::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
      }
   }


  if (IS_BTN_RIGHT_UP) {
    vibrate();
    Welche = Welche +1;
    if (Welche>2) {
      Welche =1;
    }
  RTC.read(currentTime);
    uint8_t stundeA =  ((currentTime.Hour + 11) % 12) + 1;
    int minuteA =  currentTime.Minute;
  drawWatchFace();
  showWatchFace(true);
  }
if (IS_BTN_RIGHT_DOWN) {
   }

  Watchy::handleButtonPress();

}

void Uhr::vibrate() {
  
  sensor.enableFeature(BMA423_WAKEUP, false);
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  
    delay(50);
    digitalWrite(VIB_MOTOR_PIN, true);
    delay(50);
    digitalWrite(VIB_MOTOR_PIN, false);
  
  sensor.enableFeature(BMA423_WAKEUP, true);
}



