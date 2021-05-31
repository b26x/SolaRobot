/*
 * 07_Motor.cpp
 * Created: 2021
 *
 * Pindescription Arduino Nano
 * No.	Func.	PIN-MC	Used as In/Out Res. Pindefinition
 * D0	RX		PD0		RX			In
 * D1	TX		PD1		TX			In
 * D2	INT0	PD2		US-Echo_L	In
 * D3	INT1	PD3		US-Echo_R	In
 * D4	Digit	PD4		US-Trigger_L Out
 * D5	OC0B	PD5		PWM-B		Out
 * D6	OC0A	PD6		PWM-A		Out
 * D7	Digit	PD7		US-Trigger_R Out	->	DDRD = 0b11110000
 * D8	Digit	PB0		Motor-B		Out
 * D9	Digit	PB1		Motor-A		Out
 *D10	Digit	PB2		LS_LED		Out
 *D11	Unused	PB3					Out
 *D12	Unused	PB4					Out
 *D13	LED		PB5		LED			Out	->	DDRB = 0b11111111
 *A0	Ana		PC0		Line-L3		In
 *A1	Ana		PC1		Line-L2		In
 *A2	Ana		PC2		Line-L1		In
 *A3	Ana		PC3		Line-L0		In
 *A4	Ana		PC4		Line-R0		In
 *A5	Ana		PC5		Line-R1		In	->	DDRC = 0b00000000
 *A6	Ana		ADC6	Line-R2		In
 *A7	Ana		ADC7	Line-R3		In
 */
#define F_CPU 16000000UL //16MHz required for delay
#include <avr/io.h> //Input/Output library
#include <util/delay.h> //Needed for using _delay_...
#include <avr/interrupt.h> //External and internal Interrupts
#include "Init.h"
#include "USART_Functions.h"
#include "Interrupt_Service_Routines.h"
#include "Drive_Functions.h"

//Definitions
#define FOSC				16000000 // Clock Speed
#define BAUD				76800//Baud rate, normally used by Arduino 9600, possible rates: n * 9600
#define MYUBRR				FOSC/16/BAUD-1 //Calculate my baud rate

#define  Line_L3	Analogue_value[0] //Line sensor left  outside
#define  Line_L2	Analogue_value[1]
#define  Line_L1	Analogue_value[2]
#define  Line_L0	Analogue_value[3] //Line sensor left  middle
#define  Line_R0	Analogue_value[4] //Line sensor right middle
#define  Line_R1	Analogue_value[5]
#define  Line_R2	Analogue_value[6]
#define  Line_R3	Analogue_value[7] //Line sensor right outside


#define d_L_normal 241
#define d_R_normal 255

//Macros
#define	LED_ON				PORTB |= (1 << PORTB5) //Push PORTB pin 5 high (LED, pin D13 "Nano")
#define	LED_OFF				PORTB &= ~(1 << PORTB5) //~ Not Operator
#define	LED_TOGGLE			PINB |= (1<<PINB5) //Toggle Arduino Nano pin D13


//Variables
unsigned char z; //For tests
unsigned char timer;
unsigned char Line_L0_digit;
unsigned char Line_R0_digit;
unsigned char Line_all_digit;
unsigned char linecounter;
unsigned char Threshold;
char sum_L;
char sum_R;

unsigned char US_Time_L; //Runtime of US signal, HI part
unsigned char US_Time_R; //Runtime of US signal, HI part

unsigned char Data[10]; //Number of datas displayed on Data Visualizer

//Funktions, declaration
void Init (void); //Initialize Microcontroller
void USART_Transmit(unsigned char); //Transmit ASCII to Data Visualizer
void USART_Init(unsigned int); //Initialize USART Transmission/Reception
void Transmit_literal (char); //Transmit literal to Data Visualizer Monitor
void Data_Visualizer (void); //Define datas to be vizualized


//Main routine
int main(void) {
	Init(); //Initalize the MC
	USART_Init(MYUBRR); //Transmision/reception initialize
	sei(); //Enable all interrupts
	//cli(); //Clear all interrupts
	Stop();
	_delay_ms(1000);
	//char Motor_Left = 185; //Normal PWM parameter for...
	//char Motor_Right = 192; //...driving straight on
	Threshold = 100;	
	
	
	
	while (1) {
		sum_L = Analogue_value[0] + Analogue_value[1] + Analogue_value[2] + Analogue_value[3];
		sum_R = Analogue_value[4] + Analogue_value[5] + Analogue_value[6] + Analogue_value[7];
		if (US_Time_L > 26 && US_Time_R > 26){
			if (sum_L > sum_R) Forward((unsigned char) 95+25*(sum_R/sum_L),127);
			else Forward(120,(unsigned char) 100+27*(sum_L/sum_R));
		}
		else{
			Stop();
		}
	}
}


/***********************************************************************************************/
//Other functions

//Data beeing transmitted to the Data Visualizer
void Data_Visualizer (void) {
	//Data[0] = Line_L3; //Line sensor left  outside
	//Data[1] = Line_L2;
	//Data[2] = Line_L1;
	//Data[3] = Line_L0; //Line sensor left  middle
	//Data[4] = Line_R0; //Line sensor right middle
	//Data[5] = Line_R1;
	//Data[6] = Line_R2;
	//Data[7] = Line_R3; //Line sensor right outside
	//Data[8] = US_Time_L; //Runtime of the US-signal, HI-part of TMR1-value
	//Data[9] = US_Time_R;
	//Data[8] = timer;
	//Data[8] = counter;
	Data[0] = Line_all_digit;
	Data[1] = linecounter;
	Data[3] = US_Time_L;
	Data[4] = US_Time_R;
	Data[6] = Line_L3;
	Data[7] = Line_R0;
}

/***************************************************************************************/





