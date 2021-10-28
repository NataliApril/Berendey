#include <Servo.h>
int SERVO[3]  = {9, 10, 11};
int BUTTON[3] = {6,  7,  8};
int RELE[3]   = {3,  4,  5};
bool cur_state[3] = {true, true, true};
int step = 100;
int PWM[2] = {1500, 2000};

Servo servo[3];

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++)                       //объявляем классы для работы с сервами
  {
    servo[i].attach(SERVO[i]);
  }

  for (int i = 0; i < 3; i++)                       //выствляем минимальное положение на всех сервах
  {
    digitalWrite(RELE[i], HIGH);
    delay(10);
    servo[i].write(PWM[0]);
    delay(100);
    digitalWrite(RELE[i], LOW);
  }
}

void loop() {
  for (int i = 0; i < 3; i++)
  {
    if (digitalRead(BUTTON[i]) == LOW)
    {
      cur_state[i] = !cur_state[i];
      if (cur_state[i])
      {
        digitalWrite(RELE[i], HIGH);
        delay(10);
        for (int j = PWM[0]; j <= PWM[1]; j += step)
        {
          servo[i].write(j);
          delay(400);
        }
        digitalWrite(RELE[i], LOW);
      }
      else
      {
        digitalWrite(RELE[i], HIGH);
        delay(10);
        for (int j = PWM[1]; j <= PWM[0]; j -= step)
        {
          servo[i].write(j);
          delay(400);
        }
        digitalWrite(RELE[i], LOW);
      }
    }
  }
}
