//---------------- Станок ------------------//
// Driver smd-7610
//------------------------------------------//
#include <Stepper.h>
//10360 - крайнее положение
const int stepsPerRevolution = 120;   // Количество шагов
const int stepsFromConc = 400;

#define CW_dir 1
#define CCW_dir 0

#define ZERO_Y 10     //концевик по оси Y                       
#define ZERO_X 9     //концевик по оси Х

#define PIN_VRX A0    // пин подключения контакта VRX
#define PIN_VRY A1    // пин подключения контакта VRY

const int dirPin_Y = 2;
const int stepPin_Y = 3;

const int dirPin_X = 4;
const int stepPin_X = 5;

int Y_left_steps = 0;
int Y_right_steps = 0;

int X_left_steps = 0;
int X_right_steps = 0;

int X_val;
int Y_val;

//------------------------------------------//

void setup()
{
  Serial.begin(9600);

  pinMode (dirPin_Y, OUTPUT);
  pinMode (stepPin_Y, OUTPUT);

  pinMode (dirPin_X, OUTPUT);
  pinMode (stepPin_X, OUTPUT);

  pinMode(PIN_VRX, INPUT);
  pinMode(PIN_VRY, INPUT);

  pinMode(ZERO_Y, INPUT);
  pinMode(ZERO_X, INPUT);

  Serial.println(digitalRead(ZERO_X));

  while (digitalRead(ZERO_Y) == HIGH)
  {
    digitalWrite(dirPin_Y, CW_dir);          //задаем направление движения

    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin_Y, HIGH);
      delayMicroseconds(200);

      digitalWrite(stepPin_Y, LOW);
      delayMicroseconds(200);
    }
  }

  digitalWrite(dirPin_Y, CCW_dir);
  for (int x = 0; x < stepsFromConc; x++)
  {
    Y_left_steps -= 1;
    Y_right_steps += 1;
    digitalWrite(stepPin_Y, HIGH);

    delayMicroseconds(700);

    digitalWrite(stepPin_Y, LOW);
    delayMicroseconds(700);
  }

  //=======================================//

  while (digitalRead(ZERO_X) == HIGH)
  {

    digitalWrite(dirPin_X, CCW_dir);

    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin_X, HIGH);
      delayMicroseconds(200);

      digitalWrite(stepPin_X, LOW);
      delayMicroseconds(200);
    }
  }

  digitalWrite(dirPin_X, CW_dir);

  for (int x = 0; x < stepsFromConc; x++)
  {

    X_left_steps -= 1;
    X_right_steps += 1;

    digitalWrite(stepPin_X, HIGH);
    delayMicroseconds(700);

    digitalWrite(stepPin_X, LOW);
    delayMicroseconds(700);
  }

  // Serial.println(Y_left_steps);
  // Serial.println(Y_right_steps);
  Serial.println(X_left_steps);
  Serial.println(X_right_steps);
}

void loop()
{

  //------------------------------------------//
  //------ Вывод в монитор порта координат ---//
  //------------------------------------------//
  // Serial.print("X = ");
  // Serial.println(analogRead(PIN_VRX));

  // Serial.print("Y = ");
  // Serial.println(analogRead(PIN_VRY));

  
//    X_val = analogRead(PIN_VRY);
//    Y_val = analogRead(PIN_VRX);
    Serial.print(analogRead(PIN_VRY)); Serial.print("\t"); Serial.println(analogRead(PIN_VRX));

  //------------------------------------------//
  //---------- Управление по Y ---------------//
  //------------------------------------------//


  if ( (analogRead(PIN_VRY) > 490) & (analogRead(PIN_VRY) < 530) )
  {
    // Serial.println("Centre");
  }

  else
  {
    if (abs(Y_right_steps < 10000))
    {
      while (analogRead(PIN_VRY) > 531)
      {

        digitalWrite(dirPin_Y, CCW_dir);          //задаем направление движения

        for (int x = 0; x < stepsPerRevolution; x++)
        {

          digitalWrite(stepPin_Y, HIGH);
          delayMicroseconds(200);

          digitalWrite(stepPin_Y, LOW);
          delayMicroseconds(200);
          Y_left_steps -= 1;
          Y_right_steps += 1;

        }

        if (Y_right_steps == 10000)
        {
          break;
        }

      }
    }
    else
    {
      //лента должна стать красной
    }

    if (abs(Y_right_steps > 400))
    {
      while (analogRead(PIN_VRY) < 489)
      {
        //  Serial.println(analogRead(PIN_VRY));

        digitalWrite(dirPin_Y, CW_dir);          //задаем направление движения

        for (int x = 0; x < stepsPerRevolution; x++)
        {

          digitalWrite(stepPin_Y, HIGH);
          delayMicroseconds(200);

          digitalWrite(stepPin_Y, LOW);
          delayMicroseconds(200);

          Y_left_steps += 1;
          Y_right_steps -= 1;
        }

        if (Y_right_steps == 400)
        {
          break;
        }
      }
    }
    else
    {
      //лента должна стать красной
    }

  }

  //------------------------------------------//
  //---------- Управление по X ---------------//
  //------------------------------------------//

  //500 >  < 540

  if ( (analogRead(PIN_VRX) > 500) & (analogRead(PIN_VRX) < 540) )
  {
    // Serial.println("Centre");
  }

  else
  {
    if (abs(X_right_steps < 8320))
    {
      while (analogRead(PIN_VRX) > 540)
      {

        digitalWrite(dirPin_X, CW_dir);          //задаем направление движения

        for (int x = 0; x < stepsPerRevolution; x++)
        {
          digitalWrite(stepPin_X, HIGH);
          delayMicroseconds(200);

          digitalWrite(stepPin_X, LOW);
          delayMicroseconds(200);
          X_left_steps -= 1;
          X_right_steps += 1;
        }

        if (X_right_steps == 8320)
        {
          break;
        }

      }
    }
    else
    {
      //лента должна стать красной
    }

    if (abs(X_right_steps > 400))
    {
      while (analogRead(PIN_VRX) < 489)
      {
        //  Serial.println(analogRead(PIN_VRY));

        digitalWrite(dirPin_X, CCW_dir);          //задаем направление движения

        for (int x = 0; x < stepsPerRevolution; x++)
        {

          digitalWrite(stepPin_X, HIGH);
          delayMicroseconds(200);

          digitalWrite(stepPin_X, LOW);
          delayMicroseconds(200);

          X_left_steps += 1;
          X_right_steps -= 1;
        }

        if (X_right_steps == 400)
        {
          break;
        }
      }
    }
    else
    {
      //лента должна стать красной
    }
  }
//  Serial.println(X_left_steps);
//  Serial.println(X_right_steps);
  delay(10);
}
