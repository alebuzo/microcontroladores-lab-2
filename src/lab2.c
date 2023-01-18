//     Universidad de Costa Rica
// Laboratorio de Microcontroladores
// Laboratorio 2 - Simón Dice
// Estudiantes;  Raquel Corrales Marín B92378
//               Alexa Carmona Buzo B91643        
// Enero 2022.

#include <avr/io.h>
#include <util/delay.h>



void light_red(){
  int DELAY = 5000;
  PORTB = 0b00000100;
  _delay_ms(DELAY);
  PROTB = 0b00000000;
}

void light_blue(){
  int DELAY = 5000;
  PORTB = 0b00001000;
  _delay_ms(DELAY);
  PROTB = 0b00000000;
}

void light_green(){
  int DELAY = 5000;
  PORTB = 0b00000001;
  _delay_ms(DELAY);
  PROTB = 0b00000000;
}

void light_yellow(){
  int DELAY = 5000;
  PORTB = 0b00000010;
  _delay_ms(DELAY);
  PROTB = 0b00000000;
}

// Generador de numeros random
uint8_t rotl(const uint8_t x, int k) {
    return (x << k) | (x >> (8 - k));
}
//seed
uint8_t s[2] = { 0, 0xA3 };

uint8_t next(void) {
    uint8_t s0 = s[0];
    uint8_t s1 = s[1];
    uint8_t result = s0 + s1;

    s1 ^= s0;
    s[0] = rotl(s0, 6) ^ s1 ^ (s1 << 1);
    s[1] = rotl(s1, 3);

    return result;
}
// Mantener el numero entre 0 y 3
int rand(void) {
    int randi = 0;
    while (1) {
        randi = next();
        if (randi < 4 && randi >= 0) {
           return randi;
        }
    }

}




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
