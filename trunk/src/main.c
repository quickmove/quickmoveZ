#include <avr/io.h>
#include <util/delay.h> 
#include <stdint.h>

void delay_20us(uint16_t count)
{
	uint16_t k;
	for (k = 0; k < count; k++) _delay_us(20);
}

void delay_us(uint16_t count)
{
	uint16_t k;
	for(k = 0; k < count; k++) _delay_loop_2(2);
}

uint8_t DELAY_COUNT_MAX = 5;	// state�ı���ʱֵ*20ms

uint8_t high_count = 72;
uint8_t low_count = 0;
uint8_t g_delay_count = 0;

/*
// ǰ��
uint8_t M_HighCount[3][21] = {
															{72,72,72, 76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72, 72,72},
															{72,76,80, 80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80, 76,72},
															{72,72,72, 76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72, 72,72}
														};
*/														
														
// ��ת														
uint8_t M_HighCount[3][21] = {
															{72,72,72, 76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72, 72,72},
															{72,76,80, 80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80, 76,72},
															{72,72,72, 72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, 72,72}
														};
/*
// ��ת
uint8_t M_HighCount[3][21] = {
															{72,72,72, 72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, 72,72},
															{72,76,80, 80,80,76,72,68,64,64,64,64,64,68,72,76,80,80,80, 76,72},
															{72,72,72, 76,80,80,80,80,80,76,72,68,64,64,64,64,64,68,72, 72,72}
														};
*/

														
//{72,72,72, 68,64,64,64,64,64,68,72,76,80,80,80,80,80,76,72, 72,72}
uint8_t currState = 0;

// Monitor���� 2.5ms
void pwmMonitor(uint8_t monitorIndex)
{
	high_count = M_HighCount[monitorIndex][currState];

	low_count = 125 - high_count;

	PORTB |= _BV(monitorIndex);
	
	delay_20us(high_count);

	PORTB &= ~_BV(monitorIndex);

	delay_20us(low_count);
}

void runForward()
{
	 	g_delay_count++;
 		
 		if(g_delay_count > DELAY_COUNT_MAX)
 		{
 			currState++;
			if(currState > 18)
				currState = 3;
 			g_delay_count = 0;
 		}
}

int main(void)
{
	// ��ʼ����
 	PORTB=0x00;
 	DDRB=0x00;

 	DDRB |= _BV(PB0);
 	DDRB |= _BV(PB1);
 	DDRB |= _BV(PB2);

	/*
	//����PB5����
	DDRB &= ~_BV(PB5);
	PORTB |= _BV(PB5);
	*/

 	// ����������ѭ��
 	while(1)
 	{
		//20ms{
 		pwmMonitor(0);
 		pwmMonitor(1);
 		pwmMonitor(2);
 		delay_20us(875);
 		//}
 		
		runForward();
		
 	}
}
