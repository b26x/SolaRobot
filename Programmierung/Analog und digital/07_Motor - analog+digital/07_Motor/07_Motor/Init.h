/*
 * Init.h
 *
 * Created: 2020
 */ 


#ifndef INIT_H_
#define INIT_H_

//Initialize the Microcontroller
void Init (void) {
	//Port settings
	//0 = Input, 1 = Output
	DDRB = 0b11111111; //11n.n,1LED,11n.n.,1LS_LED,11Motor1 u 2
	DDRC = 0b00000000; //Line-Sensors
	DDRD = 0b11110000; //1US-Trigger_2,11PWM1 u 2,1US-Trigger_1,00US-Echo_1 u _2,0TX,0RX
	//Interrupt settings
	//External
	EICRA  = 0b00001010; //INT0, INT1, falling edge at RD0(US) and 1
	EIMSK  = 0b00000011; //Enable INT0, enable INT1
	//Internal
	//Timer0 PWM
	TCCR0A = 0b10100001; //Clear OC1A/OC1B on Compare Match when upcounting. Set OC1A/OC1B on Compare Match when downcounting
	TCCR0B = 0b00000100;//00doesn´t matter,00unimpl.,0PWM,100TMR0-Pres1:256
	TIMSK0 = 0b00000000; //Timer 0 no Interrupt
	//Timer1 Ultrasonic
	TCCR1A = 0b00000000; //Normal-Mode
	TCCR1B = 0b00000010; //Timer 1 Prescaler 1:8, normal-Mode
	TIMSK1 = 0b00000001; //Interrupt at overflow, appr. 32ms
	//Timer2 Time divider
	TCCR2A = 0b00000000; //0000Normal port operation,00unimpl.,00WGM2[1:0]
	TCCR2B = 0b00000010; //0WGM22,010Prescaler 1:8
	TIMSK2 = 0b00000001; //Interrupt at TMR2-OVFL, 128us
	//ADC settings
	ADMUX  = 0b01100001; //AVCC with external capacitor at AREF pin, left adjusted, A1-Channel
	ADCSRA = 0b10001101; //1enable ADC,0don´t start conversion,0no auto trigger,0int. flag,1int. enable,110prescaler 1:32
}
#endif /* INIT_H_ */