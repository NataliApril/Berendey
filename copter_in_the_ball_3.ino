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
#define NUMPIXELS 17                            //количество пинов в каждом отрезеке ленты
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
  pixels_1.setBrightness(50);
  pixels_2.setBrightness(50);
  pixels_3.setBrightness(50);
  pixels_4.setBrightness(50);
}

void loop()
{
  rainbow(10);
  
  if (digitalRead(BUTTON_PIN) == HIGH)            //проверяем нажатие кнопки
  {
    delay(10);                                    //если кнопка была нажата, то ждем 10 мс и проверяем нажатие снова
    if (digitalRead(BUTTON_PIN) == HIGH)          //если через 10 мс кнопка еще нажата (не дрбезг), то выполняем действия
    {
      unsigned long timer = millis();
      while ((millis() - timer) < 5000) {
        theaterChaseRainbow(10);
        
      }
    }
  }
}


void copter_led()
{
  //  pixels_1.clear();
  //  pixels_2.clear();
  //  pixels_3.clear();
  //  pixels_4.clear();

  if (colour_red < 256) {
    colour_red += 50;
  } else {
    if (colour_green < 256) {
      colour_green += 50;
    } else {
      if (colour_blue < 256) {
        colour_blue += 50;
      }
    }
  }
  if ((colour_red > 255) & (colour_green > 255) & (colour_blue > 255)) {
    if (colour_blue > 255) {
      colour_blue -= 50;
    } else {
      if (colour_green > 255) {
        colour_green -= 50;
      } else {
        if (colour_red > 255) {
          colour_red -= 50;
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

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels_1.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels_1.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels_1.setPixelColor(i, pixels_1.gamma32(pixels_1.ColorHSV(pixelHue)));
      pixels_2.setPixelColor(i, pixels_2.gamma32(pixels_2.ColorHSV(pixelHue)));
      pixels_3.setPixelColor(i, pixels_3.gamma32(pixels_3.ColorHSV(pixelHue)));
      pixels_4.setPixelColor(i, pixels_4.gamma32(pixels_4.ColorHSV(pixelHue)));
    }
    pixels_1.show(); // Update strip with new contents
    pixels_2.show();
    pixels_3.show();
    pixels_4.show();
    delay(wait);  // Pause for a moment
  }
}


// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels_1.clear();         //   Set all pixels in RAM to 0 (off)
      pixels_2.clear();
      pixels_3.clear();
      pixels_4.clear();
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<pixels_1.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels_1.numPixels();
        uint32_t color = pixels_1.gamma32(pixels_1.ColorHSV(hue)); // hue -> RGB
        pixels_1.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        pixels_2.setPixelColor(c, color);
        pixels_3.setPixelColor(c, color);
        pixels_4.setPixelColor(c, color);
      }
      pixels_1.show();                // Update strip with new contents
      pixels_2.show();
      pixels_3.show();
      pixels_4.show();
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
