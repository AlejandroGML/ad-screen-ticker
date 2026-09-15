#include "lcd_driver.hpp"

namespace {

constexpr const char* kFallbackDisplayText = "Invalid text";

} // namespace

hd44780::hd44780() {
    LCD_RS_DIR |= LCD_RS;
    LCD_E_DIR |= LCD_E;
    LCD_DB4_DIR |= LCD_DB4;
    LCD_DB5_DIR |= LCD_DB5;
    LCD_DB6_DIR |= LCD_DB6;
    LCD_DB7_DIR |= LCD_DB7;

    LCD_E_PORT &= ~LCD_E;
    LCD_RS_PORT &= ~LCD_RS;

    millis_wait_ms(100);
    out_nibble(0x03);
    millis_wait_ms(5);
    out_nibble(0x03);
    millis_wait_ms(100);
    out_nibble(0x03);
    millis_wait_ms(100);
    out_nibble(0x02);
    millis_wait_ms(100);

    write_command(HD44780_FUNC_SET);
    millis_wait_ms(100);

    write_command(HD44780_DISPLAY_ON);
    millis_wait_ms(100);

    clear();
    millis_wait_ms(2);

    write_command(HD44780_CURSOR_SET);
    millis_wait_ms(100);
}

void hd44780::blink_text(const char* text) {
    constexpr millis_t kTotalDisplayWindowMs = 10000UL;
    constexpr millis_t kVisibleLeadInMs = 500UL;
    constexpr millis_t kBlinkIntervalMs = 300UL;
    const char* safe_text = (text == nullptr) ? kFallbackDisplayText : text;

    write_text(safe_text);

    const millis_t start = millis_get();
    millis_wait_ms(kVisibleLeadInMs);

    bool visible = true;

    while ((millis_get() - start) < kTotalDisplayWindowMs) {
        if (visible) {
            write_command(HD44780_DISPLAY_OFF);
        } else {
            write_command(HD44780_DISPLAY_ON);
        }

        visible = !visible;
        millis_wait_ms(kBlinkIntervalMs);
    }

    write_command(HD44780_DISPLAY_ON);
    write_text(safe_text);
}

void hd44780::write_text(const char* text) {
    const char* safe_text = (text == nullptr) ? kFallbackDisplayText : text;
    clear();
    unsigned char row = 0;
    unsigned char col = 0;

    while (*safe_text && row < HD44780_LINES) {
        const char c = *safe_text++;

        if (c == '\r') {
            continue;
        }

        if (c == '\n') {
            ++row;
            if (row >= HD44780_LINES) {
                break;
            }
            col = 0;
            go_to(0, row);
            continue;
        }

        write_data(static_cast<unsigned char>(c));
        ++col;

        if (col >= HD44780_CHAR_WIDTH) {
            ++row;
            if (row >= HD44780_LINES) {
                break;
            }
            col = 0;
            go_to(0, row);
        }
    }
}

void hd44780::clear(void) {
    write_command(HD44780_CLEAR);
    millis_wait_ms(2);
}

void hd44780::home(void) {
    write_command(HD44780_HOME);
    millis_wait_ms(2);
}

void hd44780::out_nibble(unsigned char nibble) {
    if ((nibble & 1U) != 0U) {
        LCD_DB4_PORT |= LCD_DB4;
    } else {
        LCD_DB4_PORT &= ~LCD_DB4;
    }

    if ((nibble & 2U) != 0U) {
        LCD_DB5_PORT |= LCD_DB5;
    } else {
        LCD_DB5_PORT &= ~LCD_DB5;
    }

    if ((nibble & 4U) != 0U) {
        LCD_DB6_PORT |= LCD_DB6;
    } else {
        LCD_DB6_PORT &= ~LCD_DB6;
    }

    if ((nibble & 8U) != 0U) {
        LCD_DB7_PORT |= LCD_DB7;
    } else {
        LCD_DB7_PORT &= ~LCD_DB7;
    }

    LCD_E_PORT |= LCD_E;
    millis_wait_ms(1);
    LCD_E_PORT &= ~LCD_E;
}

void hd44780::write(unsigned char byte) {
    out_nibble(static_cast<unsigned char>(byte >> 4));
    out_nibble(byte);
}

void hd44780::write_command(unsigned char cmd) {
    write(cmd);
    millis_wait_ms(10);
}

void hd44780::write_data(unsigned char data) {
    LCD_RS_PORT |= LCD_RS;
    millis_wait_ms(2);

    write(data);
    millis_wait_ms(2);

    LCD_RS_PORT &= ~LCD_RS;
    millis_wait_ms(2);
}

void hd44780::go_to(unsigned char x, unsigned char y) {
    const unsigned char addr = static_cast<unsigned char>(0x80 + x + (0x40 * y));
    write_command(addr);
}