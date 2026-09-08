#include "LPC17xx.h"

volatile uint32_t ticks = 0;

void config();
void configSysTick();
void SysTick_Handler();
void EINT3_IRQHandler();

int main (void){
	config();
	configSysTick();
	NVIC_EnableIRQ(EINT3_IRQn);
	LPC_GPIO0 -> FIOCLR = (0x3F << 1); // Me aseguro que todo esté apagado
	LPC_GPIO0 -> FIOSET = (1u << 1) | (1u << 6);//Estado inicial, prendo Rojo S1 y Verde S2.
	while(1){

	}
}

void config(void){
	//ENTRADAS
	LPC_PINCON -> PINSEL0 &= ~(0x3FFF << 0); //P0.0 a P0.3 como GPIO
	LPC_PINCON -> PINMODE0 &= ~(3u << 0);//Pull-Up
	LPC_GPIO0 -> FIODIR &= ~(1u << 0); //Entrada
	LPC_GPIOINT -> IO0IntEnF |= (1u << 0);//Activo interrupción por flanco de bajada

	//SALIDAS
	LPC_GPIO0 -> FIODIR |= (0x3F << 1); // ROJO = P0.1 y P0.4 | AMARILLO = P0.2 y P0.5 | VERDE = P0.3 y P0.6
}

void configSysTick(void){
	SysTick -> LOAD = (SystemCoreClock/1000) - 1; //1ms
	SysTick -> VAL = 0;
	SysTick -> CTRL = (7u << 0); //Clck interno | Excepción | Enable
}

void SysTick_Handler(void){
	ticks++;
	if(ticks == 25000){
		LPC_GPIO0 -> FIOCLR = (1u << 6); //Apago verde semaforo 2
		LPC_GPIO0 -> FIOSET = (1u << 5); //Prendo amarillo semaforo 2
	}
	if(ticks == 30000){
		LPC_GPIO0 -> FIOCLR = (1u << 1); //Apago rojo semaforo 1
		LPC_GPIO0 -> FIOCLR = (1u << 5); //Apago amarillo semaforo 2
		LPC_GPIO0 -> FIOSET = (1u << 3); //Prendo verde semaforo 1
		LPC_GPIO0 -> FIOSET = (1u << 4); //Prendo rojo semaforo 2
	}
	if(ticks == 55000){
		LPC_GPIO0 -> FIOCLR = (1u << 3);//Apago verde semaforo 1
		LPC_GPIO0 -> FIOSET = (1u << 2); //Prendo amarillo semaforo 1
		LPC_GPIO0 -> FIOSET = (1u << 4); //Prendo rojo semaforo 2
	}
	if(ticks == 60000){
		LPC_GPIO0 -> FIOCLR = (1u << 2); //Apago amarillo semaforo1
		LPC_GPIO0 -> FIOCLR = (1u << 4);//Apago rojo semaforo 2
		LPC_GPIO0 -> FIOSET = (1u << 1); //Prendo rojo semaforo 1
		LPC_GPIO0 -> FIOSET = (1u << 6); //Prendo verde semaforo 2

		ticks=0;
	}
}
void EINT3_IRQHandler(void){
	if((LPC_GPIOINT -> IO0IntStatF) & ( 1<< 0)){
		LPC_GPIOINT -> IO0IntClr = (1 <<0); //limpio bandera
		LPC_GPIO0 -> FIOCLR = (0x3F << 1);//Apago semaforos
		LPC_GPIO0 -> FIOSET = (1u << 2) | (1u << 4); //Prendo amarillo S1 y rojo S2
		ticks = 0;
		ticks = 55000;//Voy al estado de amarillo y rojo
	}
}
