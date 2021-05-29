/*
 * Drive_Functions.h
 *
 * Created: 2020
 */ 


#ifndef DRIVE_FUNCTIONS_H_
#define DRIVE_FUNCTIONS_H_

//Pin PD6 (6) -> OC0A refers to Pin PB1 (9), MotorA
//Pin PD5 (5) -> OC0B refers to Pin PB0 (8), MotorB
#define	PWMA	OCR0A //Overflow compare register A
#define PWMB	OCR0B //Overflow compare register B

void Forward (unsigned char, unsigned char); //Direction with velocity A and B,for driving straight and driving a curve
void Backward (unsigned char, unsigned char); //Direction with velocity A and B,for driving straight and driving a curve
void Turn_right (unsigned char); //Rotate on the spot right, same velocity of the wheels
void Turn_left (unsigned char); //Rotate on the spot left, same velocity of the wheels
void Stop (void); //Stop all Motors

//Motors forward
void Forward (unsigned char velocity_A, unsigned char velocity_B) {
	PWMA = velocity_A; //Pulswidth modulation, MotorA
	PWMB = velocity_B; //MotorB
	PORTB &= ~(1 << PORTB1); //MotorA
	PORTB &= ~(1 << PORTB0); //MotorB, PORTB 0bxxxxxx00
}

//Motors backward
void Backward (unsigned char velocity_A, unsigned char velocity_B) {
	PWMA = velocity_A; //Pulswidth modulation, MotorA
	PWMB = velocity_B; //MotorB
	PORTB |= (1 << PORTB1); //MotorA
	PORTB |= (1 << PORTB0); //MotorB, PORTB 0bxxxxxx11
}

//Turn right on the spot
void Turn_right (unsigned char velocity) {
	PWMA = velocity; //Pulswidth modulation, MotorA
	PWMB = velocity; //MotorB
	PORTB |=  (1 << PORTB1); //MotorA
	PORTB &= ~(1 << PORTB0); //MotorB, PORTB 0bxxxxxx10
}

//Turn left on the spot
void Turn_left (unsigned char velocity) {
	PWMA = velocity; //Pulswidth modulation, MotorA
	PWMB = velocity; //MotorB
	PORTB &= ~(1 << PORTB1); //MotorA
	PORTB |=  (1 << PORTB0); //MotorB, PORTB 0bxxxxxx01
}

//Stop the motors
void Stop (void) {
	PWMA = 0; //Velocity = 0
	PWMB = 0; //Brake low (outputs shorted to ground)
}

#endif /* DRIVE_FUNCTIONS_H_ */