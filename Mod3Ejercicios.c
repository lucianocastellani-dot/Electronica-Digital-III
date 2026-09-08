//Ejercicio 2 Y 3 JUNTOS
//SysTick que interrumpe cada 10 ms
/*
#include "LPC17xx.h"
volatile uint32_t ticks = 0;
void config();
void configSysTick();
void delay (uint32_t ms);
int main(){

	config();
	configSysTick();


	while(1){

	LPC_GPIO0 -> FIOPIN ^= (1<<22);
	delay(10);

	}
}

void config(void){
	LPC_PINCON  -> PINSEL1   &=~ (3u<<12);
	LPC_GPIO0   -> FIODIR    |=  (1<<22); //Configuro LED rojo
	LPC_GPIOINT -> IO0IntEnF =   (1<<22); //Activo por bajo


}
void configSysTick(){
	SysTick -> LOAD = (SystemCoreClock)/1000 - 1; //1 ms
	SysTick -> VAL  = 0;
	SysTick -> CTRL = (7u); //1 en bit 0: enable, 1:interrupcion y 2: clk inerno.
}
void SysTick_Handler(void){
	ticks ++;
}
void delay(uint32_t ms){
	volatile uint32_t tiempo10 = ticks;
	while((ticks - tiempo10)<ms){}
}
*/

/*
 * Ejercicio 4:
 */
/*
#include "LPC17xx.h"

void config();
void delay(uint16_t ms);
void configSysTick();
volatile uint16_t ticks;
const uint32_t tabla[16]= {
		0x3F, // 0 -> 0b00111111 (a,b,c,d,e,f encendidos)
	    0x06, // 1 -> 0b00000110 (b,c encendidos)
	    0x5B, // 2
	    0x4F, // 3
	    0x66, // 4
	    0x6D, // 5
	    0x7D, // 6
	    0x07, // 7
	    0x7F, // 8
	    0x6F, // 9
	    0x77, // A
	    0x7C, // b
	    0x39, // C
	    0x5E, // d
	    0x79, // E
	    0x71  // F
};
volatile uint16_t contador = 0;
int main(void){

	config();
	configSysTick();

	while(1){
		LPC_GPIO0 -> FIOCLR = 0xFF;
		LPC_GPIO0 -> FIOSET = tabla[contador];

		contador++;
		if(contador == 16){
			contador = 0;
		}
		delay(1000);
	}
}
void config(void){

	LPC_PINCON -> PINSEL0 &= ~(0xFFFF);
	LPC_GPIO0  -> FIODIR  |= (0xFF);

}
void configSysTick(void){
	SysTick -> LOAD = (SystemCoreClock)/1000 -1;
	SysTick -> VAL  = 0;
	SysTick -> CTRL = 7u;
}
void SysTick_Handler(void){
	ticks++;
}

void delay(uint16_t ms){
	volatile uint16_t tiempo1s = ticks;
	while((ticks - tiempo1s)<ms){}
}
*/
/*
 * Ejercicio 5: Dos tareas a la vez con SysTick
 */
/*
#include "LPC17xx.h"

void config();
volatile uint16_t contador = 0;
volatile uint32_t ticks1 = 0;
volatile uint32_t ticks2 = 0;
int bandera1 = 0;
int bandera2 = 0;
void delay();
void config();
int main(void){

	config();
	configSysTick();
	while(1){
		if(bandera1 == 1){
			LPC_GPIO0 -> FIOPIN ^= (1 << 22);
			bandera1 = 0;

		}
		if(bandera2 == 1){
			LPC_GPIO0 -> FIOCLR = (0xF);
			LPC_GPIO0 -> FIOSET = contador;
			contador++;
			if(contador == 16){
				contador = 0;
			}
			bandera2 = 0;

		}
	}
}

void config(void){
	//LED 	ROJO EN EL P0.22
	LPC_PINCON -> PINSEL0 &= ~(3u << 12);
	LPC_GPIO0  -> FIODIR  |= (1 << 20);
	//4 LED EN EL P0.0 AL P0.3
	LPC_PINCON -> PINSEL0 &= ~(0xFFFF);
	LPC_GPIO0  -> FIODIR  |= (0xF);
}
void configSysTick(void){
	SysTick -> LOAD = (SystemCoreClock)/1000 - 1;
	SysTick -> VAL = 0;
	SysTick -> CTRL = 7u;
}
void SysTick_Handler(void){
	ticks1++;
	ticks2++;
	if(ticks1 == 500){
		bandera1 = 1;
		ticks1   = 0;
	}
	if(ticks2 == 200){
		bandera2 = 1;
		tick2    = 0;
	}
}

*/
/*
 * Ejercicio 7
 */
/*
#include "LPC17xx.h"
volatile uint16_t contador = 0;

volatile uint16_t ticks = 0;
int bandera = 0;
void config();
void configSysTick();
void EINT1_IRQHandler();
int main(){
	config();
	configSysTick();

	NVIC_EnableIRQ(EINT1_IRQn);
	while(1){
		if(contador == 16){
			contador = 0;
		}
		if(bandera == 1){
			contador = 0;
			LPC_GPIO0 ->FIOCLR = (0xF);
			bandera  = 0;
		}
		LPC_GPIO0 ->  FIOCLR = (0xF);
		LPC_GPIO0 ->  FIOSET = contador;
	}
}

void config(void){
	LPC_PINCON -> PINSEL4 &= ~(3u << 22);
	LPC_PINCON -> PINSEL4 |= (1u << 22); // CONFIGURO COMO EINT1 EL P2
	LPC_SC     -> EXTINT   = (1 << 1);
	LPC_SC     -> EXTMODE  = (1 << 0);
	LPC_SC     -> EXTPOLAR = (0 << 0);

	//CONFGURO COMO SALIA P0.0-P0.3
	LPC_PINCON -> PINSEL0 &= ~(0xFF);
	LPC_GPIO0  -> FIODIR  |=  (0xFF);
}


void configSysTick(){
	SysTick -> LOAD = (SystemCoreClock)/1000 -1;
	SysTick -> VAL  = 0;
	SysTick -> CTRL = 7u;
}
void SysTick_Handler(void){
	ticks++;
	if(ticks == 2000){
		ticks   = 0;
		bandera = 1;
	}

}
void EINT1_IRQHandler(void){
	LPC_SC -> EXTINT = (1<<1);
	contador++;
}
*/
/*
 * EJERICIO 8
 */
#include "LPC17xx.h"

void config();
void configSysTick();
void EINT3_IRQHandler();

volatile uint16_t ticksRV = 0;
volatile uint16_t ticksA = 0;
int A_5seg = 0;
int main(){

	config();
	configSysTick();
	NVIC_EnableIRQ(EINT3_IRQn);
	while(1){}
}
void config(void){
	//CONFIGURO P0.0 COMO INTERRUPCION GPIO
	LPC_PINCON  -> PINSEL0    &= ~ (0x3FFF);
	LPC_PINCON  -> PINMODE0   &= ~ (3u << 0);
	LPC_GPIO0   -> FIODIR     &= ~ (1 << 0);
	LPC_GPIOINT -> IO0IntEnF  |=   (1 << 0);

	//CONFIGURO SALIDAS
	LPC_GPIO0  -> FIODIR |= (0x3F << 1); //PONGO EL 1110 EN EL P0.1 - P0.7
	/*
	 * P0.1 = ROJO 1
	 * P0.2 = AMARILLO 1
	 * P0.3 = VERDE 1
	 * P0.4 = ROJO 2
	 * P0.5 = AMARILLO 2
	 * P0.6 = VERDE 2
	 */

}

void configSysTick(){

	SysTick -> LOAD = (SystemCoreClock)/1000 - 1; // 1 ms
	SysTick -> VAL = 0;
	SysTick -> CTRL = 7u;
}
void SysTick_Handler(void){
	ticksRV++;ticksA++;

	if(ticksA == 5000){
		A_5seg = 1;
	}

	if (ticksRV == 25000){
		LPC_GPIO0 -> FIOCLR = (1 << 6);     //APAGO EL V DEL SEM2
		LPC_GPIO0 -> FIOSET = (1 << 5);     //PONGO EN A EL SEM2
	}
		if(ticksRV == 30000 && A_5seg == 1){
			LPC_GPIO0 -> FIOCLR = (1 << 5); // APAGO EL A DEL SEM2
			LPC_GPIO0 -> FIOSET = (1 << 4); // PRENDO EL R DEL SEM2

			LPC_GPIO0 -> FIOCLR = (1 << 1); // APAGO EL R DEL SEM1
			LPC_GPIO0 -> FIOSET = (1 << 6);// PRENDO EL V DEL SEM1
			ticksA = 0;
		}
	if(ticksRV == 55000){
		LPC_GPIO0 -> FIOCLR = (1 << 6); // APAGO V DEL SEM1
		LPC_GPIO0 -> FIOCLR = (1 << 4); // APAGO R DEL SEM 2

		LPC_GPIO0 -> FIOSET = (1 << 2); // PRENDO EL A DEL SEM1
		if (A_5seg == 1){
			LPC_GPIO0 -> FIOCLR = (1 << 2); // APAGO A DEL SEM1
			LPC_GPIO0 -> FIOSET = (1 << 1); // PRENDO R DEL SEM1
		}
		ticksRV = 0; ticksA = 0;

	}
}
void EINT3_IRQHandler(void){
	if((LPC_GPIOINT->IO0IntStatF) & (1<<0)){

		LPC_GPIOINT -> IO0IntClr = (1<<0);

	}
	ticksRV = 0;
	ticksRV = 55000;
	LPC_GPIO0 -> FIOCLR (0x3F);
	LPC_GPIO0 -> FIOSET = (1 << 2) | (1 << 4); // PRENDO EL A DEL SEM1

}



