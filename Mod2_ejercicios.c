/*
 * Ejercicio 1: Escriba un programa muy sencillo que utilice un pin del Puerto 2 como interrupción externa (EINT0).
Cada vez que se active la interrupción (por flanco de bajada), un LED conectado a otro pin debe
cambiar de estado (encenderse si estaba apagado, y apagarse si estaba encendido).
Pines: P2.10 para boton y P0.22 para LED.
 */

/*
#include "LPC17xx.h"


int main(){
    //P0.10 como EINT0
	LPC_PINCON -> PINSEL4 &= ~(3u << 20);
	LPC_PINCON -> PINSEL4 |=  (1u << 20);
	//P0.22 como salida
	LPC_PINCON -> PINSEL1 &= ~(3u << 12);
	LPC_GPIO2  -> FIODIR  |= (1 << 22);


	LPC_SC -> EXTMODE |= (1 << 0); //INDICO QUE QUIERO DISPARO POR FLANCO, NO NIVEL
	LPC_SC -> EXTPOLAR &= ~(1 << 0); //INDICO DISPARO POR FLANCO DE BAJADA.


	NVIC_EnableIRQ(EINT0_IRQn);

	while (1){
		__WFI();
	}

}
void EINT0_IRQHandler(void){

	LPC_SC -> EXTINT = (1 << 0); //LIMPIO BANDERA

	LPC_GPIO0 -> FIOPIN ^= (1 << 22); //INVIERTO EL ESTADO DEL PIN

}
*/
/*
#include "LPC17xx.h"

// Mapa de 7 segmentos para los dígitos 0, 1, 2, ..., E, F
const uint8_t tabla[16] = {
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
volatile uint8_t contador = 0;
void configPIO();
int main(void){

	configPIO();
	NVIC_EnableIRQ(EINT3_IRQn);

	LPC_GPIO2 -> FIOSET = tabla[contador];


	while(1){

	}
}


void configPIO(void){
	LPC_PINCON -> PINSEL0  &= ~(3u << 0);
	LPC_GPIO0  -> FIODIR   &= ~(1  << 0);
	LPC_PINCON -> PINMODE0 |=  (3u << 0);

	LPC_PINCON -> PINSEL4 &= ~(0xFFFF); //configuro P2.0:P2.7 como GPIO
	LPC_GPIO2  -> FIODIR  |=  (0xFF);
	LPC_GPIOINT -> IO0IntEnR |= (1 << 0); //Flanco de subida

}
void EINT3_IRQHandler(void){

	LPC_GPIOINT ->  IO0IntClr = (1 << 0);

	contador ++;

	if (contador == 16){
		contador =0;
	}

	LPC_GPIO2 -> FIOCLR = (0xFF);

	LPC_GPIO2 -> FIOSET = tabla[contador];




}
*/
/*

#include "LPC17xx.h"

//EJERCICIO 3: CONTADOR HEX DECENDENTE, PERO CON EINT NO GPIO
volatile uint8_t sentido = 0;
volatile uint8_t avance =  1;
volatile uint8_t cuenta = 0;
void config(void);
void delay(void);

const uint16_t tabla[16]={
			0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
			0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
	};
int main(void){



	config();


	NVIC_EnableIRQ(EINT0_IRQn);


	while(1){
		if(sentido == 0){
			cuenta++;
			if (cuenta == 16){
				cuenta =0;

			}
		} else {
			if(cuenta == 0){
				cuenta = 15;
				}else{
					cuenta--;
				}
		 }
	}

}
void config(void){
	LPC_PINCON -> PINSEL4  &= ~(3u << 20);
	LPC_PINCON -> PINSEL4  |=  (1u << 20); //CONFIGURO COMO EINT0
	LPC_PINCON -> PINMODE4 &= ~(3u << 20); // ACTIVO PULL-UP
	//LPC_GPIO0  -> FIODIR   &= ~(1u << 0);  //P0.0

	LPC_SC 	   -> EXTINT   |=  (1 << 0); //LIMPIO BANDERA DE FORMA OBLIGATORIA
	LPC_SC 	   -> EXTMODE  |=  (1 << 0); //CONFIGURO POR FLANCO, NO NIVEL.
	LPC_SC     -> EXTPOLAR &= ~(1 << 0); //FLANCO DESCENDENTE


	LPC_PINCON -> PINSEL2 &= ~(0xFFFF);
	LPC_GPIO1  -> FIODIR  |=  (0xFF);



}
void EINT0_IRQHandler(void){



	while ((LPC_GPIO2 -> FIOPIN &= (1 << 10)) == 0){}
	LPC_SC -> EXTINT |= (1 << 0);
	sentido ^= (1 << 0);



}
void delay(void){
	for(int i = 0; i < 500000; i++){}
}

*/
/*
#include "LPC17xx.h"

void delay(void);
void config(void);
void secuencia_A(void);
void secuencia_B();
volatile uint8_t estado  = 0; //1=secuencia A, 0= secuecncia B

int main(void){
	config();


	NVIC_SetPriority(EINT1_IRQn,0);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT3_IRQn);

	while(1){
		if(estado == 0){
				secuencia_A();
		} else{
			    secuencia_B();
		}
	}
}
void config(){
	//P0.0 como GPIO
	LPC_PINCON   -> PINSEL0  &= ~(3u << 0);
	LPC_PINCON   -> PINMODE0 |=  (3u << 0); //PULL-DOWN
	LPC_GPIO0    -> FIODIR   &= ~(1 << 0); //ESTE SI SE ACLARA QUE ES ENTRADA
	LPC_GPIOINT  -> IO0IntEnR =  (1 << 0);

	//P2.11 como EINT1
	LPC_PINCON -> PINSEL4  &= ~(3u << 22);
	LPC_PINCON -> PINSEL4  |= (1u << 22);
	LPC_PINCON -> PINMODE4 &= ~(3u << 22); //PULL-UP

	LPC_SC -> EXTMODE  = (1 << 1); //FLANCO
	LPC_SC -> EXTPOLAR &=~(1 << 1); //FLANCO BAJADA
	LPC_SC -> EXTINT   = (1 << 1);

	//P0.16:P0.19 COMO SALIDAS LED
	LPC_PINCON   -> PINSEL1  &= ~(0xFFF); //BITS
	LPC_GPIO0	 -> FIODIR   |=  (0x0F << 16); //PINES



}

void EINT3_IRQHandler(void){
	//CONFIGURACION DE GPIO

	if(((LPC_GPIOINT -> IO0IntStatR)) & (1 << 0)){
		LPC_GPIOINT -> IO0IntClr = (1 << 0);
		estado = 0;
	}
}


void EINT1_IRQHandler(void){
	//CONFIGURACION DE EINT1
	LPC_SC -> EXTINT = (1 << 1);
	estado = 1;


}


void secuencia_A(void){
	LPC_GPIO0 -> FIOSET |= (0x0F << 16);
	delay();
	LPC_GPIO0 -> FIOCLR |= (0x0F << 16);
	delay();
}
void secuencia_B(void){
	LPC_GPIO0 -> FIOSET = (0x0A << 16);
	LPC_GPIO0 -> FIOCLR = (0x05 << 16);
	delay();
	LPC_GPIO0 -> FIOSET = (0x05 << 16);
	LPC_GPIO0 -> FIOSET = (0x0A << 16);
}

void delay(void){
	for(int i = 0; i< 500000; i++){}
}

*/

#include "LPC17xx.h"

void config(void);
void delay(void);
volatile uint8_t contador = 0;
int play = 1;
int main(){

	config();

	NVIC_IRQEnable(EINT3_IRQn);
	while(1){
		if(play ==1){
		LPC_GPIO2 -> FIOCLR = (0xFF);
		LPC_GPIO2 -> FIOSET = contador;
		delay();
		contador++;
		} else{}
	}
}
void config(void){
	LPC_PINCON  -> PINSEL0   &= ~(3u<<0);
	LPC_PINCON  -> PINMODE0  &=~(3u<<0);
	LPC_GPIO0   -> FIODIR    &= ~(1<<0); //P0.0 como entrada
	LPC_GPIOINT -> IO0IntEnF |=(1<<0);

	LPC_PINCON ->  PINSEL4 &= ~(0xFFFF);
	LPC_GPIO2  ->  FIODIR |= (0xFF);
}
void EINT3_IRQHandler(void){

	if((LPC_GPIOINT -> IO0IntStatF)&(1<<0)){
		LPC_GPIOINT -> IO0IntClr = (1<<0);
		play ^= (1);
	}
}
void delay(void){
	for(int i = 0; i < 500000; i++){}
}






