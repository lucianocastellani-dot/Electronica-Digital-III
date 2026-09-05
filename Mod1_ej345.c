#include <LPC17xx.h>


#define retardo_largo 500000;
#define retardo_corto 100000;
#define secuenciaA;
#define secuenciaB;

void configGPIO(void);


int main(void){

	configGPIO();

	while(1){

		secuenia_A();
		retardoLargo();
		secuencia_B();
		retardoLargo();

	}


}
void secuencia_A(void){
	// ROJO -> RETARDO CORTO -> VERDE -> RETARDO CORTO -> ROJO+VERDE -> RETORNO
	LPC_GPIO0 -> FIOCLR = 1 << 22;
	LPC_GPIO3 -> FIOSET = (1 << 25 | 1 << 26);
	retardoC();

	LPC_GPIO0 -> FIOSET = 1 << 22;
	LPC_GPIO3 -> FIOCLR = 1 << 25;
	LPC_GPIO3 -> FIOSET = 1 << 26;
	retardoC();

	LPC_GPIO0 -> FIOCLR = 1 << 22;
	LPC_GPIO3 -> FIOCLR = 1 << 25;
	LPC_GPIO3 -> FIOSET = 1 << 26;
	//EL AZUL YA ESTA APAGADO DESDE LA TERCER LINEA DEL BLOQUE
	retardoC();

	LPC_GPIO0 -> FIOSET = 1 << 22;
	LPC_GPIO3 -> FIOSET = (1 << 25 | 1 << 26);
	return;


}
void secuencia_B(void){
	// AZUL -> RETARDO CORTO -> AZUL + VERDE -> RETARDO CORTO -> ROJO+AZUL+VERDE -> RETORNO
	LPC_GPIO0 -> FIOSET = 1 << 22;
	LPC_GPIO3 -> FIOSET = 1 << 25;
	LPC_GPIO3 -> FIOCLR = 1 << 26;
	retardoC();

	LPC_GPIO3 -> FIOSET = 1 << 26;
	retardoC();

	LPC_GPIO3 -> FIOCLR = 1 << 26;
	LPC_GPIO0 -> FIOCLR = 1 << 22;
	retardoC();
	LPC_GPIO3 -> FIOSET = 1 << 25;
	LPC_GPIO3 -> FIOSET = 1 << 26;
	retardoC();

	LPC_GPIO0 -> FIOCLR = 1 << 22;
	LPC_GPIO3 -> FIOCLR = (1 << 25 | 1 << 26);
	retardoC();


}
void configGPIO(void){

	LPC_PINCON -> PINSEL1 &= ~(3u << 22); //CONFIGURO PINES COMO GPIO
	LPC_PINCON -> PINSEL7 &= ~(3u << 25);
	LPC_PINCON -> PINSEL7 &= ~(3u << 26);

	LPC_GPIO0 -> FIODIR |= (1 << 22); //PONGO PINES COMO SALIDA CON 1
	LPC_GPIO3 -> FIODIR |= (1 << 25);
	LPC_GPIO3 -> FIODIR |= (1 << 26);

	LPC_GPIO0 -> FIOSET = 1 << 22; //APAGO LOS LEDS CUANDO ARRANCO
	LPC_GPIO3 -> FIOSET = 1 << 25;
	LPC_GPIO3 -> FIOSET = 1 << 26;



}
int retardoL(){



}
int retardoC(){


}
