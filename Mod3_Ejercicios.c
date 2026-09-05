//Ejercicio 1
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

	LPC_GPIO0 -> FIOCLR = (1<<22);
	delay(10);
	LPC_GPIO0 -> FIOSET = (1<<22);
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
 * Ejercicio 2:
 */
#include "LPC17xx.h"









