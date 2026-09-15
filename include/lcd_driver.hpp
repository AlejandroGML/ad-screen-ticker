#ifndef LCD_DRIVER_HPP_
#define LCD_DRIVER_HPP_

#include <avr/io.h>
#include <util/delay.h>
#include "millis.h"

// Pin configuration
#define LCD_RS_DIR          DDRD
#define LCD_RS_PORT         PORTD
#define LCD_RS              (1 << PD2)

#define LCD_E_DIR           DDRD
#define LCD_E_PORT          PORTD
#define LCD_E               (1 << PD3)

#define LCD_DB4_DIR         DDRD
#define LCD_DB4_PORT        PORTD
#define LCD_DB4             (1 << PD4)

#define LCD_DB5_DIR         DDRD
#define LCD_DB5_PORT        PORTD
#define LCD_DB5             (1 << PD5)

#define LCD_DB6_DIR         DDRD
#define LCD_DB6_PORT        PORTD
#define LCD_DB6             (1 << PD6)

#define LCD_DB7_DIR         DDRD
#define LCD_DB7_PORT        PORTD
#define LCD_DB7             (1 << PD7)

// HD44780 commands
#define HD44780_CLEAR       0x01
#define HD44780_HOME        0x02
#define HD44780_DISPLAY_OFF 0x08
#define HD44780_DISPLAY_ON  0x0C
#define HD44780_FUNC_SET    0x28
#define HD44780_CURSOR_SET  0x06

// HD44780 Characteristics
#define HD44780_LINES 2
#define HD44780_CHAR_WIDTH 16

class hd44780 {
public:
    hd44780();
    void write_text(const char* text);
    void blink_text(const char* text);
    void clear(void);
    void home(void);

private:
    void write_command(unsigned char cmd);
    void write_data(unsigned char data);
    void out_nibble(unsigned char nibble);
    void write(unsigned char byte);
    void go_to(unsigned char x, unsigned char y);
};

#endif