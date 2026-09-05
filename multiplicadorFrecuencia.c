/*
#include "LPC17xx.h"


volatile uint32_t ms_ticks = 0;
// Variable global que se incrementa 1 vez por milisegundo

volatile uint32_t tiempo_ms = 1000; //1 minuto

// RUTINA DE INTERRUPCIÓN DEL SYSTICK

void SysTick_Handler(void) {
    ms_ticks++;
}

void delay_ms(uint32_t ms) {
    uint32_t start_ticks = ms_ticks;
    // Espera hasta que la diferencia sea igual al tiempo pedido
    while ((ms_ticks - start_ticks) < ms) {
        // Bucle bloqueante
    }
}

// INTERRUPCIÓN DEL BOTÓN

void EINT3_IRQHandler(void) {


    if (LPC_GPIOINT->IO2IntStatF & (1 << 10)) {
        tiempo_ms = tiempo_ms / 2; // Divido a la mitad

        if(tiempo_ms < 125) { // Un límite inferior para que no parpadee a lo loco
            tiempo_ms = 1000; // Vuelve a 1 segundo
        }

        LPC_GPIOINT->IO2IntClr = (1 << 10);
    }
}

int main(void) {
    LPC_PINCON->PINSEL1 &= ~(3 << 12);
    LPC_GPIO0->FIODIR |= (1 << 22);
    LPC_PINCON -> PINSEL1 &= (3u << 27);
    LPC_GPIO0 -> FIODIR |= (1<<27);
    LPC_PINCON->PINSEL7 &= ~(3 << 20);
    LPC_GPIO3->FIODIR |= (1 << 26);
    LPC_GPIO3->FIOSET = (1 << 26);

    LPC_PINCON->PINSEL4 &= ~(3 << 20);
    LPC_GPIO2->FIODIR &= ~(1 << 10);

    LPC_GPIOINT->IO2IntEnF |= (1 << 10);
    NVIC_EnableIRQ(EINT3_IRQn);


    SysTick->CTRL = 0;              // 1. Apago el SysTick
    SysTick->LOAD = 100000 - 1;     // 2. Cargo el valor para 1ms
    SysTick->VAL  = 0;              // 3. Limpio el contador actual


    SysTick->CTRL = (0x07<<0);
    // Bit 0 = 1 (Enable)
    // Bit 1 = 1 (TickInt - Habilita la interrupción)
    // Bit 2 = 1 (ClkSource - Usa el reloj del procesador interno)

    while(1) {
        LPC_GPIO0->FIOPIN ^= (1 << 22);
        LPC_GPIO3->FIOPIN ^= (1 << 26);

        delay_ms(tiempo_ms);
    }


}
*/

#include "LPC17xx.h"
#define PERIODO      50
#define PERIODOMED   25

//Variable global modificada desde la ISR del switch
volatile uint32_t shift_ms = 0;

// Interrupción por GPIO (Puerto 2 - EINT3)
void EINT3_IRQHandler(void) {
    // Posición n = 0 (P2.0) -> En fase (0 ms)
    if (LPC_GPIOINT->IO2IntStatF & (1 << 0)) {
        shift_ms = 0;
        LPC_GPIOINT->IO2IntClr |= (1 << 0);
    }
    // Posición n = 1 (P2.1) -> Corrimiento completo (50 ms)
    else if (LPC_GPIOINT->IO2IntStatF & (1 << 1)) {
        shift_ms = 50;
        LPC_GPIOINT->IO2IntClr |= (1 << 1);
    }
    // Posición n = 2 (P2.2) -> Desplazamiento T/7 (7 ms)
    else if (LPC_GPIOINT->IO2IntStatF & (1 << 2)) {
        shift_ms = 7;
        LPC_GPIOINT->IO2IntClr |= (1 << 2);
    }
}

// Interrupción periódica de 1 ms
void SysTick_Handler(void) {

	static uint32_t tick_ms = 0;

    tick_ms = (tick_ms + 1) % PERIODO;

    // Generación de OUT1 (P0.0)
    if (tick_ms < PERIODOMED) {
        LPC_GPIO0->FIOSET = (1 << 0);
    } else {
        LPC_GPIO0->FIOCLR = (1 << 0);
    }

    //retardo restando el desfasaje en el dominio del tiempo
    uint32_t tick_shifted = (tick_ms + PERIODO - (shift_ms % PERIODO)) % PERIODO;

    if (tick_shifted < PERIODOMED) {
        LPC_GPIO0->FIOSET = (1 << 1) | (1 << 2);
    } else {
        LPC_GPIO0->FIOCLR = (1 << 1) | (1 << 2);
    }
}

int main(void) {
    // SystemCoreClockUpdate(); Funcion que recalcula la frecuencia a la que trabaja el reloj interno

    //Configurar Salidas en Puerto 0 (P0.0, P0.1, P0.2)
    LPC_PINCON -> PINSEL0  &= ~((0x3 << 0) | (0x3 << 2) | (0x3 << 4)); // Modo GPIO
    LPC_GPIO0  -> FIODIR   |= (1 << 0) | (1 << 1) | (1 << 2);         // Como salidas

    //entradas en Puerto 2 (P2.0, P2.1, P2.2)
    LPC_PINCON -> PINSEL4  &= ~((0x3 << 0) | (0x3 << 2) | (0x3 << 4)); // Modo GPIO
    LPC_GPIO2  -> FIODIR   &= ~((1 << 0) | (1 << 1) | (1 << 2));         // Como entradas
    LPC_PINCON -> PINMODE4 &= ~((0x3 << 0) | (0x3 << 2) | (0x3 << 4)); // Resistencias Pull-Up

    // interrupción por flanco descendente en P2.0, P2.1 y P2.2
    LPC_GPIOINT->IO2IntEnF |= (1 << 0) | (1 << 1) | (1 << 2);

    NVIC->ISER[0] |= (1 << EINT3_IRQn);

        // 3. Configuración manual de registros del SysTick para 1 ms
        SysTick->LOAD = (SystemCoreClock / 1000) - 1; // Registro de recarga
        SysTick->VAL  = 0;                            // Limpiar contador actual
        SysTick->CTRL = (1 << 2) |                    // CLKSOURCE: Frecuencia del CPU (1)
                        (1 << 1) |                    // TICKINT: Habilitar interrupción (1)
                        (1 << 0);                     // ENABLE: Encender temporizador (1)

    while (1) {
        // El microcontrolador queda a la espera de interrupciones
        __WFI();
    }
}

