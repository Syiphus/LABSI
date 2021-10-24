#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 16000000UL

volatile unsigned char contador;
volatile unsigned char leituraADC = 0;
volatile unsigned long int motor = 0;

#define DC(valor) (255*((valor*100)/1023))		// velocidade do motor
void ler_AD(void);

void init(void){
		/*PortB is the one used for the motor and is initialized in this 2 lines*/
		DDRB = 0b11100000;
		PORTB = 0b00101000;
		/*PortD will be used as output for the led*/
		DDRD = 255; 
		PORTD=1;
		/*Here we initialize the Timer/Counter 0 with a prescaler of 1024 giving us a time of 1ms*/
		OCR0A = 77;
		TCCR0A = 0b11000010;
		TCCR0B = 0b10000101;
		TIMSK0 = 0b00000010;
		/*OCR2 is where we give the microcontroler the information of the speed we want it to operate in, we forced it to start turned off,
		this uses the Timer/Counter 2 so we also initialized it TC2 witch a prescaler of 64 mode 1 and phase correct*/
		OCR2A = DC(0);
		TCCR2A = 0b10000001;
		TCCR2B = 0b00001100;
		TIMSK2 = 0b00000010;
		
		ADMUX =  0b00100000;
		ADCSRA = 0b10000110;
		
		sei();
}

ISR(TIMER0_COMPA_vect)
{
	contador++;
	if(contador==100){
		if(PORTD==0) PORTD = 1;
		else PORTD = 0;
		contador= 0;  //resets the timer
	}
}
ISR(TIMER2_COMPA_vect)
{
	ler_AD();
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
	
	motor = ADCH;
}
