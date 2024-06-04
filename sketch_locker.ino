// подключение библиотеки клавиатуры 4х4 и библиотеки для работы с сервоприводом
#include <Keypad.h>
#include <Servo.h>

Servo servo; // создание объекта servo для управления сервоприводом
String pass = "0000"; // переменная для хранения пароля
int wrg = 0; // переменная для хранения количества неправильных попыток ввода пароля
int x = 75; // продолжительность сигнала в миллисекундах
int al = 0; // переменная для сигнализации
int del = 5000; // первичная задержка

// создаем матрицу для работы клавиатуры
const byte ROWS = 4; // число строк клавиатуры
const byte COLS = 4; // число столбцов клавиатуры
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'} };

byte rowPins[ROWS] = { 9,8,7,6 }; // пины, отвечающие за строки
byte colPins[COLS] = { 5,4,3,2 }; // пины, отвечающие за столбцы
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // определение клавиатуры

const int LED_GREEN = 11; // UPD: вынесли обозначения пинов
const int LED_RED = 12;
int ALARM_PIN = 10; // не const, тк будет меняться при отключении звука

void setup() {
    pinMode(LED_RED, OUTPUT); // красный светодиод
    pinMode(LED_GREEN, OUTPUT); // зеленый светодиод
    Serial.begin(9600); // подключаем Serial Monitor
    servo.attach(13); // привязываем сервопривод к 13 пину
    servo.write(0); // устанавливаем сервопривод в начальное положение (0 градусов)
}

String buffer = ""; // основной буффер
String pass1 = ""; 
String pass2 = "";
String pass3 = "";
String xas = "3"; // переменная для хранения допустиммого количества неверных паролей

char readKey() {
    char key = keypad.getKey();

    if (key) { // если клавиша нажата
        if (key == '*') { // если нажата звездочка, то буфер очистится
            buffer = "";
        } else {
            if (buffer.length() == 0) { // если буфер пустой, то записываем нажатую клавишу
                buffer = buffer + key;
            } else {
                if (key == '0' || 
                    key == '1' || 
                    key == '2' || 
                    key == '3' || 
                    key == '4' || 
                    key == '5' || 
                    key == '6' || 
                    key == '7' || 
                    key == '8' || 
                    key == '9') {
                    buffer = buffer + key;
                } else {
                    Serial.print("Неверный символ - ");
                    Serial.println(key);

                    digitalWrite(LED_RED, HIGH);
                    delay(35);
                    digitalWrite(LED_RED, LOW);

                    tone(ALARM_PIN, 400, x);
                    delay(x);
                    noTone(ALARM_PIN);
                }
            }
        }
    } else { // если клавиша не нажата
        delay(10);
    }
    return key;
}

void buttonA() { // с помощью кнопки A можно включить уровень повышенной защиты
    if (buffer.length() == 5) {
        pass1 = buffer.substring(1, 5);
        Serial.print("pass1 = ");
        Serial.println(pass1);

        if (pass1 != pass) {
            Serial.println("Неверный пароль!");
            wrg = wrg + 1;
            Serial.print("Количество неверно введенных паролей = ");

            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            delay(200);
            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            Serial.println(wrg);
            buffer = "";
        }

    } else if (buffer.length() == 6) {
        pass1 = buffer.substring(5, 6);

        if (pass1 == "0") {
            Serial.println("Неверное число!");

            digitalWrite(LED_RED, HIGH);
            delay(35);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            buffer = "";
        } else {
            xas = pass1;
            Serial.print("Количество попыток ввода пароля до включения сигнализации = ");
            Serial.println(xas);
            al = 1;
            wrg = 0;
            buffer = "";
            Serial.print("Buffer after: ");
            Serial.println(buffer);
        }
    }
}

void buttonB() { // с помощью кнопки B можно отключить повышенную защиту
    if (buffer.length() == 5) {
        pass1 = buffer.substring(1, 5);
        Serial.print("Пароль: ");
        Serial.println(pass1);
        Serial.print("Режим повышенной защиты отключён!\n");

        if (pass1 != pass) {
            Serial.println("Неверный пароль!");
            wrg = wrg + 1;
            Serial.print("Количество неверно введенных паролей = ");

            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            delay(200);
            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            Serial.println(wrg);
            buffer = "";
        } else {
            digitalWrite(LED_GREEN, HIGH);
            delay(100);
            digitalWrite(LED_GREEN, LOW);

            tone(ALARM_PIN, 1300, x);
            delay(x);
            noTone(ALARM_PIN);

            al = 0;
            buffer = "";
        }
    }
}

void buttonC() { // с помощью кнопки C можно задать время автоматического закрытия замка (от 1 до 20 сек)
    if (buffer.length() == 5) {
        pass1 = buffer.substring(1, 5);
        Serial.print("pass1 = ");
        Serial.println(pass1);
        
        if (pass1 != pass) {
            Serial.println("Неверный пароль!");
            wrg = wrg + 1;
            Serial.print("Количество неверно введенных паролей = ");

            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            delay(200);
            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);
            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            Serial.println(wrg);
            buffer = "";
        }

    } else if (buffer.length() == 7) {
        pass1 = buffer.substring(5, 7);
        if (pass1 == "00" || pass1 > "20") {
            Serial.println("Неверное значение!");

            digitalWrite(LED_RED, HIGH);
            delay(35);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            buffer = "";
        } else {
            del = pass1.toInt() * 1000;
            Serial.print("Текущая задержка: ");
            Serial.println(del);
            wrg = 0;
            buffer = "";
        }
    }
}

void buttonD() { // с помощью кнопки D можно вкл/выкл сигнализацию
    Serial.println("Введите пароль!");
    if (buffer.length() == 5) {
        pass1 = buffer.substring(1, 5);

        if (pass1 != pass) {
            Serial.println("Неверный пароль!");
            wrg = wrg + 1;
            Serial.print("Количество неверно введенных паролей = ");

            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            delay(200);
            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            Serial.println(wrg);
            buffer = "";
            
        } else {

            if (ALARM_PIN == 10) {
                ALARM_PIN = 0;
                Serial.println("Сигнализация отключена!");
                buffer = "";
            } else {
                ALARM_PIN = 10;
                Serial.println("Сигнализация включена!");
                buffer = "";   
            }
        }
    }
}

void buttonDiez() { // с помощью кнопки # можно измменить пароль
    if (buffer.length() == 5) {
        pass1 = buffer.substring(1, 5);
        Serial.print("Текущий пароль: ");
        Serial.println(pass1);
        if (pass1 != pass) {
            Serial.println("Неверный пароль!");
            wrg = wrg + 1;
            Serial.print("Количество неверно введенных паролей = ");

            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            delay(200);
            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            Serial.println(wrg);
            buffer = "";
        }
    } else if (buffer.length() == 9) {
        pass2 = buffer.substring(5, 9);
        Serial.print("Новый пароль: ");
        Serial.println(pass2);
        if (pass2 == pass1) {
            Serial.println("Новый пароль совпадает со старым!");

            digitalWrite(LED_RED, HIGH);
            delay(35);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);
            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            buffer = "";
        }
    } else if (buffer.length() == 13) {
        pass3 = buffer.substring(9, 13);
        Serial.print("Подтверждение нового пароля: ");
        Serial.println(pass3);
        if (pass2 == pass3 && pass1 == pass) {
            pass = pass3;

            Serial.print("Успешно! Новый пароль: ");
            Serial.println(pass);

            digitalWrite(LED_GREEN, HIGH);
            delay(100);
            digitalWrite(LED_GREEN, LOW);
            delay(200);
            digitalWrite(LED_GREEN, HIGH);
            delay(100);
            digitalWrite(LED_GREEN, LOW);

            wrg = 0;
            buffer = "";

        } else {
            Serial.println("Пароли не совпадают!");

            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);
            delay(200);
            digitalWrite(LED_RED, HIGH);
            delay(100);
            digitalWrite(LED_RED, LOW);

            tone(ALARM_PIN, 600, x);
            delay(x);
            noTone(ALARM_PIN);

            tone(ALARM_PIN, 400, x);
            delay(x);
            noTone(ALARM_PIN);

            buffer = "";
        }
    }
}

void loop() {
    char keyReaded = readKey();

    if (keyReaded) {
        Serial.print("Buffer before: ");
        Serial.println(buffer);
        tone(ALARM_PIN, 900, x);
        delay(x);
        noTone(ALARM_PIN);

        if (buffer[0] == 'A') {
            buttonA();
        } else if (buffer[0] == 'B') {
            buttonB();
        } else if (buffer[0] == 'C') {
            buttonC();
        } else if (buffer[0] == 'D') {
            buttonD();
        } else if (buffer[0] == '#') {
            buttonDiez();
        } else {
            if (buffer.length() == 4) {
                if (buffer == pass) {
                    Serial.println("Замок открыт!");

                    servo.write(90);
                    digitalWrite(LED_GREEN, HIGH);

                    wrg = 0;
                    Serial.print("Количество неверно введенных паролей = ");
                    Serial.println(wrg);

                    tone(ALARM_PIN, 1000, x);
                    delay(x);
                    noTone(ALARM_PIN);

                    tone(ALARM_PIN, 1200, x);
                    delay(x);
                    noTone(ALARM_PIN);

                    delay(del);
                    
                    servo.write(0);
                    delay(750);
                    digitalWrite(LED_GREEN, LOW);
                    Serial.println("Замок закрылся!");
                    tone(ALARM_PIN, 1500, x);
                    delay(x);
                    noTone(ALARM_PIN);
                } else {
                    Serial.println("Неверный пароль!");
                    wrg = wrg + 1;
                    Serial.print("Количество неверно введенных паролей = ");

                    digitalWrite(LED_RED, HIGH);
                    delay(100);
                    digitalWrite(LED_RED, LOW);
                    delay(200);
                    digitalWrite(LED_RED, HIGH);
                    delay(100);
                    digitalWrite(LED_RED, LOW);

                    tone(ALARM_PIN, 600, x);
                    delay(x);
                    noTone(ALARM_PIN);

                    tone(ALARM_PIN, 400, x);
                    delay(x);
                    noTone(ALARM_PIN);

                    Serial.println(wrg);
                    buffer = "";
                }

                if (al == 1 && wrg >= xas.toInt()) { // сигнализация режима повышенной защиты
                    Serial.print("Тревога!\n");
                    for (int i = 0; i <= 20; i += 1) {
                        digitalWrite(LED_RED, HIGH);
                        delay(10);
                        digitalWrite(LED_RED, LOW);

                        tone(ALARM_PIN, 1800, x);
                        delay(x);
                        noTone(ALARM_PIN);
                        tone(ALARM_PIN, 1900, x);
                        delay(x);
                        noTone(ALARM_PIN);

                        digitalWrite(LED_GREEN, HIGH);
                        delay(10);
                        digitalWrite(LED_GREEN, LOW);
                    }
                      wrg = 0;
                }
                buffer = "";
            }
        }
        Serial.print("Buffer after: ");
        Serial.println(buffer);
    }
}
