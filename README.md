# PLCStandardLib
Аналог Standard.lib из CODESYS 2.3 для Arduino и т.п.

Описание в статье "Таймеры и триггеры CODESYS. Еще один шаг Arduino к классическому ПЛК"

https://habr.com/ru/post/402315/

Использование на примере Arduino:
```C
unsigned long VAR_GLOBAL_TIME = millis();
TON TON1(2000), TON2(1000);
TOF TOF1(2000);
R_TRIG R_TRIG1;
F_TRIG F_TRIG1;
RS_TRIG RS_TRIG1;
SR_TRIG SR_TRIG1;

void loop() {
  VAR_GLOBAL_TIME = millis();
  digitalWrite(Y0_PIN, TON1(!digitalRead(X0_PIN))); // выполнение с присвоением
  digitalWrite(Y1_PIN, TOF1(!digitalRead(X1_PIN)));
  if (R_TRIG1(TON1.Q)) digitalToggle(Y2_PIN);
  if (F_TRIG1(TON1.Q)) digitalToggle(Y3_PIN);
  digitalWrite(Y4_PIN, RS_TRIG1(!digitalRead(X4_PIN), !digitalRead(X5_PIN)));
  digitalWrite(Y5_PIN, SR_TRIG1(!digitalRead(X4_PIN), !digitalRead(X5_PIN)));
  
  TON2.IN = !digitalRead(X2_PIN); // присвоение
  TON2(); // выполнение
  digitalWrite(Y5_PIN, TON2.Q);
}
```
