#include <PI4IOE5V6416.h>

PI4IOE5V6416 ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PI4IOE5V6416::Polarity::ORIGINAL_ALL);
    ioex.direction(PI4IOE5V6416::Direction::OUT_ALL);
    ioex.write(PI4IOE5V6416::Level::L_ALL);
}

void loop() {
    for (size_t i = 0; i < 16; ++i) {
        Serial.print("set port high: ");
        Serial.println(i);

        ioex.write(i, PI4IOE5V6416::Level::H);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }

    for (size_t i = 0; i < 16; ++i) {
        Serial.print("set port low: ");
        Serial.println(i);

        ioex.write(i, PI4IOE5V6416::Level::L);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }
}
