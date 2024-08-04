#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>

#define NTP_OFFSET  28800 // In seconds 
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "1.asia.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    4
#define OLED_CS    5
#define OLED_RESET 2

const int buttonPin = 4;
int buttonState = 0;
int State=0;
String weekDays[7]={"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void showTime(){
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime();
    String formattedDate = timeClient.getFormattedDate();
    String day = weekDays[timeClient.getDay()];
    int splitT = formattedDate.indexOf("T");
    String dateReal = formattedDate.substring(0, splitT);
    
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(dateReal);
    display.setCursor(100, 0);
    display.println(day);
    display.setTextSize(2);
    display.setCursor(15, 15);
    display.println(formattedTime);
    display.display();
}

void bars(){
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    for(int i=2; i<128; i+=6){
        display.fillRect(i, 4, 4, 58, WHITE);
    }

    for(int i=122; i>1; i-=6){
        display.fillRect(i, 4, 4, 58, BLACK);
        display.display();
        delay(470);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(buttonPin, INPUT);
    WiFi.begin("Wokwi-GUEST", "", 6);

  while (WiFi.status() != WL_CONNECTED) {
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Connecting to WiFi");
    display.display();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setTextColor(WHITE);

}

void loop() {
    while(State==0){
        buttonState = digitalRead(buttonPin);
        showTime();
        if (buttonState == 1){
            display.clearDisplay();
            State=1;
            break;
        }
    }

    bars();
    State=0;
    
}
