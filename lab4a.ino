/*
 Target Device: ATmega 328P


 Description: this program is used to read the digital value after conversion and display on the serial monitor using ADC
  
 */
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int DValue; // DValue = digital value
volatile float AValue; // AValue = analog value

void setup() {
  
  Serial.begin(9600);
  
  setTimer();
  
  ADCSRA = 0;
  ADCSRA |= (( 1 << ADPS0 ) | ( 1 << ADPS1 ) | ( 1 << ADPS2 ) | ( 1 << ADIE ) | ( 1 << ADATE )); //ADC prescalar = 128, ADC interrupt enabled, Auto Trigger Enabled 
  
  ADCSRB = 0;
  ADCSRB |= (( 1 << ADTS0 ) | ( 1 << ADTS2 ));//ADC Auto Trigger Source Selection for Timer/Counter1 Compare Match B

  ADMUX = 0;
  ADMUX |= (( 1 << REFS0 ) | ( 1 << ADLAR ));//ADC multiplexer selection for mux0
  ADCSRA |= ( 1 << ADEN ); // ADC enabled
  
  TCCR1B |= ( 1 << CS11 ); // Start the timer 

}

void loop() {
  
  Serial.print("Digital value = ");//printing the digital value
  Serial.print(DValue);
  delay (2000);// delay to not make the printing very fast
  
  Serial.print("  ,analog value=  ");//printing the analog value
  Serial.println(AValue);
  delay (2000);
  
 }

void setTimer(){
  
  TCCR1A = 0;
  TCCR1B = 0;

  cli();

  TCNT1 = 0;
  
  OCR1B = 20000;

  TIMSK1 |= ( 1 << OCIE1B );
  
  sei();
  
}

ISR(TIMER1_COMPB_vect){
  
    TCNT1 = 0;
   
}

ISR(ADC_vect){
  
    DValue = ADCH;

    AValue = ( DValue / 255.0 ) * 5.0 ;     
    
}
