#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//INT0 interrupt 
ISR(INT0_vect ) 
{ 
  if(bit_is_set(PIND, PD3)) //clockwise
  { 
    if (OCR0A < 245) OCR0A += 5;
    else OCR0A = 255;
  } 
  else //counter-clockwise
  { 
    if (OCR0A >= 10) OCR0A -= 5; 
    else OCR0A = 0;
  }
} 

//INT1 interrupt 
ISR(INT1_vect ) 
{ 
  if(bit_is_set(PIND, PD2)) //clockwise
  { 
    if (OCR0A < 245) OCR0A += 5;
    else OCR0A = 255;
  } 
  else //counter-clockwise
  { 
    if (OCR0A >= 10) OCR0A -= 5;
    else OCR0A = 0;
  }
} 

void pwm_init(void)
{
  // Fast PWM mode
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // clear OC0A on compare match, set on BOTTOM (non-inverting)
  TCCR0A |= (1 << COM0A1) | (0 << COM0A0);

  // Use clock directly, no prescaler
  TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);

  // initialize counter for TIMER 0
  TCNT0 = 0;
  OCR0A = 128;
}

int main(void)
{
  
  cli();			//Disable global interrupts
  pwm_init();
  sei();			//Enable global interrupts
  
  // OC0A is tied to pin PD6 (pin 12 on 328 chip)
  // set data direction to OUTPUT
  DDRD |= 1 << DDD6;

  /* set PD2 and PD3 as input */
  DDRD &= ~(1 << PD2);        /* PD2 and PD3 as input */
  DDRD &= ~(1 << PD3);        
  PORTD |= (1 << PD3)|(1 << PD2);   /* PD2 and PD3 pull-up enabled   */ 

  // enable INT0 and INT1
  EIMSK |= (1<<INT0)|(1<<INT1);
  // INT0 - falling edge, INT1 - rising edge
  EICRA |= (1<<ISC01)|(1<<ISC11)|(1<<ISC10);


  while(1) 
  {
    _delay_ms(1);
  }
}