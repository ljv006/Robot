/****************************************************************/
/*本游戏只允许屏幕上有一架飞机且只能发射一颗子弹，增加游戏的难度*/
/*                   author：Jerry                              */
/****************************************************************/
#include <LiquidCrystal.h>
#include<stdio.h>
#include<stdlib.h>

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
//记录按键对应的值
int adc_key_val[5] ={50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
//记录当前的分数
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
//是否已经发射了子弹
bool flagBullet = true;
//进入游戏标记
bool gaming = false;
//游戏结束标记
bool gameover = false;
//显示得分标记
bool scoring = false;
//子弹发射前的间隔和发射后的间隔
long interval = 250;
long interval1 = 125;
//记录最高分
int _max = 0;
unsigned long previousMillis = 0;  
//游戏中的延时函数
void delayGame(int temp = interval) {
  previousMillis = millis();
  unsigned long currentMillis = millis();
   while (currentMillis - previousMillis <= temp) {
                adc_key_in = analogRead(0);    // read the value from the sensor 
                      key = get_key(adc_key_in);    // convert into key press
                         if (key == 1)
                            upbut();
                         if (key == 2)
                            downbut();
                         if (key == 4 && flagBullet)
                            fire();
              currentMillis = millis();
        }
}

void fire() {
      bulX = eaterX + 1;
      bulY = eaterY;
      lcd.setCursor(bulX, bulY);
      lcd.print(">");
      flagBullet = false;
}
//游戏中的上移函数
void upbut() {
   lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 0);
    lcd.print("@");
    eaterX = 0;
    eaterY = 0;
}
//游戏中的下移函数
void downbut() {
   lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("@");
    eaterX = 0;
    eaterY = 1;
}
//开始菜单的左移函数
void leftbut() {
  lcd.setCursor(0, 1);
  lcd.print("*START");
  lcd.setCursor(9, 1);
  lcd.print("oSCORE");
}
//开始菜单的右移函数
void rightbut() {
  lcd.setCursor(0, 1);
  lcd.print("oSTART");
  lcd.setCursor(9, 1);
  lcd.print("*SCORE");
}
//游戏外的延时函数
void _delay() {
   previousMillis = millis();
  unsigned long currentMillis = millis();
   while (currentMillis - previousMillis <= interval) {
              currentMillis = millis();
        }
}
void setup()
{
  lcd.clear(); 
  lcd.begin(16, 2);
  lcd.setCursor(2,0); 
  lcd.print("Plane Fighter!"); 
  lcd.setCursor(0, 1);
  lcd.print("oSTART");
  lcd.setCursor(9, 1);
  lcd.print("oSCORE");
}

void loop()
{
  //increasing the difficulty
    if (score > 10 && score < 20) {
      interval = 200;
      interval1 = 100;
    } 
    if (score > 20 && score < 30) {
      interval = 150;
      interval1 = 75;
    }
    if (score > 30 && score < 40) {
      interval = 100;
      interval1 = 50;
    }
    if (score > 40 && score < 50) {
      interval = 50;
      interval1 = 25;
    }
    //游戏结束分支
    if (gameover) {
        if (score > _max) _max = score;
        _delay();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("The Game Is Over");
        lcd.setCursor(0,1);
        lcd.print("Score:");
        lcd.print(score);
        adc_key_in = analogRead(0);    // read the value from the sensor 
        key = get_key(adc_key_in);    // convert into key press
        if (key == 4) {
          gameover = false;
          gaming = false;
          lcd.clear();
          lcd.setCursor(2,0); 
          lcd.print("Plane Fighter!"); 
          lcd.setCursor(0, 1);
          lcd.print("oSTART");
          lcd.setCursor(9, 1);
          lcd.print("oSCORE");
          score = 0;
        }
    }
    //显示得分分支
    else if (scoring) {
        _delay();
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("The score is:");
        lcd.setCursor(7,1);
        lcd.print(_max);
        adc_key_in = analogRead(0);    // read the value from the sensor 
        key = get_key(adc_key_in);    // convert into key press
        if (key == 4) {
          scoring = false;;
          lcd.clear();
          lcd.setCursor(2,0); 
          lcd.print("Plane Fighter!"); 
          lcd.setCursor(0, 1);
          lcd.print("oSTART");
          lcd.setCursor(9, 1);
          lcd.print("oSCORE");
        }
    }
    //游戏分支
    else if (gaming) {
      //是否需要增加飞机
      //判断是否需要增加飞机
      if (flagPlane) {
        posY = rand()% 2;
        posX = 15;
        lcd.setCursor(posX, posY);
        lcd.print("*");
        delayGame();
        flagPlane = false;
       } else {
        lcd.setCursor(posX, posY);
        lcd.print(" ");
        posX--;
        //判断是否命中飞机
        if ((bulX == posX || bulX == posX - 1) && bulY == posY) {
          flagPlane = true;
          score++;
          lcd.setCursor(posX, posY);
          lcd.print(" ");
          lcd.setCursor(posX - 1, posY);
          lcd.print(" ");
          delayGame();
          bulX = posX = bulY = posY = -1;
          flagBullet = true;
        } else {
        //判断是否已发射子弹
        if (!flagBullet) {
        lcd.setCursor(bulX, bulY);
        lcd.print(" ");
        delayGame(interval1);
        bulX++;
        }
        lcd.setCursor(posX, posY);
        lcd.print("*");
        //判断是否已发射子弹
        if (!flagBullet)
        delayGame(interval1);
        else 
        delayGame();
        //判断是否已发射子弹
        if (!flagBullet) {
          lcd.setCursor(bulX,bulY);
          lcd.print(">");
          delayGame(interval1);
         }
        }
        //飞机已到达终点，游戏结束
        if (posX == 0) {
          lcd.setCursor(posX, posY);
          lcd.print(" ");
          flagPlane = true;
          flagBullet = true;
          gameover = true;
        }
       }
       //判断输入的键
      adc_key_in = analogRead(0);    // read the value from the sensor 
                      key = get_key(adc_key_in);    // convert into key press
                         if (key == 1)
                            upbut();
                         if (key == 2)
                            downbut();
                         if (key == 4 && flagBullet)
                            fire();
    }
    //开始界面
    else {
            adc_key_in = analogRead(0);    // read the value from the sensor 
            key = get_key(adc_key_in);    // convert into key press
                 if (key == 0) {
                    rightbut();
                    _delay();
                    scoring = true;
                 }
                 if (key == 3) {
                    leftbut();
                    _delay();
                    lcd.clear();
                    lcd.setCursor(0, 1);
                    lcd.print("@");
                    _delay();
                    eaterX = 0;
                    eaterY = 1;
                    gaming = true;
                 } 
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
