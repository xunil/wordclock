#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <FastLED.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include "TeeSerial.h"
#include "WordClock.h"

// Allows streaming output (<< syntax)
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

char ssid[64];
char password[64];

#define NUM_LEDS 71
#define DATA_PIN 12
CRGB leds[NUM_LEDS];

ESP8266WebServer server(80);

void handleRoot() {
  String ntpDateTimeStr = NTP.getTimeDateString();

  String message = "<html>\n<head>\n\t<title>Word Clock</title>\n</head>\n<body>\n";
  message += "<h1>";
  message += "(device description not implemented)";
  //message += config.deviceDescription;
  message += "</h1>\n";
  message += "<h2>Status</h2>\n";
  message += "NTP date and time ";
  message += ntpDateTimeStr;
  message += "<br/>\n";
  message += "Free heap: ";
  message += ESP.getFreeHeap();
  message += " bytes<br/>\n";
  if (WiFi.status() == WL_CONNECTED) {
    message += "Connected to WiFi network ";
    message += WiFi.SSID();
    message += "<br/>\n";
  }

  String debugLog;
  TeeSerial0.getBuffer(debugLog);
  message += "<h4>Debug Log</h4>\n";
  message += "<pre>\n";
  message += debugLog;
  message += "</pre>\n";

  // message += "<form action=\"/config\" method=\"get\"><input type=\"submit\" value=\"Configure\"/></form>\n";
  message += "<form action=\"/time\" method=\"get\"><input type=\"submit\" value=\"Manage Time\"/></form>\n";
  message += "<form action=\"/reset\" method=\"get\"><input type=\"submit\" value=\"Reset\"/></form>\n";
  message += "</body>\n</html>\n";
  server.send(200, "text/html", message);
}

void handleReset() {
  String message = "<html>\n<head>\n\t<title>Reset</title>\n</head>\n<body>\n";
  message += "<h1>Resetting!</h1>";
  message += "</body>\n</html>\n";
  
  server.sendHeader("Refresh", "20; url=/");
  server.send(200, "text/html", message);
  delay(2000);
  ESP.restart();
}

void handleTime() {
  String message = "<html>\n<head>\n\t<title>Manage Time Settings</title>\n</head>\n<body>\n";
  message += "<h1>Time</h1>\n";
  if (server.method() == HTTP_POST) {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "syncNTP") {
        syncNTPTime();
        message += "<h2>Synced NTP time</h2>\n";
      }
    }
  }

  String ntpDateTimeStr = NTP.getTimeDateString();

  message += "NTP date and time ";
  message += ntpDateTimeStr;
  message += "<br/>\n";
  message += "<form action=\"/time\" method=\"post\">\n";
  message += "<input type=\"submit\" value=\"Sync with NTP\"/>\n";
  message += "<input type=\"hidden\" name=\"syncNTP\" value=\"true\"/>\n";
  message += "</form>\n";
  message += "<form action=\"/\" method=\"post\"><input type=\"submit\" value=\"Home\"/></form>\n";
  message += "</body>\n</html>\n";

  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// TODO: Implement checking for birthday
bool isDeniseBirthday() {
  return false;
}
void illuminate_word(int word, int color) {
  for (int i = 0; i < 4; i++) {
    int pixel = WORDMAP[word][i];
    if (pixel == -1)
      // -1 indicates end of the LED list for this word
      break;
    leds[pixel] = color;
  }
}

// :00 - 04 30..26    O CLOCK
// :05 - 09 25..21    FIVE PAST
// :10 - 14 20..16    TEN PAST
// :15 - 19           QUARTER PAST
// :20 - 24           TWENTY PAST
// :25 - 29           TWENTY FIVE PAST
// :30 - 34           HALF PAST
// :35 - 39           TWENTY FIVE TO
// :40 - 44           TWENTY TO
// :45 - 49           QUARTER TO
// :50 - 54           TEN TO
// :55 - 59           FIVE TO

// retrieve current date and time from RTC (day, month, year, hour, minute, second)
// if it is Denise's birthday && hour == BDAY_MSG_HOUR && minute == BDAY_MSG_MIN && second >= 0 && second <= 15
void foo(void) {
  CRGB currentColor = CRGB::White;
  time_t t = NTP.getTime();
  if (isDeniseBirthday()) {
     for (int i = 0; i < BIRTHDAY_MSG_LEN; i++) {
       illuminate_word(BIRTHDAY_MSG[i], currentColor);
     }
  } else {
    illuminate_word(WORD_IT, currentColor);
    illuminate_word(WORD_IS_1, currentColor);
    if (minute(t) >= 0 && minute(t) <= 4) {
      illuminate_word(WORD_O, currentColor);
      illuminate_word(WORD_CLOCK, currentColor);
    } else {
      if (minute(t) >= 5 && minute(t) <= 34) {
        illuminate_word(WORD_PAST, currentColor);
      } else {
        illuminate_word(WORD_TO, currentColor);
      }
      switch (minute(t) % 30) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
          illuminate_word(WORD_HA, currentColor);
          illuminate_word(WORD_LF, currentColor);
          break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
          illuminate_word(WORD_TWENTY, currentColor);
          illuminate_word(WORD_FIVE_MIN, currentColor);
          break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
          break;
      }
    }
  }
}

void connectToWiFi() {
  // TODO: Config parsing
  //config.wiFiSSID.toCharArray(ssid, 64);
  //config.wiFiPassword.toCharArray(password, 64);
  if (config.wiFiPassword.length() > 0) {
    WiFi.begin(ssid, password);
  } else {
    WiFi.begin(ssid);
  }

  if (config.wiFiMode == WIFI_STA) {
    // Wait up to 1 minute for connection
    uint8_t attempts = 0; 
    while (attempts <= 120 && WiFi.status() != WL_CONNECTED) {
      delay(500);
      TeeSerial0 << ".";
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      TeeSerial0 << "\n";
      TeeSerial0 << "Connected to " << config.wiFiSSID << "\n";
      TeeSerial0 << "IP address: " << WiFi.localIP() << "\n";
    } else {
      TeeSerial0 << "\n";
      TeeSerial0 << "Unable to connect to " << config.wiFiSSID << " network.  Giving up.\n";
    }
  } else if (config.wiFiMode == WIFI_AP) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    TeeSerial0 << "Starting WiFi network named " << config.wiFiSSID << "...";
    TeeSerial0.flush();
    if (config.wiFiPassword.length() > 0) {
      WiFi.softAP(ssid, password);
    } else {
      WiFi.softAP(ssid);
    }

    TeeSerial0 << "done.\n";
    TeeSerial0 << "My IP address is " << WiFi.softAPIP() << "\n";
  }
}

void syncNTPTime() {
  TeeSerial0 << "Resyncing NTP time\n";
  NTP.stop();
  setupNTP();
}

void setupNTP() {
  NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
    if (error) {
      TeeSerial0 << "Time Sync error: ";
      if (error == noResponse)
        TeeSerial0 << "NTP server not reachable\n";
      else if (error == invalidAddress)
        TeeSerial0 << "Invalid NTP server address\n";
    } else {
      TeeSerial0 << "Got NTP time: ";
      TeeSerial0 << NTP.getTimeDateString(NTP.getLastNTPSync()) << "\n";
    }
  });
  NTP.begin("time.nist.gov", -7, true);
  NTP.setInterval(1800);
}

void startWebServer() {
  // Set up handlers for different pages
  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.on("/time", handleTime);
  server.onNotFound(handleNotFound);

  // Start the web server
  server.begin();
  TeeSerial0 << "HTTP server started\n";
}

void basicSetup(void) {
  // Nothing to do here yet
  yield();
}

void ledsSetup(void) {
  pinMode(DATA_PIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void setup(void) {
  basicSetup();
  ledsSetup();
  connectToWiFi();
  startWebServer();
}

void loop(void) {
  String nowString;

  // Probably unnecessary, but doesn't hurt anything.
  yield();

  server.handleClient();
}

