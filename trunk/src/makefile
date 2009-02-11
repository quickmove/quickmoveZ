MCU=atmega8
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

all: main.hex
main.hex : main.out 
	$(OBJCOPY) -R .eeprom -O ihex main.out main.hex 
main.out : main.o 
	$(CC) $(CFLAGS) -o main.out -Wl,-Map,main.map main.o 
main.o : main.c 
	$(CC) $(CFLAGS) -Os -c main.c

# erase the AVR before it is programmed
load: main.hex
	uisp -dlpt=/dev/parport0 --erase  -dprog=dapa
	uisp -dlpt=/dev/parport0 --upload if=main.hex -dprog=dapa  -v=3 --hash=32

clean:
	rm -f *.o *.map *.out *.hex
