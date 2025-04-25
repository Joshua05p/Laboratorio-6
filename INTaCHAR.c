#include <avr/io.h>
#include <avr/interrupt.h>
#include "INTaCHAR.h"

void setup(){
	DDRB = 0xFF;

	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PORTD2);
	DDRD |= (1 << DDD5) | (1 << DDD4) | (1 << DDD3);

	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT18);

	sei();
}

void iniciar_USART(unsigned int ubrr) {
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0) ; // Habilitar transmisión

	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void iniciar_adc(){
	cli();
	ADMUX |= (1<<REFS0); // Voltaje de referencia 5V
	ADMUX |= (1<<MUX2) | (1<<MUX1); // Selecciona el canal 6

	ADCSRA = (1 << ADEN)  | // Habilitar ADC
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | // Prescaler 128 (125 kHz)
	(1 << ADIE); // Habilitar interrupción del ADC

	sei();
}
void iniciar_conversion(){
	ADCSRA |= (1 << ADSC);
}

void enviar(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;

}

void cadena(char *caracteres) {
	while (*caracteres != '\0'){
		enviar(*caracteres);
		caracteres++;
	}

}
