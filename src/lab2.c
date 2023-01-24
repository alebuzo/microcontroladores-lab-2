//     Universidad de Costa Rica
// Laboratorio de Microcontroladores
// Laboratorio 2 - Simón Dice
// Estudiantes;  Raquel Corrales Marín B92378
//               Alexa Carmona Buzo B91643        
// Enero 2022.

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>




//declaración de variables que se necesitan luego
int interrupt_0 = 0; 
int interrupt_1 = 0; 
int interrupt_2 = 0; 
int interrupt_3 = 0; 
int low_edge_int2 = 0; 
int low_edge_int3 = 0; 
int button_pressed = 0; 
int error = 0; 
int nivel = 4; 
int array_random[14] = {}; 
int array_introducido[14] = {};
int delay = 100;


void comp(int delay)
{
 int i=0;
 while(i<=delay)
 {
  while((TIFR & (1 << OCF0A) )==0);  //se espera a que OCF0A flag esté en alto
  TIFR|=(1<<OCF0A);    //toggling OCF0A
  i++;                //se incrementa la cuenta
 }
}

// ENCENDIDO DE LEDs INDIVIDUALES
void light_green(int delay){ 
  
  PORTB = 0b00000001;
  comp(delay);
  PORTB = 0b00000000;
  comp(delay);
}

void light_yellow(int delay){ 
  
  PORTB = 0b00000010;
  comp(delay);
  PORTB = 0b00000000;
  comp(delay);
}

void light_red(int delay){ 
  
  PORTB = 0b00000100;
  comp(delay);
  PORTB = 0b00000000;
  comp(delay);
}

void light_blue( int delay){ 
  
  PORTB = 0b00001000;
  comp(delay);
  PORTB = 0b00000000;
  comp(delay);
}



// GENERADOR DE NUMEROS RANDOM
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
  
  PORTB = 0b00000000; 
  comp(100);
   PORTB = 0b00001111;
  comp(100);
  PORTB = 0b00000000; 
  comp(100);
   PORTB = 0b00001111;
  comp(100);
  PORTB = 0b00000000;
  _delay_ms(7000);
} 

void blinking_final(){ //LEDs parpadean 3 veces 
  
  PORTB = 0b00000000; 
  comp(100);
   PORTB = 0b00001111;
  comp(100);
  PORTB = 0b00000000; 
  comp(100);
   PORTB = 0b00001111;
  comp(100);
  PORTB = 0b00000000;
  comp(100);
   PORTB = 0b00001111;
  comp(100);
  PORTB = 0b00000000;
  comp(100);
} 


void blinking_nivel_correcto(){ //LEDs parpadean 1 vez
  
  PORTB = 0b00000000; 
  comp(100);
  PORTB = 0b00001111; 
  comp(100);
  PORTB = 0b00000000; 
  comp(100);
} 

void turn_on_lights(int *array_random, int delay){ 
  
  for (int i = 0; i < nivel; i++) {
    int LED = array_random[i];
  
    if (LED == 0){
       
      light_green( delay); 
    }
        
    if (LED == 1){
      
      light_yellow( delay);
    }

    if (LED == 2){
      
      light_red(delay);
    }

    if (LED == 3){
      
      light_blue(delay);
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

ISR(PCINT0_vect) 
{
  if (low_edge_int2 == 0) {
    low_edge_int2 = low_edge_int2 + 1;
  }
  else if (low_edge_int2 == 1)
  {
    interrupt_2 = 1; 
    low_edge_int2 = 0;
  }
}
  
ISR(PCINT1_vect) 
{
  if (low_edge_int3 == 0){ 
    low_edge_int3 = low_edge_int3 + 1;
  }

  else if (low_edge_int3 == 1){
    
    interrupt_3 = 1; 
    low_edge_int3 = 0;
  }
}

ISR (INT0_vect){ 
interrupt_0 = 1; 
}

ISR (INT1_vect){ 
interrupt_1 = 1; 
}




int main(void)
{

    _delay_ms(5000);


    

  //DEFINICIÓN DE SALIDAS 
    DDRB = 0b00001111; //Configuracion del puerto B, corresponden a salidas, puertos B0,B1,B2 y B3

  //DEFINICIÓN DE ENTRADAS CON INTERRUPCIONES 
    GIMSK |= (1<<INT0)|(1<<INT1); // interrupciones externas en D2 y D3

    GIMSK |= (1<<PCIE0)|(1<<PCIE1);  // interrupciones por cambio de pin en A0 y B4

    PCMSK |= 0b00010000; // Se habilita el PCINT4 correspondiente al pin B4

    PCMSK1 |= 0b00000001; // Se habilita el PCINT8 correspondiente al pin A0

     DDRB |= (1<<PB1); // set PB1 as output
    
    TCCR0A=0x00;
    TCCR0B=0x00;
    TCCR0B |= (1<<CS00)|(1<<CS02);   //prescaling with 1024
    TCCR0A|=(1<<WGM01);//toggle mode and compare match  mode
    OCR0A= 161; //compare value
    TCNT0=0;

    sei(); 

    _delay_ms(1000); 


    char state = waiting_interrupt; 
    char next_state = waiting_interrupt; 

    while (1) { // loop infinito
    
       
        state = next_state;
        switch (state){

            case (waiting_interrupt): 

                if ((interrupt_0 == 1) | (interrupt_1 == 1) | (interrupt_2 == 1) |(interrupt_3 == 1)){
                    blinking_inicial();
                    interrupt_0 = 0; interrupt_1 = 0; interrupt_2 = 0; interrupt_3 = 0;
                    next_state = start_level;
                     array_random[0] = rand(); 
                     array_random[1] = rand(); 
                     array_random[2] = rand();
                     array_random[3] = rand();
                    }
                
                else{
                    next_state = waiting_interrupt;
                }

            break;


            case (start_level):

                turn_on_lights(array_random, delay);
                next_state = reading_inputs;
              
            break;
            
            
            case (reading_inputs):

                
                if (interrupt_0 == 1){
                        interrupt_0 = 0;
                        light_green( delay); //feedback de que se presiono el boton
                        array_introducido[button_pressed] = 0;
                        button_pressed = button_pressed + 1;
                        next_state = check; 
                }

                else if (interrupt_1 == 1){
                        interrupt_1 = 0;
                        light_yellow( delay); //feedback de que se presiono el boton
                        array_introducido[button_pressed] = 1;
                        button_pressed = button_pressed + 1;
                        next_state = check; 
                }

                else if (interrupt_2 == 1){
                        interrupt_2 = 0; 
                        light_red( delay); //feedback de que se presiono el boton
                        array_introducido[button_pressed] = 2;
                        button_pressed = button_pressed + 1;
                        next_state = check; 
                }

                else if (interrupt_3 == 1){
                        interrupt_3 = 0;
                        light_blue( delay); //feedback de que se presiono el boton
                        array_introducido[button_pressed] = 3;
                        button_pressed = button_pressed + 1;
                        next_state = check; 
                }

                else{
                       next_state = reading_inputs; 
                }
      
            break;

            

            case (check):

                if (array_random[button_pressed-1] != array_introducido[button_pressed-1]) {
                        next_state = lost;
                        button_pressed = 0; 
                }
                
                else if (nivel == button_pressed){
                        next_state = won; 
                }
        
                else {
                        next_state = reading_inputs;
                }

            break;


            case (won):
                
                array_random[nivel] = rand(); 
                
              if(nivel < 16){
                nivel = nivel + 1; //se enciende un led más que el nivel anterior
                delay = delay - 10;
                }
                
                else{
                nivel = 4; 
                delay = 100;
                }
               
                blinking_nivel_correcto(); 
                next_state = start_level; 
                button_pressed = 0;
            
            break;


            case (lost):

                blinking_final(); 
                _delay_ms(15000);
                next_state = waiting_interrupt; 
                nivel = 4;

            break;
        }
    }
}


