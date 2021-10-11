#include <Servo.h>                             //подключение либы для мотора
#include <Adafruit_NeoPixel.h>                 //подключение либы для светодиодов

#define FIRST_MOTOR 2                          //пины для подключния моторов
#define SECOND_MOTOR 3
#define THIRD_MOTOR 4
#define FOURTH_MOTOR 5

#define BUTTON_PIN 6

#define LED_1 7                                 //пины для светодионой ленты
#define LED_2 8
#define LED_3 9
#define LED_4 10
#define LED_BACKGROUND 11
#define NUMPIXELS 16                            //количество пинов в каждом отрезеке ленты
#define NUMPIXELS_BACKGROUND 32
#define DELAYVAL 500                            //время между переключениями режимов светодиодов

Adafruit_NeoPixel pixels_1(NUMPIXELS, LED_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_2(NUMPIXELS, LED_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_3(NUMPIXELS, LED_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_4(NUMPIXELS, LED_4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel background(NUMPIXELS_BACKGROUND, LED_BACKGROUND, NEO_GRB + NEO_KHZ800);

Servo M1;                                       //ШИМ каналы
Servo M2;
Servo M3;
Servo M4;

int minPWM = 1000;
int current_position = 0;
float maxdelta = 10;

byte colour_red = 0;
byte colour_green = 0;
byte colour_blue = 0;

void setup() {
  Serial.begin (115200);
  while (!Serial) {}

  M1.attach(FIRST_MOTOR);                        //подключаем моторы к ихсигнальным пинам
  M2.attach(SECOND_MOTOR);
  M3.attach(THIRD_MOTOR);
  M4.attach(FOURTH_MOTOR);

  M1.write(minPWM);                              //подаем минимальный уровень сигнала на каждый пин
  M2.write(minPWM);
  M3.write(minPWM);
  M4.write(minPWM);

  unsigned int timer = millis();
  while ((millis() - timer) < 5000) {}            //ждем 5 секунд

  pixels_1.begin();                                 //начало работы сос светодиодными лентами
  pixels_2.begin();
  pixels_3.begin();
  pixels_4.begin();
  background.begin();
  pixels_1.clear();
  pixels_2.clear();
  pixels_3.clear();
  pixels_4.clear();
  background.clear();
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == HIGH)            //проверяем нажатие кнопки
  {
    delay(10);                                    //если кнопка была нажата, то ждем 10 мс и проверяем нажатие снова
    if (digitalRead(BUTTON_PIN) == HIGH)          //если через 10 мс кнопка еще нажата (не дрбезг), то выполняем действия
    {
      current_position = minPWM;
      while (current_position <= 1200)
      {
        M1.write(current_position);               //подаем сигнал на запуск моторов, плавно раскручивая их в течение 3 сек
        M2.write(current_position);
        M3.write(current_position);
        M4.write(current_position);
        current_position += maxdelta;
        delay (150);
      }
      unsigned int timer = millis();
      while ((millis() - timer) < 2000) {}          //ждем 2 секунды

      while (current_position >= minPWM)
      {
        M1.write(current_position);                 //плавно отсанавливаем мотор, в течение 1 секунды
        M2.write(current_position);
        M3.write(current_position);
        M4.write(current_position);
        current_position -= maxdelta;
        delay (50);

        timer = millis();
        while ((millis() - timer) < 10000) {}       //ждем 10 секунд
      }
    }
  }
  copter_led();
  int num_pin = random (NUMPIXELS_BACKGROUND);
  background_light_random(num_pin, 1000);
  background_light(3000);
}


void copter_led()
{
  //  pixels_1.clear();
  //  pixels_2.clear();
  //  pixels_3.clear();
  //  pixels_4.clear();

  if (colour_red < 256) {
    colour_red += 10;
  } else {
    if (colour_green < 256) {
      colour_green += 10;
    } else {
      if (colour_blue < 256) {
        colour_blue += 10;
      }
    }
  }
  if ((colour_red > 255) & (colour_green > 255) & (colour_blue > 255)) {
    if (colour_blue > 255) {
      colour_blue -= 10;
    } else {
      if (colour_green > 255) {
        colour_green -= 10;
      } else {
        if (colour_red > 255) {
          colour_red -= 10;
        }
      }
    }
  }
  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    pixels_1.setPixelColor(i, pixels_1.Color(colour_red, colour_green, colour_blue));
    pixels_1.show();
    pixels_2.setPixelColor(i, pixels_2.Color(colour_red, colour_green, colour_blue));
    pixels_2.show();
    pixels_3.setPixelColor(i, pixels_3.Color(colour_red, colour_green, colour_blue));
    pixels_3.show();
    pixels_4.setPixelColor(i, pixels_4.Color(colour_red, colour_green, colour_blue));
    pixels_4.show();
    delay(DELAYVAL);
  }
}

unsigned long timer_delay = 0;

void background_light_random(int num_pin, int delaytime)
{
  background.setPixelColor(num_pin, background.Color(  127, 127, 127));         //Белый цвет на рндомный светодиод в основном шаре
  background.show();
  if ((abs(timer_delay - millis())) > delaytime) {
    timer_delay = millis();
    background.setPixelColor(num_pin, background.Color(  127, 127, 127));         //Белый цвет на рндомный светодиод в основном шаре
    background.show();
  }
  return 0;
}



void background_light(int delaytime)
{
  for (int i = 0; i <= NUMPIXELS_BACKGROUND; i++) {
    background.setPixelColor(i, background.Color(  127, 127, 127));
    background.show();
    if ((abs(timer_delay - millis())) > delaytime) {
      timer_delay = millis();
      background.setPixelColor(i, background.Color(  0, 0, 0));
      background.show();
    }
  }
  return 0;
}
