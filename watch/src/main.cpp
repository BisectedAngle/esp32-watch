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

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void showTime(){
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime();
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String dateReal = formattedDate.substring(0, splitT);
    
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(dateReal);
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.println(formattedTime);
    display.display();
}

void setup() {
  pinMode(buttonPin, INPUT);
  WiFi.begin("Wokwi-GUEST", "", 6);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setTextColor(WHITE);

}

void loop() {
    showTime();
    delay(10);
    delay(100);
}