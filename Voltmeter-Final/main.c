/*
 * voltmeter1.c
 *
 * Created: 22-06-2021 09:52:15 PM
 * Author : User
 */ 


#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <stdlib.h>

#define enable            5
#define registerselection 6

void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);

int main(void)
{
    DDRB = 0xFF;					//configuring portb as output
    DDRA = 0;						//configuring porta as input
    DDRD = 0xFF;					//configuring portd as output
    _delay_ms(50);
    
    ADMUX |=(1<<REFS0);				//AVCC As Reference Voltage
    ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);				// Enable ADC & Set Division Factor 128
    
    float voltage = 0;				// variable to store voltage value
    char voltageshow [7];			//Array to hold voltage value in string form

    send_a_command(0x01);			//Clear Screen 0x01 = 00000001
    _delay_ms(50);
    send_a_command(0x38);			//8 Bit mode
    _delay_ms(50);
    send_a_command(0x0F);			//lcd on cursor blinking
    _delay_ms(50);
    

    while(1)
    {
		ADCSRA |=(1<<ADSC);
        voltage = ADC*0.004887*5.545455;// Voltage =  ADC value * step size * (Vin/Vout)
        send_a_string ("APPLIED ");
        send_a_command(0x80 + 0x40 + 0);//set position,first address
        send_a_string ("VOLTAGE=");
        send_a_command(0x80 + 0x40 + 8);//second address
        dtostrf(voltage, 5, 2, voltageshow);
        send_a_string(voltageshow);
        send_a_string("V   ");
        send_a_command(0x80 + 0);
        
    }
}

void send_a_command(unsigned char command)
{
    PORTB = command;
    PORTD &= ~ (1<<registerselection);
    PORTD |= 1<<enable;
    _delay_ms(2);
    PORTD &= ~1<<enable;
    PORTB = 0;
}

void send_a_character(unsigned char character)
{
    PORTB = character;
    PORTD |= 1<<registerselection;
    PORTD |= 1<<enable;
    _delay_ms(2);
    PORTD &= ~1<<enable;
    PORTB = 0;
}
void send_a_string(char *string_of_characters)
{
    while(*string_of_characters > 0)
    {
        send_a_character(*string_of_characters++);
    }
}



