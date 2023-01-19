//     Universidad de Costa Rica
// Laboratorio de Microcontroladores
// Laboratorio 2 - Simón Dice
// Estudiantes;  Raquel Corrales Marín B92378
//               Alexa Carmona Buzo B91643        
// Enero 2022.

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>




int interrupcion_D0 = 0; 
int interrupcion_D1 = 0; 
int interrupcion_D2 = 0; 
int interrupcion_D3 = 0; 
int button_pressed  = 0; 
int nivel = 4; 
int error = 0; 
int array_random[100] = {}; 
int array_introducido[100] = {};


void light_red(){
  int DELAY = 5000;
  PORTB = 0b00000100;
  _delay_ms(DELAY);
  PORTB = 0b00000000;
}

void light_blue(){
  int DELAY = 5000;
  PORTB = 0b00001000;
  _delay_ms(DELAY);
  PORTB = 0b00000000;
}

void light_green(){
  int DELAY = 5000;
  PORTB = 0b00000001;
  _delay_ms(DELAY);
  PORTB = 0b00000000;
}

void light_yellow(){
  int DELAY = 5000;
  PORTB = 0b00000010;
  _delay_ms(DELAY);
  PORTB = 0b00000000;
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

void blinking_nivel_correcto(){ //LEDs parpadean 1 vez
  int DELAY = 5000;
  
  PORTB = 0b00000000; 
  _delay_ms(DELAY);
  PORTB = 0b00001111; 
} 


void turn_on_lights(int *array_random){ 
  int DELAY = 2000;
  for (int i = 0; i < nivel; i++) {
    int LED = array_random[i];
  
    if (LED == 0){
      _delay_ms(DELAY); 
      light_green(); 
    }
        
    if (LED == 1){
      _delay_ms(DELAY);
      light_yellow();
    }

    if (LED == 2){
      _delay_ms(DELAY);
      light_red();
    }

    if (LED == 3){
      _delay_ms(DELAY);
      light_blue();
    }
  }
}




//DEFINICIÓN DE ESTADOS 
#define waiting_interrupt 0 //esperando cambios 
//siguiente estado start_level

#define start_level 1 // se muestra el patrón
//siguiente estado reading_inputs

#define reading_inputs 2 //se recibe el patrón del usuario
//siguiente estado check

#define check 3 //se revisa el patrón ingresado para ver si coincide con el mostrado
//siguiente estado puede ser won o lost

#define won 4 //se gana el nivel y se sigue con el siguiente
//siguiente estado start_level

#define  lost 5 //se indica que el patrón no fue correcto parpadeando 3 veces
//siguiente estado debe ser waiting_interrupt





// MANEJO DE INTERRUPCIONES 

ISR (INT0_vect){ 
interrupcion_D2 = 1; 
}

ISR (INT1_vect){ 
interrupcion_D3 = 1; 
}

ISR(PCINT2_vect) 
{
  interrupcion_D0 = 1;
}
  
ISR(PCINT2_vect) 
{
  interrupcion_D1 = 1;
}



int main(void)
{


//DEFINICIÓN DE ENTRADAS CON INTERRUPCIONES 
    GIMSK |= (1<<INT0); // interrupción externa en D2

    GIMSK |= (1<<INT1); // interrupción externa en D3

    GIMSK |= (1<<PCIE2); // Se habilita el PCIE2 donde estan las interrupciones PCINT11 Y PCINT12 

    PCMSK2 |= 0b00000001; // Se habilita el PCINT11 correspondiente al pin D0

    PCMSK2 |= 0b00000010; // Se habilita el PCINT12 correspondiente al pin D1


    sei(); // La función sei() permite el manejo de interrupciones de manera global

//DEFINICIÓN DE SALIDAS 
    DDRB = 0x0F; //Configuracion del puerto B, corresponden a salidas, puertos B0,B1,B2 y B3

  
  char state = waiting_interrupt; 
  char next_state = waiting_interrupt;

  while (1) {

    state = next_state;

        switch (state){


            case (waiting_interrupt):
            
                if ((interrupcion_D0 == 1) | (interrupcion_D1 == 1) | (interrupcion_D2 == 1) |(interrupcion_D3 == 1)){
                        blinking_inicial();
                        interrupcion_D0 = 0; interrupcion_D1 = 0; interrupcion_D2 = 0; interrupcion_D3 = 0;
                        next_state = start_level;
                        
                        array_random[0] = rand(); 
                        array_random[1] = rand(); 
                        array_random[2] = rand(); 
                        array_random[3] = rand(); 
                        }
                    
                    else{
                        PORTB = 0b00000000;
                        next_state = waiting_interrupt;
                    }
                
            break;




            case (start_level):

                 turn_on_lights(array_random);
                 next_state = reading_inputs;

            break;




            case (reading_inputs):

              if (button_pressed == nivel) {
                    next_state = check;
                    button_pressed = 0; 
                }
                else{
                    next_state = reading_inputs; 

                    if (interrupcion_D2 == 1){
                        interrupcion_D2 = 0;
                        light_green(); 
                        array_introducido[button_pressed] = 0;
                        button_pressed = button_pressed + 1;
                    }

                    else if (interrupcion_D3 == 1){
                        interrupcion_D3 = 0;
                        light_yellow(); 
                        array_introducido[button_pressed] = 1;
                        button_pressed = button_pressed + 1;
                    }

                    else if (interrupcion_D0 == 1){
                        interrupcion_D0 = 0; 
                        light_red(); 
                        array_introducido[button_pressed] = 2;
                        button_pressed = button_pressed + 1;
                    }

                    else if (interrupcion_D1 == 1){
                        interrupcion_D1 = 0;
                        light_blue(); 
                        array_introducido[button_pressed] = 3;
                        button_pressed = button_pressed + 1;
                    }
                }

             

            break;




            case (check):
            
              // inicialmente se supone que no hay errores
                error = 0; 

                int i = 0;

                while (i < nivel) {
                   
                    if (array_random[i] != array_introducido[i]) {
                        error = 1;
                    }
                    i = i + 1;
                }

                if (error == 0){
                    next_state = won;
                }
                else if (error == 1){
                    next_state = lost;
                    error = 0; 
                }
               
              
            break;




            case (won):

               

            break;




            case (lost):

             
            break;


}


  }
}
