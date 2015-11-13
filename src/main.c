#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//TODO:
//Somebody implement this function, you can find the code at
//https://github.com/pmaclellan/WirelessClub
//Once you copy/paste that here, practice committing your changes with Git.
void hsvtorgb(uint8_t*, uint8_t*, uint8_t*, uint8_t, uint8_t, uint8_t);

volatile uint8_t knob1_value;
volatile uint8_t knob2_value;
volatile uint8_t knob3_value;
volatile uint8_t portchistory = 0;

//PCINT1 interrupt 
ISR(PCINT1_vect) 
{ 
  uint8_t changedbits;

  //capture which bits have changed since last time

  changedbits = PINC ^ portchistory;
  portchistory = PINC;

  if(changedbits & _BV(PC0))
  {
    /* PCINT8 changed */
    // Knob 1

      if (_BV(PC0) & PINC) //rising edge
      {
        if (_BV(PC1) & PINC) //clockwise
          knob1_value++;
        else 
          knob1_value--;
      }
      else //falling edge
      {
        if (_BV(PC1) & PINC)
          knob1_value--;
        else 
          knob1_value++;
      }
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
  pwm_init();
  sei();			//Enable global interrupts
  
  /*
  TODO:
    Add to the following section to set up additional input pins
    for two more rotary encoders
  */

  // OC0A is tied to pin PD6 (pin 12 on 328 chip)
  // set data direction to OUTPUT
  DDRD |= 1 << DDD6;

  /* set PD2 and PD3 as input */
  DDRC &= ~(1 << PCINT8);        /* PD2 and PD3 as input */
  DDRC &= ~(1 << PCINT9);        
  PORTC |= (1 << PCINT8)|(1 << PCINT9);  //enable pullups

  PCICR |= _BV(PCIE1); //enable PCINT1 interrupt signal
  //enable interrupts on three pins
  PCMSK1 |= (_BV(PCINT8) | _BV(PCINT10) | _BV(PCINT12)); 

  while(1) 
  {
    OCR0A = knob1_value;
    _delay_ms(1);
  }
}