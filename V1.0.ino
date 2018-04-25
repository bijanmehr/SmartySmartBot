#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
 
// Initialize Wifi connection to the router
char ssid[] = "ssid";
char password[] = "pass";
 
// Initialize Telegram BOT
#define BOTtoken "373621299:AAG4VF5QsJsHSWFRf_mIshowOZveInWX04E"
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
int Bot_mtbs = 100; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
 
const int ledPin = D0;
int ledStatus = 0;
const int ledPinRED = D7;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
 
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 
    if (text == "/ledon") {
      digitalWrite(ledPin, HIGH);   // turn the LED on
      digitalWrite(ledPinRED, LOW);
      ledStatus = 1;
      bot.sendMessage(chat_id, "Led is ON", "");
       
    }
 
    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(ledPin, LOW);    // turn the LED off
      digitalWrite(ledPinRED, HIGH);
      bot.sendMessage(chat_id, "Led is OFF", "");
      
    }
 
    if (text == "/status") {
      if(ledStatus){
        bot.sendMessage(chat_id, "Led is ON", "");
      } else {
        bot.sendMessage(chat_id, "Led is OFF", "");
      }
    }
 
    if (text == "/start") {
      String welcome = "به برنامه ارتباط اینترنت اشیاء با ربات تلگرام خوش آمدید" + from_name + ".\n";
      welcome += "برنامه نمونه برای مدیریت ال ای دی.\n\n";
      welcome += "/ledon : جهت روشن کردن ال ای دی از این دستور استفاده کنید.\n";
      welcome += "/ledoff : جهت خاموش کردن ال ای دی از این دستور استفاده کنید.\n";
      welcome += "/status : چنانچه نمیدانید ال ای دی در چه وضعیتی است از این دستور استفاده کنید.\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
 
 
void setup() {
 
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, LOW); // initialize pin as off

  pinMode(ledPinRED, OUTPUT);
  delay(10);
  digitalWrite(ledPinRED, HIGH); 
}
 
void loop() {

  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
 
    Bot_lasttime = millis();
  }
}
