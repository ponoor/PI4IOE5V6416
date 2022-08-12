# PI4IOE5V6416

Arduino library for [PI4IOE5V6416](https://www.diodes.com/part/view/PI4IOE5V6416/) (16-bit general-purpose I/O expander that provides remote I/O expansion for most microcontroller families via the I2C-bus interface).

This library is forked from hideakitai's [PCA95x5](https://github.com/hideakitai/PCA95x5) library.

To Do:
- Set output drive strength
- Set input latch
- Set interrupt mask
- Read interrupt status


## Usage

You can specify a port using either index (`0` - `15`) or enum (`PI4IOE5V64XX::Port::P02`, etc.).

### Input

```C++
#include <PI4IOE5V6416.h>

PI4IOE5V6416 ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PI4IOE5V64XX::Polarity::ORIGINAL_ALL);
    ioex.direction(PI4IOE5V64XX::Direction::IN_ALL);
    ioex.pullUpDownEnable(PI4IOE5V64XX::PullUpDownEnable::ENABLE_ALL);
    ioex.pullUpDownSelection(PI4IOE5V64XX::PullUpDownSelection::PULL_UP_ALL);
}

void loop() {
    Serial.println(ioex.read(), BIN);
    delay(1000);
}
```

### Output

```C++
#include <PI4IOE5V6416.h>

PI4IOE5V6416 ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PI4IOE5V64XX::Polarity::ORIGINAL_ALL);
    ioex.direction(PI4IOE5V64XX::Direction::OUT_ALL);
    ioex.write(PI4IOE5V64XX::Level::L_ALL);
}

void loop() {
    for (size_t i = 0; i < 16; ++i) {
        Serial.print("set port high: ");
        Serial.println(i);

        ioex.write(i, PI4IOE5V64XX::Level::H);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }

    for (size_t i = 0; i < 16; ++i) {
        Serial.print("set port low: ");
        Serial.println(i);

        ioex.write(i, PI4IOE5V64XX::Level::L);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }
}
```

## APIs

```C++
void attach(WireType& wire, uint8_t i2c_addr = BASE_I2C_ADDR);
uint16_t read();
Level::Level read(const uint8_t port);
bool write(const uint16_t value);
bool write(const uint8_t port, const Level::Level level);
bool polarity(const uint16_t value);
bool polarity(const uint8_t port, const Polarity::Polarity pol);
bool direction(const uint16_t value);
bool direction(const uint8_t port, const Direction::Direction dir);
bool pullUpDownEnable(const uint16_t value);
bool pullUpDownEnable(const Port::Port port, const PullUpDownEnable::PullUpDownEnable pe);
bool pullUpDownSelection(const uint16_t value);
bool pullUpDownSelection(const Port::Port port, const PullUpDownSelection::PullUpDownSelection pud);

uint8_t i2c_error() const;
```

```C++
namespace Port {
    enum Port : uint8_t {
        P00,
        P01,
        P02,
        P03,
        P04,
        P05,
        P06,
        P07,
        P10,
        P11,
        P12,
        P13,
        P14,
        P15,
        P16,
        P17,
    };
}
namespace Level {
    enum Level : uint8_t { L, H };
    enum LevelAll : uint16_t { L_ALL = 0x0000, H_ALL = 0xFFFF };
}
namespace Polarity {
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint16_t { ORIGINAL_ALL = 0x0000, INVERTED_ALL = 0xFFFF };
}
namespace Direction {
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint16_t { OUT_ALL = 0x0000, IN_ALL = 0xFFFF };
}
namespace PullUpDownEnable {
    enum PullUpDownEnable : uint8_t { DISABLE, ENABLE };
    enum PullUpDownEnableAll : uint16_t { DISABLE_ALL = 0x0000, ENABLE_ALL = 0xFFFF };
}
namespace PullUpDownSelection {
    enum PullUpDownSelection : uint8_t { PULL_DOWN, PULL_UP };
    enum PullUpDownSelectionAll : uint16_t { PULL_DOWN_ALL = 0x0000, PULL_UP_ALL = 0xFFFF };
}
```

## License

MIT
