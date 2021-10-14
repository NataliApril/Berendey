String input_string = "";   //Объявляем до циклов

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      //если у нас пришел знак конца строки, то разбираем ее так, как нам необходимо и отчищвем строку
      Serial.println(input_string);
      input_string = "";
    } else
    {
      //Serial.println(c);
      input_string += c;  //Складываем полученные символы
    }
  }
}
