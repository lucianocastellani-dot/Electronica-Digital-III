#include "LPC17xx.h"

#define delay 300000
void confiGPIO (void);
void retardo(void);
int main(){

	confiGPIO();
	while(1){
/*
		LPC_GPIO1 -> FIOCLR |= 1 << 22;
		retardo();
		LPC_GPIO1 -> FIOSET |= 1 << 22;
		retardo();
		LPC_GPIO1 -> FIOCLR |= 1 << 22;
		retardo();
		LPC_GPIO4 -> FIOCLR |= 1 << 25;
		retardo();
		LPC_GPIO4 -> FIOSET |= 1 << 25;
		retardo();
		LPC_GPIO4 -> FIOCLR |= 1 << 25;
		retardo();
		LPC_GPIO4 -> FIOCLR |= 1 << 26;
		retardo();
		LPC_GPIO4 -> FIOSET |= 1 << 26;
		retardo();
		LPC_GPIO4 -> FIOCLR |= 1 << 26;
*/
		LPC_GPIO0 -> FIOPIN ^= 1 << 22; //esto hace lo mismo que lo re arriba pero con una XOR
		retardo();
		LPC_GPIO3 -> FIOPIN ^= 1 << 25;
		retardo();
		LPC_GPIO3 -> FIOPIN ^= 1 << 26;
		retardo();
	}

}

void confiGPIO(void){

	LPC_PINCON -> PINSEL1 &= ~(3u << 12); //LED ROJO
	LPC_PINCON -> PINSEL7 &= ~(3u << 20); //LED AZUL
	LPC_PINCON -> PINSEL7 &= ~(3u << 18); //LED VERDE
/*
	LPC_PINCON -> PINMODE1 &= ~(1 << 13);
	LPC_PINCON -> PINMODE1 &= ~(0 << 12);

	LPC_PINCON -> PINMODE7 &= ~(1 << 21);
	LPC_PINCON -> PINMODE7 &= ~(0 << 20);

	LPC_PINCON -> PINMODE7 &= ~(1 << 19);
	LPC_PINCON -> PINMODE7 &= ~(0 << 18);
*/

	LPC_GPIO1 -> FIODIR |= (1 << 22);
	LPC_GPIO4 -> FIODIR |= (1 << 26);
	LPC_GPIO4 -> FIODIR |= (1 << 25);



}
void retardo(void){

	for (volatile uint32_t i = 0; i <= delay; i++){

	}

}


