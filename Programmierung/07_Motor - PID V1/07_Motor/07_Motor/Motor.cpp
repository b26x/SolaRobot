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


//Macros
#define	LED_ON				PORTB |= (1 << PORTB5) //Push PORTB pin 5 high (LED, pin D13 "Nano")
#define	LED_OFF				PORTB &= ~(1 << PORTB5) //~ Not Operator
#define	LED_TOGGLE			PINB |= (1<<PINB5) //Toggle Arduino Nano pin D13

#define left_n 186
#define right_n 191

//Variables
unsigned char z; //For tests
unsigned char timer;
unsigned char Line_L0_digit;
unsigned char Line_R0_digit;
unsigned char Line_all_digit;
unsigned char counter;
unsigned char Threshold;
signed int pid;
float temp;
double temp2;
double temp3;

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

		while(1){
			Data_Visualizer();
			
			if(US_Time_L < 24 || US_Time_R < 24) Stop();
			else{
				pid = Line_L3 + Line_L2 + Line_L1 + Line_L0; // sum left values
				pid = pid - Line_R3 - Line_R2 - Line_R1 - Line_R0;
			
				if(pid >= 0){
					LED_ON;
					temp = 67 * (float) pid;
					temp2 = temp/1000;
					temp3 = (left_n - temp2);
					Forward( (unsigned char) temp3,191);
				}
				else{
					LED_OFF;
					temp = 71 * (float) pid;
					temp2 = temp/1000;
					temp3 = (right_n + temp2);
					Forward(186,(unsigned char) temp3);
				}
			
				//if(pid >= 0){
					//Forward( (unsigned char) pid/4 ,191);
				//}
				//else{
					//Forward(186,(unsigned char) pid/4);
				//}
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
	//Data[7] = Line_R3;
	Data[0] = (unsigned char) temp;
	Data[1] = (unsigned char) temp2;
	Data[2] = (unsigned char) temp3;
	//Data[8] = pid;
	//Data[8] = US_Time_L; //Runtime of the US-signal, HI-part of TMR1-value
	//Data[9] = US_Time_R;

}

/***************************************************************************************/





