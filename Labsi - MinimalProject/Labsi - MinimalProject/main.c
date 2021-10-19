#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 16000000UL

volatile unsigned char contador = 0;
volatile unsigned char leituraADC = 0;
volatile unsigned long int motor = 0;

#define DC(valor) ((255*valor)/100)		// velocidade do motor

void init(void){
		/*PortB is the one used for the motor and is initialized in this 2 lines*/
		DDRB = 0b11100000;
		PORTB = 0b00100000;
		/*Port C will be used as output for the led*/
		DDRD = 1; 
		PORTD=1;
		/*Here we initialize the Timer/Counter 0 with a prescaler of 1024 giving us a time of 5ms*/
		OCR0A = 77;
		TCCR0A = 0b00001111;
		TIMSK0|= 0b00000010;
		/*OCR2 is where we give the microcontroler the information of the speed we want it to operate in, we forced it to start turned off,
		this uses the Timer/Counter 2 so we also initialized it TC2 witch a prescaler of 64 mode 1 and phase correct*/
		OCR2A = DC(0);
		TCCR2A = 0b01100011;
		
		sei();
}

ISR(TIMER0_COMPA_vect)
{
	contador++;
	if(contador==200){
		PORTB ^= (1 << 1); //If the bit is 1 it will be forced to 0 if it is 0 then 1 will be forced
		contador= 0;  //resets the timer
	}
}
ISR(TIMER2_COMPA_vect)
{
	OCR2A = DC(motor);
}

int main(void)
{
	init();
    while (1) 
    {
    }
}

void ler_AD(void){
	ADCSRA = ADCSRA | 0b01000000;
	
	while((ADCSRA & (1<<ADSC)) != 0);
	
	leituraADC = ADCL;
	motor =  ADCL/1023;
}

