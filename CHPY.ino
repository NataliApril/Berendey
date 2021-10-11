#define ZERO_X 6                            //концевик по оси Х
#define ZERO_Y 7                            //концевик по оси Y

#define PIN_VRY 8                           //Пин джойстика по Y
#define PIN_VRX 9                           //Пин джойстика по Х

#define MAX_val 200
#define CW_dir 1
#define CCW_dir 0

const int dirPin_X = 2;
const int stepPin_X = 3;
const int dirPin_Y = 4;
const int stepPin_Y = 5;

const int stepsPerRevolution = 200;

int X_val = 0;                              //Отклонение джойстика по Х
int Y_val = 0;                              //Отклонение джойстика по Y
int min_center_val = 490;
int max_center_val = 530;
int X_left_steps = 0;
int X_right_steps = 0;
int Y_left_steps = 0;
int Y_right_steps = 0;
int X_ofset = 0;
int Y_ofset = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  for (int i = 2; i < 6; i++) {
    pinMode (i, OUTPUT);
  }
  while (analogRead(ZERO_X) == HIGH) {
    digitalWrite(dirPin_X, CW_dir);          //задаем направление движения
    digitalWrite(dirPin_X, HIGH);
    delayMicroseconds(50);
    digitalWrite(dirPin_X, LOW);
    delayMicroseconds(50);
  }
  while (analogRead(ZERO_Y) == HIGH) {
    digitalWrite(dirPin_Y, CW_dir);          //задаем направление движения
    digitalWrite(dirPin_Y, HIGH);
    delayMicroseconds(50);
    digitalWrite(dirPin_Y, LOW);
    delayMicroseconds(50);
  }
}

void loop() {
  Serial.print("X = ");
  Serial.println(analogRead(PIN_VRX));

  Serial.print("Y = ");
  Serial.println(analogRead(PIN_VRY));

  X_val = analogRead(PIN_VRY);
  Y_val = analogRead(PIN_VRX);

  if ((X_val > min_center_val) & (X_val < max_center_val)) {
    //Центральное положение, не делаем ничего
    X_ofset = abs(X_right_steps - X_left_steps);
  } else {
    if (abs(X_right_steps) < MAX_val) {
      while (X_val > max_center_val) {
        digitalWrite(dirPin_X, CCW_dir);          //задаем направление движения
        digitalWrite(dirPin_X, HIGH);
        delayMicroseconds(50);
        digitalWrite(dirPin_X, LOW);
        delayMicroseconds(50);
        X_right_steps += 1;
        X_left_steps -= 1;
      }
    } else {
      //лента должна стать красной
    }
    if (abs(X_left_steps) < MAX_val) {
      while (X_val < min_center_val) {
        digitalWrite(dirPin_X, CW_dir);          //задаем направление движения
        digitalWrite(dirPin_X, HIGH);
        delayMicroseconds(50);
        digitalWrite(dirPin_X, LOW);
        delayMicroseconds(50);
        X_left_steps += 1;
        X_right_steps -= 1;
      }
    } else {
      //лента должна стать красной
    }
  }

  if ((Y_val > min_center_val) & (Y_val < max_center_val)) {
    //Центральное положение, не делвем ничего
    Y_ofset = abs(Y_right_steps - Y_left_steps);
  } else {
    if (abs(Y_right_steps) < MAX_val) {
      while (Y_val > max_center_val) {
        digitalWrite(dirPin_Y, CCW_dir);          //задаем направление движения
        digitalWrite(dirPin_Y, HIGH);
        delayMicroseconds(50);
        digitalWrite(dirPin_Y, LOW);
        delayMicroseconds(50);
        Y_right_steps += 1;
        Y_left_steps -= 1;
      }
    } else {
      //лента должна стать красной
    }
    if (abs(Y_right_steps) < MAX_val) {
      while (Y_val < min_center_val) {
        digitalWrite(dirPin_Y, CW_dir);          //задаем направление движения
        digitalWrite(dirPin_Y, HIGH);
        delayMicroseconds(50);
        digitalWrite(dirPin_Y, LOW);
        delayMicroseconds(50);
        Y_left_steps += 1;
        Y_right_steps -= 1;
      }
    } else {
      //лента должна стать красной
    }
  }
}
