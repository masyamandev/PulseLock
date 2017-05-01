#include <avr/io.h>
#include <avr/interrupt.h>

// for 1.2MHz

#define PIN_OUT (1 << PB1)
#define PWR_MASK (1 << PCINT0) // Power interrupt (Same as previous line)

volatile uint8_t pulseCounter;

main()
{

    DDRB = PIN_OUT;
    PORTB = 0;

    // init timer
    // prescale timer to 1/8th (1<<CS01) or 1/1st (1<<CS00) the clock rate
    TCCR0B = (1<<CS02); // 1/256
    // enable timer overflow interrupt
//    TIMSK0 = (1<<TOIE0);

	// init pc0 interrupt
	// External Interrupt Request 0 Enable
	GIMSK = (1 << PCIE);
	// Set pin mask
	PCMSK = PWR_MASK;

	pulseCounter = 0;

    sei();

    while (1)
    {
    	if (TCNT0 > 220)
    	{
    		PORTB = (pulseCounter == 15) ? PIN_OUT : 0;
    		pulseCounter = 0;
    		TCNT0 = 0;
    	}
    }
}


// Timer interrupt
ISR(TIM0_OVF_vect)
{
//	pulseCounter = 0;
//	PORTB = 0;
}


// Power change interrupt
ISR(PCINT0_vect)
{
	if (PINB & (1 << PB0)) {
		return;
	}
	uint8_t timer = TCNT0;
	TCNT0 = 0;
	if (timer >= 16 && timer <= 21)
	{
		pulseCounter++;
//		PORTB = PIN_OUT;
	}
	else
	{
		pulseCounter = 0;
	}
}
