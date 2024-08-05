#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <ESP32Time.h>

#define NTP_OFFSET  28800 // In seconds 
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "1.asia.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
ESP32Time rtc(0);
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    4
#define OLED_CS    5
#define OLED_RESET 2

const int buttonPin = 4;
int buttonState = 0;
int State=0;

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setTimeData(){
    timeClient.update();
    String formatteddate = timeClient.getFormattedDate();
    int year = (formatteddate.substring(0,4)).toInt();
    int month = (formatteddate.substring(5,7)).toInt();
    int day = (formatteddate.substring(8,10)).toInt();
    Serial.print(year);
    Serial.print(month);
    Serial.print(day);

    int hour = timeClient.getHours();
    int mins = timeClient.getMinutes();
    int secs = timeClient.getSeconds();
    rtc.setTime(secs, mins, hour, day, month, year);
}

void showTime(){
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    
    display.setTextSize(2);
    display.setCursor(10, 15);
    display.println(rtc.getTime("%I:%M:%S"));
    display.setTextSize(1);
    display.setCursor(110, 20);
    display.println(rtc.getTime("%p"));
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
    // display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    // display.clearDisplay();
    // display.setTextColor(WHITE);
    // display.setTextSize(1);
    // display.setCursor(0, 0);
    // display.println("Connected");
    // display.display();

    setTimeData();
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));

    
    
}

void loop() {
    showTime();
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
}