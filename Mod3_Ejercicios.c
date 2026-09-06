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













