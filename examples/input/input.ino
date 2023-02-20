#include <PI4IOE5V6416.h>

PI4IOE5V6416 ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    ioex.direction(PCA95x5::Direction::IN_ALL);
}

void loop() {
    Serial.println(ioex.read(), BIN);
    delay(1000);
}
