/*
 *EJERCICIO 2: TOGGLE LED CON TIMER0
 */
/*
#include "LPC17xx.h"

void config();
void configTimer();
int main(){
	config();
	configTimer();


	NVIC_EnableIRQ(TIMER0_IRQn);
	while(1){}
}
void config(void){
	//HABILITO EL MAT0.0 COMO SALIDA PARA EL LED
	LPC_PINCON -> PINSEL3 |= (3u << 24);

}
void configTimer(void){
	LPC_SC -> PCONP |= (1 << 1);
	//Precarga para que TC AVANCE CON PASOS DE 1 ms
	LPC_TIM0 -> PR  = 24999;
	LPC_TIM0 -> MR0 = 1000 - 1; //medio periodo de 1 segundo
	LPC_TIM0 -> MCR = (3u << 0); //interrumpo y reseteo
	LPC_TIM0 -> TCR = (1u << 1);
	LPC_TIM0 -> TCR = (1u << 0);
	//CON ESTO LOGRO UN PERIODO DE 1 s EN ALTO Y OTRO EN BAJO
}
void TIMER0_IRQHandler(void){
	LPC_TIM0 -> IR = (1 << 0);
	//Hago el toggle con EMR (pongo 11 en bits 4 y 5)
	LPC_TIM0 -> EMR |= (3u << 4);
}
*/
/*
 * EJERCICIO 3
 */
/*

#include "LPC17xx.h"
#define   MR0_base 1000000
volatile  uint8_t frec = 0;

void config();
void configTimer();
void EINT3_IRQHandler();
void TIMER0_IRQHandler();

int main(){
	config();
	configTimer();


	//NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(EINT3_IRQn);
	//NVIC_SetPrority(EINT3_IRQn,0);
	//NVIC_SetPriority(TIMER0_IRQn,1);
	while(1){}
}
void config(void){

	LPC_PINCON  -> PINSEL0   &= ~(3u << 0);
	LPC_PINCON  -> PINSEL0   |=  (1u << 0);
	LPC_PINCON  -> PINMODE0  &= ~(3u << 0);
	LPC_GPIO0   -> FIODIR    &= ~ (1 << 0);
	LPC_GPIOINT -> IO0IntEnF  =   (1 << 0);

	LPC_PINCON -> PINSEL3 |= (3u << 24);

}

void EINT3_IRQHandler(void){
	if((LPC_GPIOINT->IO0IntStatF)&(1<<0)){

		LPC_GPIOINT -> IO0IntClr = (1 << 0);

		// ACA TENGO QUE CAMBIAR LA FRECUENCIA
		frec++;
		if(frec>10){
			frec = 0;
		}

		LPC_TIM0 -> MR0 = (MR0_base >> frec) - 1;
		// Correr a la derecha es lo mismo que dividir por dos

	}
}
/*
void TIMER0_IRQHandler(void){
	LPC_TIM0 -> IR  = (1 << 0);
}
void configTimer(void){
	LPC_SC -> PCONP = (1 << 1);

	LPC_TIM0 -> PR  = 249999;
	LPC_TIM0 -> MR0 = 1000 - 1;
	LPC_TIM0 -> MCR = (2u << 0); //hago que resetee pero no interrumpa

	LPC_TIM0 -> TCR = (2u << 0);
	LPC_TIM0 -> TCR = (1u << 1);

	LPC_TIM0 -> EMR = (3u << 4);
}
*/

/*
 * EJERCICIO 4
 */
/*
#include "LPC17xx.h"

void EINT3_IRQHandler();
void config();
void configTimer2();



int main(){

	config();
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
	configTimer2();

	while(1){}

}
void config(void){
	LPC_PINCON -> PINSEL4 &= ~(3u << 20);
	LPC_PINCON -> PINSEL4  |= (1u << 20); //configuro EINT0
	LPC_SC -> EXTINT   |=  (1<<0);
	LPC_SC -> EXTMODE  |=  (1<<0);
	LPC_SC -> EXTPOLAR &= ~(1<<0);

	LPC_PINCON -> PINSEL0 &= ~(3u << 0); //configuro GPIO para salida
	LPC_GPIO0 ->  FIODIR 	|=(1<<0);
}
void configTimer2(void){

	LPC_SC -> PCONP |= (1 << 22); //PRENDO EL TIMER2

	LPC_TIM2 -> PR  = 24999; //TC CUENTA DE A 1 ms
	LPC_TIM2 -> MR0 = 1000 - 1; //Tiempo total
	LPC_TIM2 -> MR1 = 500 - 1; //Periodo en ALTO
	LPC_TIM2 -> MCR = ((1u << 0)| (1u << 1)| (1u << 3)); //001 en MR0 y 001 en MR1
	LPC_TIM2 -> TCR =  (1u << 1) | (1u << 0);
}

void TIMER2_IRQHandler(void){

	if((LPC_TIM2 -> IR) & (1 << 0)){
		LPC_TIM2 -> IR = (1 << 0);
		//COMO TERMINO MI PERIODO PRENDO EL P0.0
		LPC_GPIO0 -> FIOSET = (1<<0);
		}
	if((LPC_TIM2 -> IR)&(1 << 1)){
		LPC_TIM2 -> IR = (1 << 1);
		//COMO CUMPLO MI DUTY CYCLE, APAGO EL P0.0
		LPC_GPIO0 -> FIOCLR = (1 << 0);
	}

}

void EINT0_IRQHandler(void){
	LPC_SC -> EXTINT = (1 << 0);

	LPC_TIM2 -> MR1 +=100 - 1;
	if((LPC_TIM2->MR1) > 1000){
		LPC_TIM2 -> MR1 = 500 -1;
	}

}
*/
/*
#include "LPC17xx.h"

volatile uint8_t contador = 0;

void configTimer3();
void config();
void TIMER3_IRQHandler();

int main(){

	config();
	configTimer3();

	NVIC_EnableIRQ(TIMER3_IRQn);
	while(1){}
}
void config(void){

	//P0.0:P0.3 como salidas
	LPC_PINCON -> PINSEL0 &= ~(0xFF);
	LPC_GPIO0  -> FIODIR  |= (0x0F);
	LPC_GPIO0  -> FIOCLR  |= (0x0F);

	LPC_PINCON -> PINSEL1  &= ~(3u << 14);
	LPC_PINCON -> PINSEL1  |=  (3u << 14); // USO CAP3.0 PARA CAPTURAR EL BOTON
	LPC_PINCON -> PINMODE1 &= ~(3u << 0);  //activo las pull up


}


void configTimer3(void){
	LPC_SC -> PCONP = (1 << 23);

	LPC_TIM3 -> PR  = 24999999; //TC AVANZA CADA 1 s
	//LPC_TIM3 -> MR0 = 100 - 1; EL EJERCICIO NO ME PONE UN LIMITE DE CONTEO
	LPC_TIM3 -> MCR = (1 << 1); //010 EN EL MR0 (STOP:0 RESET:1 INTERRUPT:0)
	LPC_TIM3 -> TCR = 2;
	LPC_TIM3 -> TCR = 1;
	LPC_TIM3 -> CCR = (1 << 1)|(1 << 2); //ME INTERESA LA CAPTURA EN EL FLANCO DE BAJADA
}
void TIMER3_IRQHandler(void){
	if((LPC_TIM3 -> IR) & (1 << 4)){ //pregunto si la I la genero la captura

		LPC_TIM3 -> IR = (1 << 4);
		contador = LPC_TIM3 -> CR0;

		if(contador > 15){
			contador = 0;
		}
		LPC_GPIO0 -> FIOCLR = 0x0F;
		LPC_GPIO0 -> FIOSET = contador;


		LPC_TIM3 -> TCR = (1u << 1);
		LPC_TIM3 -> TCR = (1u << 0);

	}
}
*/
#include "LPC17xx.h"

void config();
void EINT0_IRQHandler();
void configTimer0();


int main(){

	config();
	configTimer0();

	NVIC_EnableIRQ(EINT0_IRQn);

	NVIC_SetPriority(EINT0_IRQn,0);
	NVIC_SetPriority(TIMER0_IRQn,1);
	//INICIO CON SEM1 EN ROJO Y SEM2 EN VERDE
	LPC_GPIO0 -> FIOCLR = (0x3F << 0);

	LPC_GPIO0 -> FIOSET = (1 << 2)|(1 << 3);
	while(1){}

}

void config(void){
	//BOTON DE PASO COMO EINT
	LPC_PINCON -> PINSEL4  &= ~(3u << 20);
	LPC_PINCON -> PINSEL4  |=  (1u << 20);
	LPC_PINCON -> PINMODE4 |=  (3u << 20);//ACTIVO PULL UP
	LPC_SC -> EXTINT    =  (1 << 0);
	LPC_SC -> EXTMODE  |=  (1 << 0);
	LPC_SC -> EXTPOLAR &= ~(1 << 0);

	//SALIDAS DE LOS SEMAFOROS EN P0.0:P0.5
	LPC_PINCON -> PINSEL0 &= ~(0xFFF);
	LPC_GPIO0  -> FIODIR  |=  (0x3F);

}
void configTimer0(void){
	LPC_TIM0 -> PR  = 24999999; //TC AVANZA EN PASOS DE 1s
	LPC_TIM0 -> MR0 = 25-1;
	LPC_TIM0 -> MR1 = 30-1;
	LPC_TIM0 -> MR2 = 55-1;
	LPC_TIM0 -> MR3 = 60-1;

	LPC_TIM0 -> MCR = (3u << 9)|(1u << 6)|(1u << 3)|(1u << 0); //cargo 011 en MR3 y 001 MR1 MR2 MR3

	LPC_TIM0 -> TCR = (1u << 1);
	LPC_TIM0 -> TCR = (1u << 0);

}
void EINT0_Handler(void){

	LPC_SC -> EXTINT = (1<<0);

	LPC_GPIO0 -> FIOCLR = (0x3F);
	LPC_GPIO0 -> FIOSET |= (1<<1)|(1<<5);

	LPC_TIM0 -> TC = 55; //HAGO QUE EL TC EMPIEZE A CONTAR EN 55;


}
void TIMER0_Handler(void){
	/*
	 * P0.0 = VERDE SEM1
	 * P0.1 = AMARILLO SEM1
	 * P0.2 = ROJO SEM1
	 * P0.3 = VERDE SEM2
	 * P0.4 = AMARILLO SEM2
	 * P0.5 = ROJO SEM2
	 */

	if((LPC_TIM0 -> IR) & (1 << 0)){ //entro solo si estoy en 25s

		LPC_TIM0 -> IR = (1<<0); //LIMPIO BANDERA
		//APAGO VERDE SEM2, PRENDO AMARILLO SEM2
		LPC_GPIO0 -> FIOCLR = (1 << 3);
		LPC_GPIO0 -> FIOSET = (1 << 4);

	}
	if((LPC_TIM0 -> IR) & (1<<1)){

		LPC_TIM0 -> IR = (1<<1);
		//APAGO ROJO SEM1 Y PRENDO VERDE SEM1
		LPC_GPIO0 -> FIOCLR = (1<<2);
		LPC_GPIO0 -> FIOSET = (1<<0);
		//APAGO AMARILLO SEM2 PRENDO VERDE ROJO SEM2
		LPC_GPIO0 -> FIOCLR = (1<<4);
		LPC_GPIO0 -> FIOSET = (1<<5);
	}
	if(((LPC_TIM0->IR))&(1<<2)){

		LPC_TIM0 -> IR = (1<<2);
		//APAGO VERDE SEM1, PRENDO AMARILLO SEM1
		LPC_GPIO0 -> FIOCLR = (1<<0);
		LPC_GPIO0 -> FIOSET = (1<<1);

	}
	if((LPC_TIM0->IR)&(1<<3)){

		LPC_TIM0 -> IR = (1<<3);

		//APAGO AMARILLO SEM1, PRENDO ROJO SEM1
		LPC_GPIO0 -> FIOCLR = (1<<1);
		LPC_GPIO0 -> FIOSET = (1<<2);

		//APAGO ROJO SEM2, PRENDO VERDE SEM2
		LPC_GPIO0 -> FIOCLR = (1<<5);
		LPC_GPIO0 -> FIOSET = (1<<3);
	}
}
