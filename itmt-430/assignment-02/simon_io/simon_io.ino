 /*
 * Created by Myungji
 * This program demonstrates Simon Game
 * class : IT892 (IIT)
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "iPhone"
#define STAPSK  "03032626"
#endif


const int MAX_LEVEL = 10;  //you can set up
int sequence[MAX_LEVEL];
int sound[MAX_LEVEL];
int gamer_sequence[MAX_LEVEL];
int dingDongDang[4] = {523, 659, 783, 1046};

int level = 1;
int sounds = 0;
int velocity = 1000;

const int green_led = 13;
const int red_led = 12;
const int yellow_led = 14;
const int blue_led = 15;

const int green_btn = 2;
const int red_btn = 16;
const int yellow_btn = 5;
const int blue_btn = 4;

const int buzzer = 0;


/**********connecting webserver***********/

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);


//Set game start
void handleRoot() {
  char temp[800];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 800,

                    "<html>\
                      <head>\
                        <meta http-equiv='refresh' content='5'/>\
                        <title>SIMON GAME PAGE</title>\
                        <style>\
                          body { background-color: #FFFFFF; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; text-align:center; }\
                        </style>\
                      </head>\
                      <body>\
                        <h1>This is SIMON GAME!</h1>\
                        <p>If you want to \start, \press the yellow button</p>\
                        <h2>Your Score : %d</h2>\
                        <p>Play Time: %02d:%02d:%02d </p>\
                        <img src=\"/color.svg\" />\\
                      </body>\
                    </html>",
                      --level, hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
}


//fail to connect
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}


//when you pushed button, color image is changed
void change() {
  String out;
  out.reserve(2600);
  int btnState[4] = {1, 1, 1, 1};
  btnState[0] = digitalRead(green_btn);
  btnState[1] = digitalRead(red_btn);
  btnState[2] = digitalRead(yellow_btn);
  btnState[3] = digitalRead(blue_btn);
  //to check which button player push? red or blue or green or yellow
  if (btnState[0] == LOW) {
    out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n";
    out += "<circle cx=\"50\" cy=\"50\" r = \"50\" stroke = \"rgb(0, 128, 0)\" fill=\"rgb(0, 128, 0)\" />\n";
  }
  else if (btnState[1] == LOW) {
    out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n";
    out += "<circle cx=\"50\" cy=\"50\" r = \"50\" stroke = \"rgb(255, 0, 0)\" fill=\"rgb(255, 0, 0)\" />\n";
  }
  else if (btnState[2] == LOW) {
    out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n";
    out += "<circle cx=\"50\" cy=\"50\" r = \"50\" stroke = \"rgb(255, 255, 0)\" fill=\"rgb(255, 255, 0)\" />\n";
  }
  else if (btnState[3] == LOW) {
    out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n";
    out += "<circle cx=\"50\" cy=\"50\" r = \"50\" stroke = \"rgb(0, 0, 255)\" fill=\"rgb(0, 0, 255)\" />\n";
  }
  else {
    out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n";
    out += "<circle cx=\"50\" cy=\"50\" r = \"50\" stroke = \"rgb(255, 255, 255)\" fill=\"rgb(255, 255, 255)\" />\n";

  }
  out += "</svg>";
  server.send(200, "image/svg+xml", out);
}



/**********arduino setting***********/


void setup() {
  Serial.begin(19200);

  //Try to connect wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  //Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("SIMON GAME")) {
    Serial.println("game started");
  }

  //Connect to web server
  server.on("/", handleRoot);
  server.on("/color.svg", change);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  
  pinMode(green_led, OUTPUT); //green_led
  pinMode(red_led, OUTPUT); //red_led
  pinMode(yellow_led, OUTPUT); //yellow_led 
  pinMode(blue_led, OUTPUT); //blue_led 

  pinMode(green_btn, INPUT_PULLUP); //button 1, green_btn
  pinMode(red_btn, INPUT_PULLUP); //button 2, red_btn
  pinMode(yellow_btn, INPUT_PULLUP); //button 3, yellow_btn
  pinMode(blue_btn, INPUT_PULLUP); //button 4, blue_btn

  pinMode(buzzer, OUTPUT); //buzzer
}

//ready to play game
//when yellow button is pushed, Game Start
void loop() {

  //web server
  server.handleClient();
  MDNS.update();

  Serial.println("To start game, press the yellow button");

  //ready to start game
  if (level == 1) {
    set_sequence();
    
    for (int i = 15; i >= 12; i--) { //flashing leds
      digitalWrite(i, HIGH);
      delay(100);
      digitalWrite(i, LOW);
    }
  }

  //start game
  if (digitalRead(yellow_btn) == LOW || level != 1) { //start button == button 1
    Serial.print("Game Start");
    show_sequence();
    get_sequence();
  }

  //end game
  if (level == MAX_LEVEL) {
    Serial.print("Good job!!");
    for(int i = 0; i < 4; i++){
      digitalWrite(12+i, HIGH);
      tone(buzzer, dingDongDang[i]); 
      delay(100);
      noTone(buzzer);
    }
    level = 1;
  }
}

//make sequence for game
//create array of sequence
void set_sequence() {
  randomSeed(millis()); //true random

  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(12, 16);

    switch (sequence[i]) { //convert color to sound
      case blue_led:
        sounds = 349; //Fa
        break;
      case yellow_led:
        sounds = 329; //Mi
        break;
      case red_led:
        sounds = 293; //Re
        break;
      case green_led:
        sounds = 261; //Do
        break;
    }
    sound[i] = sounds;
  }
}

//show arrays of led and sound
void show_sequence() {
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(blue_led, LOW);

  for (int i = 0; i < level; i++) {
    digitalWrite(sequence[i], HIGH);
    tone(buzzer, sound[i]);
    delay(velocity);
    digitalWrite(sequence[i], LOW);
    noTone(buzzer);
    delay(200);
  }
}

//game
void get_sequence() {
  int flag = 0; //flag correct sequence

  for (int i = 0; i < level; i++) {
    flag = 0;

    while (flag == 0) {

      if (digitalRead(green_btn) == LOW) {
        digitalWrite(green_led, HIGH);
        tone(buzzer, 261); //Do
        delay(velocity);
        noTone(buzzer);
        gamer_sequence[i] = green_led;
        flag = 1;
        delay(200);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(green_led, LOW);
      }

      if (digitalRead(red_btn) == LOW) {
        digitalWrite(red_led, HIGH);
        tone(buzzer, 293); //Re
        delay(velocity);
        noTone(buzzer);
        gamer_sequence[i] = red_led;
        flag = 1;
        delay(200);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(red_led, LOW);
      }

      if (digitalRead(yellow_btn) == LOW) {
        digitalWrite(yellow_led, HIGH);
        tone(buzzer, 329); //Mi
        delay(velocity);
        noTone(buzzer);
        gamer_sequence[i] = yellow_led;
        flag = 1;
        delay(200);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(yellow_led, LOW);
      }

      if (digitalRead(blue_btn) == LOW) {
        digitalWrite(blue_led, HIGH);
        tone(buzzer, 349); //Fa
        delay(velocity);
        noTone(buzzer);
        gamer_sequence[i] = blue_led;
        flag = 1;
        delay(200);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(blue_led, LOW);
      }
    }
  }

  server.handleClient();
  MDNS.update();
  
  //correct!
  right_sequence();
}


//when user input correct buttons
//blink leds, level up and increase velocity
void right_sequence() {
  delay(100);

  if (level < MAX_LEVEL) {
    level++;
  }

  server.handleClient();
  MDNS.update();
  
  velocity -= 50; //increases difficulty
}


//When user input wrong button
//start begining
void wrong_sequence() {
  
  Serial.print("Wrong Input!");
  for (int i = 0; i < 2; i++) {
    digitalWrite(green_led, HIGH);
    digitalWrite(red_led, HIGH);
    digitalWrite(yellow_led, HIGH);
    digitalWrite(blue_led, HIGH);
    tone(buzzer, 233);
    delay(200);

    digitalWrite(green_led, LOW);
    digitalWrite(red_led, LOW);
    digitalWrite(yellow_led, LOW);
    digitalWrite(blue_led, LOW);
    noTone(buzzer);
    delay(200);
  }
  //Initialization
  level = 1;
  velocity = 1000;
}
