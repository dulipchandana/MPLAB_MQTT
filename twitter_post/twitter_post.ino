#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ArduinoJson.h>                  // https://github.com/bblanchon/ArduinoJson
//#include "secret.h"                       // uncomment if using secret.h file with credentials
//#define TWI_TIMEOUT 3000                  // varies depending on network speed (msec), needs to be before TwitterWebAPI.h
#include <TwitterWebAPI.h>
#include "Ultrasonic.h"

Ultrasonic ultrasonic( 12, 14 );

#ifndef WIFICONFIG
const char* ssid = "AndroidAP-dulip";           // WiFi SSID //SLT-ADSL-69008 ,SLT-LTE-WiFi-9201
const char* password = "dul1985cha22";   // WiFi Password //Q3F2B14H1H0 , 5012@slt
#endif

std::string search_str = "#dog";          // Default search word for twitter
std::string twitter_post_msg = "Meeting room 001 avilable";
std::string twitter_post_msg2 = "Meeting room 002 occupied";
const char *ntp_server = "pool.ntp.org";  // time1.google.com, time.nist.gov, pool.ntp.org
int timezone = 5.5;                        // US Eastern timezone -05:00 HRS
unsigned long twi_update_interval = 20;   // (seconds) minimum 5s (180 API calls/15 min). Any value less than 5 is ignored!

#ifndef TWITTERINFO  // Obtain these by creating an app @ https://apps.twitter.com/
  static char const consumer_key[]    = "xM6dp9hXguV8hL4pDtU4CXXyO";
  static char const consumer_sec[]    = "AJPXHxrjo44uZNnQnnoafQ6mPKQCMvDTp77aRccLX0B0AELD3Z";
  static char const accesstoken[]     = "1014084029627289602-smDsmtDqBWYZxO3EsRGzLXZcOYVl6S";
  static char const accesstoken_sec[] = "ivjeZzoAnYlqlKSdEVcbNfb0oF1BBY2EGoTXd63ZXNfRq";
#endif

unsigned long api_mtbs = twi_update_interval * 1000; //mean time between api requests
unsigned long api_lasttime = 0; 
bool twit_update = true;
bool status_availabal = false;
bool status_occupied = false;
std::string search_msg = "No Message Yet!";

//led control
const int plus = 5;  //D1
const int neg = 16;  //D0

//distance ultrasonic
int distans;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, timezone*3600, 60000);  // NTP server pool, offset (in seconds), update interval (in milliseconds)
TwitterClient tcr(timeClient, consumer_key, consumer_sec, accesstoken, accesstoken_sec);


void setup() {

  //led
  pinMode(plus, OUTPUT); // Sets the trigPin as an Output
  pinMode(neg, OUTPUT);
  digitalWrite(neg, LOW);

  // initialize digital pin 13 as an output.
  Serial.begin(9600);                  
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to ");
  Serial.print(ssid);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(plus, HIGH);
    delay(500);
    digitalWrite(plus, LOW);
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected. yay!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
  // Connect to NTP and force-update time
  tcr.startNTP();
  Serial.println("NTP Synced");
  delay(100);
  if (twi_update_interval < 5) api_mtbs = 5000; // Cant update faster than 5s.
}

// the loop function runs over and over again forever
void loop() {

 distans = ultrasonic.read(CM);
 Serial.print( distans);
 Serial.println( "cm" );
 if(distans < 100){
  digitalWrite(plus, HIGH);
  delay(125);
  digitalWrite(plus, LOW);
  delay(375);
 }else {
  delay(500);
 }
   
 delay(500);

  if (millis() > api_lasttime + api_mtbs)  {
    char snum[2];
    if(distans > 100 & !status_availabal){
    tcr.tweet(twitter_post_msg+"-"+itoa(millis(), snum, 10));
    digitalWrite(plus, HIGH);
    delay(500);
    digitalWrite(plus, LOW);
    status_availabal = true;
    status_occupied = false;
    Serial.print("Post: avilable " );
    }else if (distans < 100 & !status_occupied){
    status_occupied = true;
    status_availabal = false; 
    tcr.tweet(twitter_post_msg2+"-"+itoa(millis(), snum, 10));
    digitalWrite(plus, HIGH);
    delay(500);
    digitalWrite(plus, LOW);
    Serial.print("Post: occupied " );
    }    
    api_lasttime = millis();
  }
  delay(2);
  yield();
  
}
