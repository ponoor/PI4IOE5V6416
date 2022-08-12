#include <Arduino.h>
#include <Wire.h>

namespace PI4IOE5V64XX {

namespace Reg {
    enum : uint8_t {
        INPUT_PORT_0 = 0x00,
        INPUT_PORT_1,
        OUTPUT_PORT_0,
        OUTPUT_PORT_1,
        POLARITY_INVERSION_PORT_0,
        POLARITY_INVERSION_PORT_1,
        CONFIGURATION_PORT_0,
        CONFIGURATION_PORT_1,
        OUTPUT_DRIVE_STRENGTH_REGISTER_0_0 = 0x40,
        OUTPUT_DRIVE_STRENGTH_REGISTER_0_1,
        OUTPUT_DRIVE_STRENGTH_REGISTER_1_0,
        OUTPUT_DRIVE_STRENGTH_REGISTER_1_1,
        INPUT_LATCH_REGISTER_0,
        INPUT_LATCH_REGISTER_1,
        PULL_UP_DOWN_ENABLE_REGISTER_0,
        PULL_UP_DOWN_ENABLE_REGISTER_1,
        PULL_UP_DOWN_SELECTION_REGISTER_0,
        PULL_UP_DOWN_SELECTION_REGISTER_1,
        INTERRUPT_MASK_REGISTER_0,
        INTERRUPT_MASK_REGISTER_1,
        INTERRUPT_STATUS_REGISTER_0,
        INTERRUPT_STATUS_REGISTER_1,
        OUTPUT_PORT_CONFIGURATION_REGISTER
    };
}

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
}  // namespace Port

namespace Level {
    enum Level : uint8_t { L, H };
    enum LevelAll : uint16_t { L_ALL = 0x0000, H_ALL = 0xFFFF };
}  // namespace Level

namespace Polarity {
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint16_t { ORIGINAL_ALL = 0x0000, INVERTED_ALL = 0xFFFF };
}  // namespace Polarity

namespace Direction {
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint16_t { OUT_ALL = 0x0000, IN_ALL = 0xFFFF };
}  // namespace Direction

namespace PullUpDownEnable {
    enum PullUpDownEnable : uint8_t { DISABLE, ENABLE };
    enum PullUpDownEnableAll : uint16_t { DISABLE_ALL = 0x0000, ENABLE_ALL = 0xFFFF };
}

namespace PullUpDownSelection {
    enum PullUpDownSelection : uint8_t { PULL_DOWN, PULL_UP };
    enum PullUpDownSelectionAll : uint16_t { PULL_DOWN_ALL = 0x0000, PULL_UP_ALL = 0xFFFF };
}

template <typename WireType = TwoWire>
class PI4IOE5V64XX {
    union Ports {
        uint16_t w;
        uint8_t b[2];
    };

    static constexpr uint8_t BASE_I2C_ADDR = 0x20;

    WireType* wire {nullptr};
    uint8_t addr {BASE_I2C_ADDR};
    Ports input {0x0000};
    Ports output {0xFFFF};
    Ports pol {0x0000};
    Ports dir {0xFFFF};
    Ports pe {0x0000};
    Ports pud {0x0000};
    uint8_t status {0x00};

public:
    void attach(WireType& wire, uint8_t i2c_addr = BASE_I2C_ADDR) {
        this->wire = &wire;
        this->addr = i2c_addr;
    }

    uint16_t read() {
        read_bytes(this->addr, Reg::INPUT_PORT_0, this->input.b, 2);
        return this->input.w;
    }
    Level::Level read(const Port::Port port) {
        uint16_t v = read();
        return (v & (1 << port)) ? Level::H : Level::L;
    }

    bool write(const uint16_t value) {
        this->output.w = value;
        return write_impl();
    }
    bool write(const Port::Port port, const Level::Level level) {
        if (level == Level::H) {
            this->output.w |= (1 << port);
        } else {
            this->output.w &= ~(1 << port);
        }
        return write_impl();
    }

    bool polarity(const uint16_t value) {
        this->pol.w = value;
        return polarity_impl();
    }
    bool polarity(const Port::Port port, const Polarity::Polarity pol) {
        if (pol == Polarity::INVERTED) {
            this->pol.w |= (1 << port);
        } else {
            this->pol.w &= ~(1 << port);
        }
        return polarity_impl();
    }

    bool direction(const uint16_t value) {
        this->dir.w = value;
        return direction_impl();
    }

    bool direction(const Port::Port port, const Direction::Direction dir) {
        if (dir == Direction::IN) {
            this->dir.w |= (1 << port);
        } else {
            this->dir.w &= ~(1 << port);
        }
        return direction_impl();
    }

    bool pullUpDownEnable(const uint16_t value) {
        this->pe.w = value;
        return pullup_down_enable_impl();
    }

    bool pullUpDownEnable(const Port::Port port, const PullUpDownEnable::PullUpDownEnable pe) {
        if (pe == PullUpDownEnable::ENABLE) {
            this->pe.w |= (1 << port);
        } else {
            this->pe.w &= ~(1 << port);
        }
        return pullup_down_enable_impl();
    }

    bool pullUpDownSelection(const uint16_t value) {
        this->pud.w = value;
        return pullup_down_selection_impl();
    }

    bool pullUpDownSelection(const Port::Port port, const PullUpDownSelection::PullUpDownSelection pud) {
        if (pud == PullUpDownSelection::PULL_UP) {
            this->pe.w |= (1 << port);
        } else {
            this->pe.w &= ~(1 << port);
        }
        return pullup_down_selection_impl();
    }

    uint8_t i2c_error() const {
        return status;
    }

private:
    bool write_impl() {
        return write_bytes(this->addr, Reg::OUTPUT_PORT_0, this->output.b, 2);
    }

    bool polarity_impl() {
        return write_bytes(this->addr, Reg::POLARITY_INVERSION_PORT_0, this->pol.b, 2);
    }

    bool direction_impl() {
        return write_bytes(this->addr, Reg::CONFIGURATION_PORT_0, this->dir.b, 2);
    }

    bool pullup_down_enable_impl() {
        return write_bytes(this->addr, Reg::PULL_UP_DOWN_ENABLE_REGISTER_0, this->dir.b, 2);
    }

    bool pullup_down_selection_impl() {
        return write_bytes(this->addr, Reg::PULL_UP_DOWN_SELECTION_REGISTER_0, this->dir.b, 2);
    }

    int8_t read_bytes(const uint8_t dev, const uint8_t reg, uint8_t* data, const uint8_t size) {
        wire->beginTransmission(dev);
        wire->write(reg);
        wire->endTransmission();
        wire->requestFrom(dev, size);
        int8_t count = 0;
        while (wire->available()) data[count++] = wire->read();
        return count;
    }

    bool write_bytes(const uint8_t dev, const uint8_t reg, const uint8_t* data, const uint8_t size) {
        wire->beginTransmission(dev);
        wire->write(reg);
        for (uint8_t i = 0; i < size; i++) wire->write(data[i]);
        status = wire->endTransmission();
        return (status == 0);
    }
};

}  // namespace PI4IOE5V64XX

// using PI4IOE5V6408 = PI4IOE5V64XX::PI4IOE5V64XX<>;
using PI4IOE5V6416 = PI4IOE5V64XX::PI4IOE5V64XX<>;
