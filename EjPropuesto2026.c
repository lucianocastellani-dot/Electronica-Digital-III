#include "LPC17xx.h"


void config();
void configEINT();
void configSysTick();
void delay();
void SysTick_Handler();
void EINT0_IRQHandler();
void EINT1_IRQHandler();
void configTimer0();
uint8_t secuencia[15] ={0,0,0,0,1,0,1,0,1,0,0,0,0,0};


int main(){
	config();
	configEINT();
	configSysTick();

	NVIC_EnableIRQ(EINT0_IRQn,EINT1_IRQn);
	NVIC_SetPriority(EINT0_IRQn,0,EINT1_IRQn,1);

	while(1){
		__WFI();
	}
}

void config(){
	LPC_PINCON -> PINSEL4 &= ~(3u<<2); //LIMPIO EL P2.1
	LPC_GPIO2  -> FIODIR |=  (1u<<1);
	LPC_GPIO2  -> FIOCLR  =  (1u<<1); //ESRTADO INCIAL DEL P2.1 EN BAJO


	LPC_PINCON  -> PINSEL4 &= ~(0xF<<20); //LIMPIO P2.20 : P2.23
	LPC_PINCON  -> PINSEL4 |=  (1u <<20) |(1u<<22); //ACTIVO EINT0
	LPC_PINCON  -> PINSEL4 |=  (1u <<22); //ACTIVO EINT1
}
void configSysTick(void){
	SysTick -> LOAD = (SystemCoreClock/1000)-1;
	SysTick -> VAL = 0;
	SysTick -> CTRL = (1<<0)|(1<<1)|(1<<2);
}
void SysTick_Handler(void){
	ticks++;
}
void configTimer0(void){
	LPC_SC -> PCONP = 1 << 1; //ENCIENDO EL TIMER0

	LPC_TIM0 -> PR = 24999; //TC CON PASOS DE 1ms

	LPC_TIM0 -> MR0 = 75-1; //PERIODO TOTAL DE LA SEÑAL
	LPC_TIM0 -> MR1 = 5-1;
	LPC_TIM0 -> MR2 = 10-1;
	LPC_TIM0 -> MR3 = 20-1;

	LPC_TIM0 -> MCR = (1u<<0); //ENABLE(0) = 1; RESET(1) = 0; STOP(2)= 0
	LPC_TIM0 -> TCR = (1u<<1);
	LPC_TIM0 -> TCR = (1u<<0);
}
void configEINT(void){
	//CONFIGURO EINT0
	LPC_SC -> EXTINT = (1<<0); //LIMPIO BANDERA
	LPC_SC -> EXTPOLAR |= (1<<0); //ACTIVO INT POR FLANCO
	LPC_SC -> EXTMODE |= (1<<0); //FLANCO ASCENDENTE

	//CONFIGURO EINT1
	LPC_SC -> EXTINT = (1<<1);
	LPC_SC -> EXTMODE = (1<<0);
	LPC_SC -> EXTPOLAR = (1<<1);

}

void EINT0_IRQHandler(void){
	LPC_SC -> EXTINT = (1u<<1);

	volatile uint32_t tiempo = ticks;
	while((ticks-tiempo) < delay(140000)){

		for(int i = 0; i < 15;i++){
			LPC_GPIO2 -> FIOCLR = (1u<<1);
			LPC_GPIO2 -> FIOSET = (secuencia[i]<<1);
			delay(10000);
		}

     }
}
void EINT1_IRQHandler(void){

	LPC_SC -> EXTINT = (1u<<0);

	LPC_GPIO2 -> FIOCLR = (1u<<0);
	while((LPC_TIM0 -> TC)<(75000)){
		for(int i = 0; i<15;i++){
			LPC_GPIO2 -> FIOCLR = (1u<<1);
			LPC_GPIO2 -> FIOSET = secuencia[i]<<1;
			delay(5000);
		}
	}
}
void delay(uint32_t ms){
	volatile uint32_t tiempo = ticks;
	while((ticks-tiempo)<ms){}
}

