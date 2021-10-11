#include <PS2X_lib.h>                             //библиотека для работы с джойстиком

#define DATA 8                                    //пины для джойстика
#define CMD 10
#define CLK 9
#define CS 11

#define MIN_SPEED 0
#define MAX_SPEED 60

PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;
int left_pos = 0;
int right_pos = 0;
int speed_left_forward = 0;
int speed_left_back = 0;
int speed_right_forward = 0;
int speed_right_back = 0;

//Подключение пинов арду к пинам драйвера
int speed_M [6] = {7, 6, 3, 2, 4, 5};             //пины регулировки скорости
int forward_M [6] = {32, 31, 25, 23, 27, 28};     //направление движения вперед (пины драйвера)
int back_M [6] = {33, 30, 24, 22, 26, 29};        //напрвление движения назад (пины драйвера)

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(speed_M[i], OUTPUT);
  }

  for (int i = 0; i < 6; i++) {
    pinMode(forward_M[i], OUTPUT);
    pinMode(back_M[i], OUTPUT);
  }
  //********************************************//
  for (int i = 0; i < 6; i++) {
    digitalWrite(forward_M[i], LOW);
    digitalWrite(back_M[i], LOW);
  }


  Serial.begin(115200);

  error = ps2x.config_gamepad(CLK, CMD, CS, DATA, true, true);
  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");


  type = ps2x.readType();                             //определение типа контроллера
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
}

void loop() {
  //if (type == 1) {                                    //если тип контроллера "DualShock Controller"

  ps2x.read_gamepad(false, vibrate);
  vibrate = ps2x.Analog(PSAB_BLUE);

  Serial.print("Stick Values:");
  Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
  Serial.print(",");
  Serial.println(ps2x.Analog(PSS_RY), DEC);

  //Центральное положение всех стиков
  left_pos = ps2x.Analog(PSS_LY);            //находим значение положения левого стика
  right_pos = ps2x.Analog(PSS_RY);           //находим значение положения правого стика
  Serial.print(left_pos);  Serial.print("\t");  Serial.println(right_pos);

  if ((left_pos == 127) & (right_pos == 127))
  {
    /* for (int i = 0; i < 6; i++) {
       analogWrite(speed_M[i], 50);
       digitalWrite(forward_M[i], HIGH);
       digitalWrite(back_M[i], LOW);
      }
      } else {
      for (int i = 0; i < 6; i++) {
       analogWrite(speed_M[i], 0);
       digitalWrite(forward_M[i], LOW);
       digitalWrite(back_M[i], LOW);
       Serial.println("Центральное положение");
      }
      }*/
    //останавливаем все колеса, останавливаемся на месте
    for (int i = 0; i < 6; i++) {
      analogWrite(speed_M[i], 0);
      digitalWrite(forward_M[i], LOW);
      digitalWrite(back_M[i], LOW);
      Serial.println("Центральное положение");
    }
  } else {
    if (left_pos > 127) {
      speed_left_forward = map(left_pos, 127, 255, MIN_SPEED, MAX_SPEED);        //определяем скрорость движения левых колес вперед
      speed_left_back = 0;
      //      Serial.print("speed_left_forward"); Serial.print("\t"); Serial.println(speed_left_forward);
      //      Serial.print("speed_left_back"); Serial.print("\t"); Serial.println(speed_left_back);
      for (int i = 0; i < 6; i += 2) {
        analogWrite(speed_M[i], speed_left_forward);
        digitalWrite(forward_M[i], LOW);
        digitalWrite(back_M[i], HIGH);
      }
    } else {
      speed_left_back = map(left_pos, 0, 127, MIN_SPEED, MAX_SPEED);             //определяем скрорость движения левых колес назад
      speed_left_forward = 0;
      //      Serial.print("speed_left_forward"); Serial.print("\t"); Serial.println(speed_left_forward);
      //      Serial.print("speed_left_back"); Serial.print("\t"); Serial.println(speed_left_back);
      for (int i = 0; i < 6; i += 2) {
        analogWrite(speed_M[i], speed_left_back);
        digitalWrite(forward_M[i], HIGH);
        digitalWrite(back_M[i], LOW);
      }
    }
    if (right_pos > 127) {
      speed_right_forward = map(right_pos, 127, 255, MIN_SPEED, MAX_SPEED);      //определяем скрорость движения правых колес вперед
      speed_right_back = 0;
      //      Serial.print("speed_right_forward"); Serial.print("\t"); Serial.println(speed_right_forward);
      //      Serial.print("speed_right_back"); Serial.print("\t"); Serial.println(speed_right_back);
      for (int i = 1; i < 6; i += 2) {
        analogWrite(speed_M[i], speed_right_forward);
        digitalWrite(forward_M[i], LOW);
        digitalWrite(back_M[i], HIGH);
      }
    } else {
      //if (right_pos < 127) {
      speed_right_back = map(right_pos, 0, 127, MIN_SPEED, MAX_SPEED);           //определяем скрорость движения правых колес назад
      speed_right_forward = 0;
      //      Serial.print("speed_right_back"); Serial.print("\t"); Serial.println(speed_right_back);
      //      Serial.print("speed_right_forward"); Serial.print("\t"); Serial.println(speed_right_forward);
      for (int i = 1; i < 6; i += 2) {
        analogWrite(speed_M[i], speed_right_back);
        digitalWrite(forward_M[i], HIGH);
        digitalWrite(back_M[i], LOW);
      }
    }
  }
delay(100);
}
