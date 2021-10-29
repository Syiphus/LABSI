#include <avr/interrupt.h>
#include <avr/io.h>
/* We define in this line the frequency in wich the microcontroler will work, in this case it is 16MHz with the use of a crystal oscilator*/
#define F_CPU 16000000UL
/*Contador will be used to generate the square wave to change the state of the led leituraADC is the value the adc returns and motor is the OCR2A value*/
volatile unsigned char contador;
volatile unsigned char leituraADC = 0;
volatile unsigned char motor = 0;

/*In this line we define the function ler_AD*/
void ler_AD(void);

void init(void){
		/*PortB is the one used for the motor and is initialized in this 2 lines*/
		DDRB = 255;
		PORTB = 255;
		/*PortD will be used as output for the led*/
		DDRD = 255; 
		PORTD= 255;
		/*PORTC is used to read the values of the ADC so it is initialized as input*/
		DDRC = 0b00000000;
		PORTC = 0b00000000;
		/*Here we initialize the Timer/Counter 0 in compare mode with a prescaler of 128*/
		OCR0A = 77;
		TCCR0A = 0b11000010;
		TCCR0B = 0b10000101;
		TIMSK0 = 0b00000010;
		/*OCR2 is where we give the microcontroler the information of the speed we want it to operate in,
		this uses the Timer/Counter 2 so we also initialized it TC2 witch a prescaler of 128 in Fast PWM*/
		TCCR2A = 0b10000011;
		TCCR2B = 0b00000101;
		TIMSK2 = 0b00000010;
		/*The ADC converter wich will be used is the ADC1 and a division factor of 64*/
		ADMUX =  0b00100001;
		ADCSRA = 0b10000110;
		
		sei();
}
/*This is the Timer/Counter 0 interrupt where the timer of 5ms will be incremented to 100 (0.5s)
and it will check the value of the port in witch the LED is located and will alternate its value*/
ISR(TIMER0_COMPA_vect)
{
	contador++;
	if(contador==100){
		if(PORTD==0) PORTD = 1;
		else PORTD = 0;
		contador= 0;  //resets the timer
	}
}
/*This is the Timer/Counter 2 interrupt where it will check the value of the ADC [0;255] and will
atribute the value to OCR2A changing the speed of the motor*/
ISR(TIMER2_COMPA_vect)
{
	ler_AD();
	OCR2A = motor;
}

/*Program will start here this function will call the init() and will then be in an infinite loop
the program execution will be based on the Timer/Counter */
int main(void)
{
	init();
    while (1) 
    {
    }
}

void ler_AD(void){
	ADCSRA |= (1<<6);  /*Starts reading the ADC*/
	while ((ADCSRA & (1<<6)) != 0); /*Waits for the ADC to finish reading*/
	motor = ADCH; /*Stores the value of the ADCH ( Results are fixed at the left ) */
}
