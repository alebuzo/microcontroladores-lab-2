//     Universidad de Costa Rica
// Laboratorio de Microcontroladores
// Laboratorio 2 - Simón Dice
// Estudiantes;  Raquel Corrales Marín B92378
//               Alexa Carmona Buzo B91643        
// Enero 2022.

#include <avr/io.h>
#include <util/delay.h>





void blinking_inicial(){ //LEDs parpadean 2 veces 
  int DELAY = 5000;
  PORTB = 0b00000000; 
  _delay_ms(DELAY);
  PORTB = 0b00001111; 
  _delay_ms(DELAY);
  PORTB = 0b00000000; 
  _delay_ms(DELAY);
  PORTB = 0b00001111; 
  _delay_ms(DELAY);
  PORTB = 0b00000000;
  _delay_ms(7000);
} 

void blinking_final(){ //LEDs parpadean 3 veces 
  int DELAY = 5000;
  PORTB = 0b00000000; 
  _delay_ms(DELAY);
  PORTB = 0b00001111; 
  _delay_ms(DELAY);
  PORTB = 0b00000000; 
  _delay_ms(DELAY);
  PORTB = 0b00001111; 
  _delay_ms(DELAY);
  PORTB = 0b00000000;
  _delay_ms(DELAY);
   PORTB = 0b00001111; 
  _delay_ms(DELAY);
  PORTB = 0b00000000;
  _delay_ms(DELAY);
} 



int main(void)
{


//DEFINICIÓN DE ENTRADAS CON INTERRUPCIONES 
    GIMSK |= (1<<INT0); // interrupción externa en D2

    GIMSK |= (1<<INT1); // interrupción externa en D3

    GIMSK |= (1<<PCIE2); // Se habilita el PCIE2 donde estan las interrupciones PCINT11 Y PCINT12 

    PCMSK2 |= 0b00000001; // Se habilita el PCINT11 correspondiente al pin D0

    PCMSK2 |= 0b00000010; // Se habilita el PCINT12 correspondiente al pin D1


//DEFINICIÓN DE SALIDAS 
    DDRB = 0x0F; //Configuracion del puerto B, corresponden a salidas, puertos B0,B1,B2 y B3

  //Parpadear
  while (1) {
    blinking_inicial();
    _delay_ms(10000);
    blinking_final();
  }
}
