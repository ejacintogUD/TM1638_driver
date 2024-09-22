/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "TM1638.h"
#include <ctime>

// Define los pines que usarás para conectar el TM1638
#define STB_PIN D10  // Pin de STB (Strobe)
#define CLK_PIN D9  // Pin de CLK (Clock)
#define DIO_PIN D8  // Pin de DIO (Data In/Out)

//prototipos 
void leer_botones(void);

//variables 
int conteo = 0;

//hilos y sistema operativo
Thread T_leer_botones (osPriorityNormal,4096, NULL, NULL);
TM1638 tm1638(STB_PIN, CLK_PIN, DIO_PIN);

int main() {
    // Inicializo el display y programa 
 
    tm1638.init();
    tm1638.setBrightness(7); // Ajusta el brillo al máximo
    printf("Arranque del programa\n\r");
    
    // Pruebo que funcione
    tm1638.displayNumber(12345678); // Muestra un número en el display
    wait_us(5e6);
    tm1638.clearDisplay();          // Limpia el display
    tm1638.clearLeds();             //limpia los leds

    //Inicializo variables 

    // Arranco hilos 
    T_leer_botones.start(leer_botones);

    while (true) 
         {
             
             tm1638.displayNumber(conteo++);
             ThisThread::sleep_for(1s);
        
        }
       
}

void leer_botones(void)
{
    uint8_t keys=0; 
    while(true)
    {
        keys = tm1638.readKeys();  // Lee el estado de las teclas
        if (keys) {
            // Procesa las teclas presionadas
            printf("Keys pressed: %02X\n", keys);
            tm1638.displayLed(keys-1, true);
        }
        else tm1638.clearLeds();
       
        ThisThread::sleep_for(100ms);
    }
}

