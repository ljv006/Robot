#include <LiquidCrystal.h>
#include<stdio.h>
#include<stdlib.h>

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
/*int msgs[5] = {"Right Key OK ",
                    "Up Key OK    ",               
                    "Down Key OK  ",
                    "Left Key OK  ",
                    "Select Key OK" };
*/
int adc_key_val[5] ={50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;
int score = 0;
//你的位置
int eaterX = -1;
int eaterY = -1;
//飞机的位置
int posX = -1;
int posY = -1;
//子弹的位置
int bulX = -1;
int bulY = -1;
//是否需要增加飞机
bool flagPlane = true;
bool flagBullet = true;
const long interval = 500;   
unsigned long previousMillis = 0;  

void gameOver() {
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("The Game Is Over");
}
void fire() {
      bulX = eaterX + 1;
      bulY = eaterY;
      lcd.setCursor(bulX, bulY);
      lcd.print(">");
      flagBullet = false;
}
void upbut() {
   lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 0);
    lcd.print("@");
    eaterX = 0;
    eaterY = 0;
}
void downbut() {
   lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("@");
    eaterX = 0;
    eaterY = 1;
}
void leftbut() {
  lcd.setCursor(0, 1);
  lcd.print("*START");
  lcd.setCursor(9, 1);
  lcd.print("oSCORE");
}
void rightbut() {
  lcd.setCursor(0, 1);
  lcd.print("oSTART");
  lcd.setCursor(9, 1);
  lcd.print("*SCORE");
}
void delay() {
   while (true) {
                unsigned long currentMillis = millis();
                if(currentMillis - previousMillis >= interval) {
                      return;
               }
                adc_key_in = analogRead(0);    // read the value from the sensor 
                      key = get_key(adc_key_in);    // convert into key press
                         if (key == 1)
                            upbut();
                         if (key == 2)
                            downbut();
                         if (key == 4 && flagBullet)
                            fire();
        }
}
void startGame() {
    //start the game
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("@");
    eaterX = 0;
    eaterY = 1;
    while (true) {
      //是否需要增加飞机
      if (flagPlane) {
        posY = rand()% 1;
        posX = 15;
        lcd.setCursor(posX, posY);
        lcd.print("*");
        flagPlane = false;
       } else {
        lcd.setCursor(posX, posY);
        lcd.print(" ");
        posX--;
        if (!flagBullet) {
        lcd.setCursor(bulX, bulY);
        lcd.print(" ");
        bulX++;
        }
        if (bulX == posX && bulY == posY) {
          flagPlane = true;
          score++;
          bulX = posX = bulY = posY = -1;
          flagBullet = true;
          continue;
        }
        if (posX == 0) {
          gameOver();
          break;
        }
        lcd.setCursor(posX, posY);
        lcd.print("*");
         delay();
        if (!flagBullet) {
          lcd.setCursor(bulX,bulY);
          lcd.print(">");
          delay();
         }
      }
      adc_key_in = analogRead(0);    // read the value from the sensor 
                      key = get_key(adc_key_in);    // convert into key press
                         if (key == 1)
                            upbut();
                         if (key == 2)
                            downbut();
                         if (key == 4 && flagBullet)
                            fire();
    }
    
}
void showScore() {
  //show the score
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("The score is:");
    lcd.setCursor(7,1);
    lcd.print(score);
}

void setup()
{
  lcd.clear(); 
  lcd.begin(16, 2);
  lcd.setCursor(3,0); 
  lcd.print("Plane Fighter!"); 
  lcd.setCursor(0, 1);
  lcd.print("oSTART");
  lcd.setCursor(9, 1);
  lcd.print("oSCORE");
}

void loop()
{
  adc_key_in = analogRead(0);    // read the value from the sensor 
  key = get_key(adc_key_in);  // convert into key press
 
  if (key != oldkey)   // if keypress is detected
    {
     delay(50);  // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor 
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey)    
    {   
         if (key == 0) {
            rightbut();
            delay(500);
            showScore();
         }
         if (key == 3) {
            leftbut();
            delay(500);
            startGame();
         }  
          oldkey = key;
    }
    delay(100);
  }
}
// Convert ADC value to key number
int get_key(unsigned int input)
{
    int k;
   
    for (k = 0; k < NUM_KEYS; k++)
    {
      if (input < adc_key_val[k])
      {
            return k;
        }
   }
   
    if (k >= NUM_KEYS)k = -1;  // No valid key pressed
    return k;
}
