/*
 * EJERCICIO 1: SYSTICK E EINT
 */
/*
#include "LPC17xx.h"


void config();
void configSysTick();
void configEINT();
volatile uint8_t contador = 0;
int bandera = 0, bandera_eint0 = 0, bandera_eint1 = 0, badnera_eint2 = 0;
const uint8_t display[9] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
int main(){
	config();
	configSysTick();
	configEINT();


	NVIC_Enable(EINT0_IRQn, EINT1_IRQn, EINT2_IRQn); //HABILITO LAS EINT
	//ORDENO PRIORIDADAES
	//PRIORIZO EL RESET, DESPUES LA PAUSA Y POR ULTIMO EL INCREMENTO EN FRECUENCIA
	NVIC_SetPriority(EINT0,0);
	NVIC_SetPriority(EINT1,1);
	NVIC_SetPriority(EINT2,2);
	while(1){
		if(contador > 9){
			contador = 0;
		}
		if(banderaTicks == 1){
			//SI LA BADNERA ESTÁ EN 1, SYSTICK INTERRUMPIÓ Y YA PASÓ 1 SEGUNDO, AUMENTO EL CONTEO
			contador++;
			LPC_GPIO0 -> FIOCLR = (0xFF);
			delay(1000); //DELAY DE 1 SEGUNDO
			LPC_GPIO0 -> FIOSET = display[contador];
		}

		if(bandera_eint0 == 1){
			//ACA RESETEO EL CONTADOR Y ME MANTENGO EN ESE VALOR MIENTRAS PULSADOR ESTE PRESIONADO
			contador = 0;
			LPC_GPIO0 -> FIOCLR = (0xFF);
			LPC_GPIO0 -> FIOSET = display[contador];
			//PREGUNTO SI EL BOTON SIGUE PRESIONADO, SI ES ASI, MUESTRO LO QUE VENIA MOSTRANDO
			if((LPC_SC -> FIOPIN >> 10) & (1<<0)){
				LPC_GPIO0 -> FIOCLR = 0xFF;
				LPC_GPIO0 -> FIOSET = display[contador];
			}

		}
		if(bandera_eint1 == 1){
			//ACA ME MANTENGO EN PAUSA, NO AUMENTO NI RESETEO EL CONTADOR
			LPC_GPIO0 -> FIOCLR = (0xFF);
			LPC_GPIO0 -> FIOSET = display[contador];
		}
	}
}

void config(void){
	LPC_PINCON -> PINSEL4 &= ~(0xFFF);
	LPC_PINCON -> PINSEL4 |= (1u << 20) | (1u << 22) | (1u << 24); //HABLITO EINT0, 1 Y 2
	LPC_PINCON -> PINMODE4 &= ~(1u << 22)|(1u << 24); // PULL UP PARA EINT1 Y 2 XQ SON BAJAS
	LPC_PINCON -> PINMODE4 |= (3u << 20);// PULL DOWN PARA EINT0
	//SALIDAS
	LPC_PINCON -> PINSEL0 &= ~(0x3FFF);
	LPC_GPIO0  -> FIODIR |= (0x3F);

}
void configEINT(void){

	LPC_SC -> EXTINT   = (1u << 0) | (1u << 1) | (1u << 2);
	LPC_SC -> EXTMODE  &= ~ (1u << 0) | (1u << 1) | (1u << 2); //TODAS POR FLANCO, NO NIVEL
	LPC_SC -> EXTPOLAR |= (1u << 0); //EINT0 CON FLANCO DE SUBIDA
	LPC_SC -> EXTPOLAR &= ~ ((1u << 1) | (1u << 2)); //EINT1 Y 2 CON FLANCO DE BAJADA

}

void configSysTick(void){
	SysTick -> LOAD = 16000000-1; //CUENTO 1 s, ESO PORQUE MI CLK ES DE 16 MHz
	SysTick -> VAL  = 0;
	SysTick -> CTRL = (1 << 0) | (1 << 1); //HABILITO-PERMITO INT-USO EL CLK INTERNO

}
void SysTick_Handler(void){
	ticks++;
	banderaTicks = 1;

}
void delay(uint16_t ms){
	volatile uint16_t tiempo = ticks;

	while ((ticks-tiempo) < ms){	}
}
void EINT0_IRQHandle(void){
	LPC_SC -> EXTINT = (1 << 0);
	bandera_eint0 = 1;
}
void EINT1_IRQHandler(void){
	LPC_SC -> EXTINT = (1 << 1),
	bandera_eint1 = 1;
}
void EINT2_IRQHandler(void){
	LPC_SC -> EXTINT = (1 << 2);
	//CAMBIO LA CARGA DEL SYSTICK PARA QUE CUENTE CADA 1 ms
	SysTick ->LOAD = 15999;
}
*/
/*
 * EJERCICIO 2: GPIOs
 */
#include "LPC17xx.h"

void config();
void configEINT();
void EINT3_IRQHandler();
void deshabilitar_int();
volatile uint8_t array[10];
int cant_int = 0;
int prioridad = 0;
int main(){
	config();
	configEINT();
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT3_IRQn,prioridad);
	while(1){
		__WFI();
	}
}
void config(void){
	LPC_PINCON -> PINSEL0  &= ~(0xFF); // LIMPIO PUERTO
	LPC_PINCON -> PINMODE0 |=  (0xFF); //ACTIVO PULL UP
	LPC_GPIO0  -> FIODIR   &= ~(0xF); //PONGO COMO ENTRADAS EL PUERTO

}
void configEINT(void){
	LPC_GPIOINT -> IO0IntClr = (1<<0)|(1<<1)|(1<<2)|(1<<3);  //LIMPIO BANDERAS
	LPC_GPIOINT -> IO0IntEnR = (1u<<0)|(1<<1)|(1<<2)|(1<<3); //PONGO FLANCO DE SUBIDA
}
void EINT3_IRQHandler(void){
	LPC_GPIOINT -> IO0IntClr = (1 << 0) | (1<<1) | (1<<2) | (1<<3);
	cant_int++;
	volatile uint8_t numero = LPC_GPIO0 -> FIOPIN & 0x0F; //LEO LOS VALORES DE P0.0 A P0.3
	deshabilitar_int();
	for(int i = 9; i > 0; i--){
		array[i]= array[i-1]; // CORRO EL VALOR ANTERIOR AL ACTUAL
	}
	array[0]   = numero; //REEMPLAZO EL VALOR DEL ELEMENTO 0 POR EL NUEVO
}
void deshabilitar_int(void){
	if (cant_int  == 200){
		cant  = 0;
		prioridad++;
			if(prioridad < 32){
				NVIC_SetPriority(EINT3_IRQn,prioridad);
			}else{
				NVIC_DisableIRQ(EINT3_IRQn);
			}
	}
	return;
}








