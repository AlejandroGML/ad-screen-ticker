MCU ?= atmega328p
F_CPU ?= 16000000UL

# Plain '=' (not '?='): make has a built-in default for CXX (= g++), and '?='
# would silently keep the host compiler. Command line still overrides: make CXX=...
CXX = avr-g++
OBJCOPY = avr-objcopy
SIZE = avr-size

TARGET ?= ad-screen-ticker

CXXFLAGS = -std=gnu++20 -Wall -Wextra -Wpedantic -Wundef -Os -fno-exceptions -fno-rtti -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Iinclude
LDFLAGS = -mmcu=$(MCU)

SOURCES = \
	main.cpp \
	src/millis.cpp \
	src/lcd_driver.cpp \
	src/ad_catalog.cpp \
	src/ad_engine.cpp \
	src/app_controller.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET).hex

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

PROGRAMMER ?= arduino
PORT ?= /dev/ttyACM0

flash: $(TARGET).hex
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$<:i

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).hex

.PHONY: all clean flash