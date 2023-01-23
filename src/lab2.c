#include<avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

int intr_count=0;
int sec=0;
int flag_button = 0;
int interrupt_1 = 0;
int interrupt_0 = 0;

ISR (TIMER0_OVF_vect)      //Interrupt vector for Timer0
{
  if (intr_count==63 && flag_button == 1) //waiting for 63 because to get 1 sec delay
  {
    PORTB^=(1<<PB2); //toggling the LED
    intr_count=0; //making intr_count=0 to repeat the count
    flag_button = 0;
    ++sec;
  }
  else  intr_count++; //incrementing c upto 63
}


ISR (INT0_vect){ 
interrupt_0 = 1; 
flag_button = 1;
}

ISR (INT1_vect){ 
interrupt_1 = 1; 
flag_button = 1;

}

void timer_setup()
{
   // set PB1 as output(LED)
  TCCR0A=0x00;   //Normal mode
  TCCR0B=0x00;
  TCCR0B |= (1<<CS00)|(1<<CS02);   //prescaling with 1024
   //enabling global interrupt
  TCNT0=0;
  TIMSK|=(1<<TOIE0); //enabling timer0 interrupt
  PORTB|=(1<<PB2);
  DDRB = 0b00001111; //Configuracion del puerto B, corresponden a salidas, puertos B0,B1,B2 y B3

  //DEFINICIÓN DE ENTRADAS CON INTERRUPCIONES 
    GIMSK |= (1<<INT0)|(1<<INT1); // interrupciones externas en D2 y D3

    GIMSK |= (1<<PCIE0)|(1<<PCIE1);  // interrupciones por cambio de pin en A0 y B4

    PCMSK |= 0b00010000; // Se habilita el PCINT4 correspondiente al pin B4

    PCMSK1 |= 0b00000001; // Se habilita el PCINT8 correspondiente al pin A0

  sei();
}

  
int main () 
{
  timer_setup();
  while(1)
  {
    PORTB^=(1<<PB0); 
  }
}