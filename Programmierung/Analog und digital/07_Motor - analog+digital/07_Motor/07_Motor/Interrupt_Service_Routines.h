/*
 * Interrupt_Service_Routines.h
 *
 * Created: 2020
 */ 


#ifndef INTERRUPT_SERVICE_ROUTINES_H_
#define INTERRUPT_SERVICE_ROUTINES_H_

//Interrupt service routines (ISR)

#define	LED_TOGGLE		PINB |= (1<<PINB5) //Toggle Arduino Nano pin 13
#define Line_LED_On		PORTB |= (1 << PINB2) //Turn on Vcc of the LEDs of the line sensor
#define Line_LED_Off	PORTB &= ~(1 << PINB2) //Turn off Vcc of the LEDs of the line sensor

extern unsigned char timer;
unsigned int T2ck; //Counter TMR2 overflow

extern unsigned char US_Time_L; //Runtime of US signal, HI part
extern unsigned char US_Time_R; //Runtime of US signal, HI part
unsigned char Counter_US; //Counter TMR2 overflow generating a US-trigger every 32ms

unsigned char Counter_Transmission; //Counter of digits of a transmission of a literal
unsigned char Counter_Transmission_Data; //Counter of transmitted data

extern unsigned char Data[10]; //Number of datas displayed on Data Visualizer

unsigned char Channel; //Current analogue channel of the ADC
unsigned char Analogue_value[8]; //Number of analogue inputs
//extern unsigned char Line_all_digit;
//extern unsigned char Threshold;

//TMR2 overflow Interrupt
//To time several works
ISR(TIMER2_OVF_vect) { //Prescaler TMR2 1:8 = Interrupt every 128 us
	//To save energie we turn the IR-LEDs 128us on, before the conversion of the voltage starts
	//After the 4us-sample-and-hold-cycle we start conversion and turn the IR-LEDs off
	//So we get values between ~100 (white surface) and ~250 (black)
	//To get all 8 values we need 2ms
	if (T2ck & 0b00000001) { //Every 256us shifted by 128us to the 'false' case
		//LED_TOGGLE; //For debugging
		ADCSRA |= (1 << ADSC); //Start conversion, the line sensor LEDs have been lighted since 128us
		_delay_us(4); //Sample-and-Hold-Time
		Line_LED_Off; //LED has been on for 128us + 4us
	}
	else { //Every 256us shifted by 128us to the 'true' case
		Channel++; //Increment Channel by 1
		if (Channel == 8) { //We have 8 line sensors
			Channel = 0; //Modulo 8
		}
		ADMUX = 0b01100000 | (Channel); //Bit 3:0 choose the channel
		Line_LED_On; //Turn on Vcc of the LEDs of the line sensors
	}
	//Toggeling the LED, just to see, if the MC is working
	T2ck++; //Counter TMR2 OVF
	if(T2ck == 2000) { //Every 250ms
		T2ck = 0; //Don´t forget!!
		//LED_TOGGLE; //See above
	}
	//Ultrasonic distance
	Counter_US++; //256 * 128us = 32768us
	if (!Counter_US) {//Approx. every 32ms
		if(timer > 0) timer++; //increments until overflow -> ca. 8,1s
		
		PORTD |= 0b10010000; //Trigger US, HI
		_delay_us(10); //10 microseconds
		PORTD &= 0b01101111; //Trigger US, LO
		//First you have to write TCNT1H, before writting TCNT1L!!!
		TCNT1H = 0; //Timer 1 set to zero...
		TCNT1L = 0; //...to quantify the distance of the obstacle
		//LED_ON; //Tests
	}
//Transmitting data	
//It´s a little bit tricky, but it saves a plenty of time
	if (T2ck & 0b00000001) { //Every 256us, 
		//**********************needs 2us per digit**********************************
		//instead of over 100us, while waiting for transmitting byte
//		LED_ON; //Test
		Counter_Transmission++; //Four digits will be transmitted
		if (Counter_Transmission == 4) { //
			Counter_Transmission = 0; //Modulo 4
			Counter_Transmission_Data++;  //
			if (Counter_Transmission_Data == 10) { //There will be transmitted ten literals
				Counter_Transmission_Data = 0; //Modulo 10
				USART_Transmit('\n'); //At last write a new line
			}
			Computing_Transmission_Values(Data[Counter_Transmission_Data]); //Computing the next three digits
		}
		//Only initialize, there is no need waiting, while transmission is done
		if (Counter_Transmission == 3) USART_Transmit('\t'); //At last write a tabulator
		else USART_Transmit(Trans_data[Counter_Transmission]); //One digit after another
//		LED_OFF; //Test
	}
//States of Line Detector
//If you like, you may digitize the analogue values:
//if (Analogue_value[i] < threshold) Digital_value = white; 
//Then of course you have to declare i, threshold, Digital_value and white/black
	
}



//Interrupt on falling edge at RD0, US sensor
ISR(INT0_vect) {
	//LED_ON; //Test
	unsigned char junk; //Only to read TCNT1L
	junk = TCNT1L; //We have to read the low Byte first, only then we could read TCN1H, but we don´t need the value
	US_Time_L = TCNT1H; //runtime of the US signal, HI part
	//16MHz Prescaler 1:8 -> 2MHz TMR1 Counter
	//LED_OFF; //Test
}

//Interrupt on falling edge at RD1, US sensor
ISR(INT1_vect) {
	//LED_ON; //Test
	unsigned char junk; //Only to read TCNT1L
	junk = TCNT1L; //We have to read the low Byte first, only then we could read TCN1H, but we don´t need the value
	US_Time_R = TCNT1H; //runtime of the US signal, HI part
	//16MHz Prescaler 1:8 -> 2MHz TMR1 Counter
	//LED_OFF; //Test
}

//Timer 1 Overflow Interrupt
ISR(TIMER1_OVF_vect) { //Prescaler TMR1 1:8 = Interrupt every 32 ms
	//US_Time_L = 255; //If there is no obstacle
	//LED_TOGGLE; //for testing
}

//Interrupt on completition of Analog_to_Digital_Conversion
//A conversion on a prescaler of 1:32 needs ~28us, sample and hold is about 4us
ISR(ADC_vect) { //Called every 256us (2048 cycles)
	//LED_TOGGLE; //For debugging
	Analogue_value[Channel] = ADCH; //HI byte of the resolution
}
#endif /* INTERRUPT_SERVICE_ROUTINES_H_ */