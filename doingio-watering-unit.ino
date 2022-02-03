#include <M5Atom.h>

const int pump_pin = 26;
const int sensor_pin = 32;
int set_value = 2;

void setup() {
    M5.begin(true, false, true);
    pinMode(pump_pin, OUTPUT);
    pinMode(sensor_pin, INPUT);
    digitalWrite(pump_pin, LOW);
}

void loop() {
    M5.update();

    // ボタンプッシュ検知
    if (M5.Btn.wasPressed()) {
        set_value++;
        if (set_value > 6) {
            set_value = 0;
        } 
    }

    // センサー値取得、水分レベル変換
    int moisture = analogRead(sensor_pin);
    int moisture_level = 0;
    if (moisture >= 1650) {
        moisture_level = map(moisture, 1450, 1900, 2, 0);
    } else {
        moisture_level = map(moisture, 1350, 1450, 5, 3);
    }
    if (moisture_level < 0) moisture_level = 0;
    if (moisture_level > 5) moisture_level = 5;

    Serial.print(moisture);
    Serial.print(", ");
    Serial.println(moisture_level);

    // display
    for (int i = 0; i < 5; i++) {
        if (i < moisture_level) {
            M5.dis.drawpix(5 * i, 0x0000f0);
            M5.dis.drawpix(1 + 5 * i, 0x0000f0);
            M5.dis.drawpix(2 + 5 * i, 0x0000f0);
            M5.dis.drawpix(3 + 5 * i, 0x0000f0);
        } else {
            M5.dis.drawpix(5 * i, 0x000000);
            M5.dis.drawpix(1 + 5 * i, 0x000000);
            M5.dis.drawpix(2 + 5 * i, 0x000000);
            M5.dis.drawpix(3 + 5 * i, 0x000000);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (set_value == 6) {
            M5.dis.drawpix(4 + 5 * i, 0xffffff);
        } else if (i < set_value) {
            M5.dis.drawpix(4 + 5 * i, 0xf00000);
        } else {
            M5.dis.drawpix(4 + 5 * i, 0x000000);
        }
    }

    // run pump
    if (set_value == 6) {
        digitalWrite(pump_pin, HIGH);
    } else if (moisture_level < set_value) {
        digitalWrite(pump_pin, HIGH);
    } else {
        digitalWrite(pump_pin, LOW);
    }

    delay(30);
}