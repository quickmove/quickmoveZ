///////////////////////////////////////
// QuickmoveZero main.
// by linjing
//////////////////////////////////////
#ifndef F_CPU
#define F_CPU 8000000L
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 
#include <stdint.h>

void delay_20us(uint16_t count) {
  uint16_t k;
  for (k = 0; k < count; k++) _delay_us(20);
}

void delay_us(uint16_t count) {
  uint16_t k;
  for(k = 0; k < count; k++) _delay_loop_2(2);
}

uint8_t DELAY_COUNT_MAX = 5;	// state changed delay*20ms
uint8_t high_count = 72;        // delay count of high level
uint8_t low_count = 0;          // delay count of low level
uint8_t g_delay_count = 0;      // delay count of globe

uint8_t currState = 0;    // current running state.
uint8_t currAction = 0; // current running action.

uint8_t TAB_HighCount[4][3][29] = {
  // forward
  {
    {72,72,72, 76,80,80,80,80,80,80,80,76,72,72,72,68,64,64,64,64,64,64,64,68,72,72,72, 72,72},
    {72,76,80, 80,80,80,80,76,72,68,64,64,64,64,64,64,64,64,64,68,72,76,80,80,80,80,80, 76,72},
    {72,72,72, 72,72,76,80,80,80,80,80,80,80,76,72,72,72,68,64,64,64,64,64,64,64,68,72, 72,72}
/*     {72,72,72,76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72,72,72}, */
/*     {72,76,80,80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80,76,72}, */
/*     {72,72,72,76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72,72,72} */
  },
  // right
  {
    {72,72,72, 76,80,80,80,80,80,80,80,76,72,72,72,68,64,64,64,64,64,64,64,68,72,72,72, 72,72},
    {72,76,80, 80,80,80,80,76,72,68,64,64,64,64,64,64,64,64,64,68,72,76,80,80,80,80,80, 76,72},
    {72,72,72, 72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, 72,72}
/*     {72,72,72,76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72,72,72}, */
/*     {72,76,80,80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80,76,72}, */
/*     {72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72} */
  },
  //left
  {
    {72,72,72, 72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, 72,72},
    {72,76,80, 80,80,80,80,76,72,68,64,64,64,64,64,64,64,64,64,68,72,76,80,80,80,80,80, 76,72},
    {72,72,72, 72,72,76,80,80,80,80,80,80,80,76,72,72,72,68,64,64,64,64,64,64,64,68,72, 72,72}
/*     {72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72}, */
/*     {72,76,80,80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80,76,72}, */
/*     {72,72,72,76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72,72,72} */
  },
  //backward
  {
    {72,72,72, 72,72,76,80,80,80,80,80,80,80,76,72,72,72,68,64,64,64,64,64,64,64,68,72, 72,72},
    {72,76,80, 80,80,80,80,76,72,68,64,64,64,64,64,64,64,64,64,68,72,76,80,80,80,80,80, 76,72},
    {72,72,72, 76,80,80,80,80,80,80,80,76,72,72,72,68,64,64,64,64,64,64,64,68,72,72,72, 72,72}
/*     {72,72,72,76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72,72,72}, */
/*     {72,76,80,80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80,76,72}, */
/*     {72,72,72,76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72,72,72} */
  }
};

void pwmMonitor(uint8_t monitorIndex) {
  high_count = TAB_HighCount[currAction][monitorIndex][currState];
  low_count = 125 - high_count;
  PORTB |= _BV(monitorIndex);
  delay_20us(high_count);
  PORTB &= ~_BV(monitorIndex);
  delay_20us(low_count);
}

void running(void) {
  g_delay_count++;
  if(g_delay_count > DELAY_COUNT_MAX) {
    currState++;
    if(currState > 26)
      currState = 3;
    g_delay_count = 0;
  }
}

int maindebug(void) {
  PORTB=0x00;
  DDRB=0x00;
  PORTC=0x00;
  DDRC=0x00;

  DDRB |= _BV(PB0);
  PORTC |= _BV(PC0);
  while(1) {
    if(PINC&_BV(PC0)) {
      PORTB |= _BV(PB0);
    }
    else {
      PORTB &= ~_BV(PB0);
    }
  }
  return 0;
}

int main(void) {
  // init port.
  PORTB=0x00;
  DDRB=0x00;
  PORTC=0x00;
  DDRC=0x00;
  PORTD=0x00;
  DDRD=0x00;

  DDRB = _BV(PB0) | _BV(PB1) | _BV(PB2);
  PORTC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
  DDRD |= _BV(PD7);

  MCUCR = _BV(ISC00) | _BV(ISC01);
  GICR=_BV(INT0);
  sei();

  // main loop.
  while(1) {
    //20ms{
    pwmMonitor(0);
    pwmMonitor(1);
    pwmMonitor(2);
    delay_20us(875);
    //}
    running();
    if(PINC&_BV(PC0)) {
      currAction = 0;
    }
    if(PINC&_BV(PC1)) {
      currAction = 1;
    }
    if(PINC&_BV(2)) {
      currAction = 2;
    }
    if(PINC&_BV(3)) {
      currAction = 3;
    }
  }
  return 0;
}

int led = 0;
// INT0 LXDR02A action
SIGNAL(SIG_INTERRUPT0) {
  if(led) {
    PORTD |= _BV(PD7);
  }
  else {
    PORTD &= ~_BV(PD7);
  }
  led = !led;

  GIFR |= _BV(INTF0);
}
