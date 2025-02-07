/*
  File: projekt_prog.cpp
  Author: Ruben Sundell Boström
  Date: 2025-02-7
  Description: ett spel som har återskapat "pong", det använder två knappar och en oled skärm med u8g library
*/

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
int xpos = 63; //gör variabler för x position för bollen i början
int ypos = 31; //samma som xpos fast för y
int radie = 5; //sätter bollens radie
float xspeed = 2; //sätter hastigheten i början
float yspeed = -2; //-||-
float speedchange = 0.04; //variabel för hur mycket hastigheten ökar
int right_slider = 31; //positioner som racken börjar med
int left_slider = 31;



void setup() {
  pinMode(2, INPUT_PULLUP); //sätter input för en av knapparna med input pullup metoden
  pinMode(3, INPUT_PULLUP); //-||-
  Serial.begin(9600); //serial monitor setup för felsökning
}



void loop() { //loopen där koden körs

  sliders_movement(); //ändrar positionen för racken

  update_position(); //ändrar bollens position

  check_collision(); //kollar bollens kollision med racken och kanterna

  u8g.firstPage(); //här ritas allt på skärmen
  do {
    sliders(); //ritar sliders
    circle(xpos, ypos); //ritar bollen med xpos och ypos som inputs
  }

  while (u8g.nextPage());
  //delay(20); //använde detta delay för att ha en jämn uppdateringsfrekvens
}

void circle(int ball_x, int ball_y) {
  u8g.drawFilledEllipse(ball_x, ball_y, radie, radie); //ritar en boll med variabeln radie för både x och y
}

/*
  flyttar "sliders" med hjälp av knapparna (digitalRead)
  Parameters: inga
  Returns: void
*/

void sliders_movement() {

  if (digitalRead(2) == LOW) {
    right_slider -= 4;
  } else {
    right_slider += 4;
  }
  if (digitalRead(3) == LOW) {
    left_slider -= 4;
  } else {
    left_slider += 4;
  }

  if (right_slider > 63 - 20) {
    right_slider = 63 - 20;
  }

  if (left_slider > 63 - 20) {
    left_slider = 63 - 20;
  }

  if (right_slider < 0) {
    right_slider = 0;
  }

  if (left_slider < 0) {
    left_slider = 0;
  }
}

/*
  ritar sliders på oled skärmen
  Parameters: inga
  Returns: void
*/

void sliders() {
  u8g.drawBox(5, left_slider, 3, 20);

  u8g.drawBox(127 - 5, right_slider, 3, 20);
}

/*
  flyttar positionen av bollen med speedchange och xspeed och yspeed
  Parameters: inga
  Returns: void
*/

void update_position() {
  if (xspeed <= 0) {
    xspeed -= speedchange;
  } else {
    xspeed += speedchange;
  }

  if (yspeed <= 0) {
    yspeed -= speedchange;
  } else {
    yspeed += speedchange;
  }

  xpos += xspeed;
  ypos += yspeed;
}

/*
  startar om positionen och hastigheten av bollen ifall den vänstra spelaren vann
  Parameters: inga
  Returns: void
*/

void left_score() {
  xpos = 63;
  ypos = 31;
  xspeed = -2;
  yspeed = 2;
  u8g.write("Left Player Scored");
}

/*
  startar om positionen och hastigheten av bollen ifall den högra spelaren vann
  Parameters: inga
  Returns: void
*/

void right_score() {
  xpos = 63;
  ypos = 31;
  xspeed = 2;
  yspeed = 2;
  u8g.write("Right Player Scored");
}

/*
  kollar alla kollisioner mellan bollar och sliders och kanterna till skärmen
  Parameters: inga
  Returns: void
*/

void check_collision() {
  if (xpos - radie < 0) {
    right_score();
  }

  if (xpos + radie > 127) {
    left_score();
  }
  
  if (xpos - radie <= 8  && (ypos > left_slider && ypos < left_slider + 20)) {
    xpos = 8 + radie;
    xspeed *= -1;
  }

  if (xpos + radie >= 119 && (ypos > right_slider && ypos < right_slider + 20)) {
    xpos = 119 - radie;
    xspeed *= -1;
  }

  if (ypos - radie <= 0) {
    ypos = 5;
    yspeed *= -1;
  }

  if (ypos + radie >= 63) {
    ypos = 58;
    yspeed *= -1;
  }
}