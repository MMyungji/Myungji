/*
 * Created by Myungji
 * This program demonstrates Simon Game
 * class : IT892 (IIT)
 */

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


void setup() {
  Serial.begin(9600);
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
      delay(1000);
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
  
  //correct!
  right_sequence();
}


//when user input correct buttons
//blink leds, level up and increase velocity
void right_sequence() {
  delay(500);

  if (level < MAX_LEVEL) {
    level++;
  }
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
