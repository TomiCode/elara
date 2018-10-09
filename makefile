# Currently using the ATmega168p MCU
MCU = atmega168p
# Configured hardware clock speed
F_CPU = 16000000
# Target project name
TARGET = elara
# All source and object files
SRC = $(TARGET).c drv/acs724.c drv/nrf24.c drv/serial.c drv/ws2812.c drv/status.c
OBJ = $(SRC:.c=.o)
# What compier whe should use
CC = avr-gcc
# objcopy toolchain binary
OBJCPY = avr-objcopy
# All compiler flags
CFLAGS = -Wall -Wextra -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections
# USB Device for programming the development board
USB_DEV = /dev/cu.usbmodem00177832

.PHONY: clean

all: $(TARGET).hex

$(TARGET).hex: $(TARGET)
	$(OBJCPY) -O ihex -R .eeprom $(TARGET) $@
	avr-size --format=avr --mcu=$(MCU) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL $< -o $@ -c

upload:
	avrdude -v -p$(MCU) -cstk500 -P$(USB_DEV) -b57600 -U flash:w:$(TARGET).hex:i

clean:
	rm -f $(OBJ) $(TARGET) $(TARGET).hex
