/*
#include "LPC17xx.h"
void ventana();
void delay();
void config();
void configSysTick();
volatile uint32_t ticks = 0;
const uint32_t CCLK = 70000000;
volatile uint8_t contador = 0;
int ticketValido = 0;
int main(){
	config();
	configSysTick();

	NVIC_EnableIRQ(EINT3_IRQn);
	while(1){
		if(ticketValido == 1){
			LPC_GPIO0 -> FIOCLR = 1 << 5;
			ventana();
		}else{

			LPC_GPIO0 -> FIOSET = 1 << 5;
		}
	}
}
void config(void){
	//CONFIGURO PIN
	LPC_PINCON   -> PINSEL4  &= ~(3u << 8);
	LPC_GPIO2    -> FIODIR   &= ~(1 << 4); //P2.4 ENTRADA
	LPC_PINCON   -> PINMODE2 &= ~(3u<<0);  //PULL UP
	//CONFIGURO LA EINT POR GPIO
	LPC_GPIOINT -> IO2IntClr = (1<<4); //LIMPIO BANDERA
	LPC_GPIOINT -> IO2IntEnF = (1<<4); //FLANCO DE BAJADA

	//SALIDA DE BARRERA EN EL P0.15
	LPC_PINCON -> PINSEL0 &= ~(3u << 0);
	LPC_GPIO0  -> FIODIR  |= (1u << 0);

	//SALIDA EN LED P1.5
	LPC_PINCON -> PINSEL2 &= ~(3u << 10);
	LPC_GPIO1  -> FIODIR   |= (1u << 5);
	//ENTRADA EN P2.0
	LPC_PINCON -> PINSEL4  &= ~(3u << 0);
	LPC_PINCON -> PINMODE4 &= ~(1u << 0);
	LPC_GPIO2  -> FIODIR   &=~(1u << 0);

}

void configSysTick(void){
	SysTick -> LOAD = (CCLK/1000) - 1; //cuento de a 1ms
	SysTick -> VAL = 0;
	SysTick -> CTRL = (1<<0)|(1<<1); //ENABLE = 1 ; RESET = 1 ; CCLK = 0

}
void SysTick_IRQHandler(void){
	ticks++;
}
void delay(uint16_t ms){
	volatile uint16_t tiempo = ticks;
	while((ticks-tiempo)<ms){}
}
void EINT3_IRQHandler(void){
	if((LPC_GPIOINT -> IO2IntStatF)&(1<<4)){
		LPC_GPIOINT -> IO2IntClr = (1<<4);
		ticketValido = 1;
	}
}
void ventana(void){
	contador = 0;
	volatile uint32_t tiempo = ticks;
	while((ticks-tiempo)<3000){//VENTANA DE 3 s

	if((LPC_GPIO2 -> FIOPIN) >> & (1<<0)){
		contador++;
		if(contador > 4){contador = 0;}

		if(contador == 0 || contador == 4){
			LPC_GPIO0 -> FIOSET = 1 <<15;
			delay(5000);}
		if(contador == 1){
			LPC_GPIO0 -> FIOSET = 1 <<15;
			delay(10000);}
		if(contador == 2){
			LPC_GPIO0 -> FIOSET = 1 <<15;
			delay(20000);}
		if(contador == 3){
			LPC_GPIO0 -> FIOSET = 1 <<15;
			delay(40000);}
		}
	}
}
*/
#include "LPC17xx.h"

 void config();
 void delay();
 void configSysTick();
 void SysTick_Handler();
 void ventana();
int banderaPuerta = 0;
volatile uint32_t ticks = 0;
 int main(){

	 config();
	 configSysTick();

	 NVIC_EnableIRQ(EINT3_IRQn);
	 while(1){
		 if(banderaPuerta == 1){
			 ventana();
			 banderaPuerta = 0;
		 }else{
			 LPC_GPIO2 -> FIOCLR = (0xF);
		 }
	 }
 }
 void config(void){
	 //CONFIGURO LA EINT DEL PUERTO 0 COMO GPIO
	 LPC_PINCON -> PINSEL0 &= ~(3u << 12);
	 LPC_GPIO0  -> FIODIR  &= ~ (1 << 6);
	 //CONFIGURO ENTRADAS DEL P2
	 LPC_PINCON -> PINSEL4  &= ~(0xFF);
	 LPC_GPIO2  -> FIODIR   &= ~(0xF);
	 LPC_PINCON -> PINMODE4 &= ~(3u << 0);
	 //CONFIGURO SALIDA EN P1.11 PERO DUDO, P1.11 ESTÁ RESERVADO
	 LPC_PINCON -> PINSEL2  &= ~(3u << 22);
	 LPC_GPIO1  -> FIODIR   |= (1u << 11);

	 LPC_GPIOINT -> IO0IntClr = (1<<6);   //LIMPIO BANDERA
	 LPC_GPIOINT -> IO0IntEnF = (1<<6);	  //ACTIVO POR FLANCO DE BAJADA

 }

 void configSysTick(void){

	 SysTick -> LOAD = (SystemCoreClock/1000) - 1; //PASOS DE 1 s
	 SysTick -> VAL = 0;
	 SysTick -> CTRL = (1<<0)|(1<<1)|(1<<2); //ENABLE = 1; RESET = 1; CCLK = 1
 }

 void SysTick_Handler(void){
	 ticks++;
 }
 void delay(uint32_t ms){
	 volatile uint32_t tiempo = ticks;
	 while((ticks - tiempo) < ms){	 }
 }
void EINT3_IRQHandler(void){
	LPC_GPIOINT -> IO0IntClr = (1<<6);
	banderaPuerta = 1;

}
void ventana(void){
	int vecesCodigo = 0;
	volatile uint32_t tiempo = ticks;
	while((ticks - tiempo) < delay(30000)){

		if((LPC_GPIO2->FIOPIN & 0x0F) == (0x0A)){ //SI EL CODIGO ES CORRECTO RETORNO
			LPC_GPIO1 -> FIOCLR = (1u<<11);
			return;
		}else{
			vecesCodigo++;
		    if(vecesCodigo >= 2){
			LPC_GPIO1 ->FIOSET = (1 << 11);
		    }
		}
		//ESTO ME DA LUGAR A CAMBIAR EL CODIGO MIENTRAS SUENA LA ALARMA
		if (vecesCodigo > 2 && ((LPC_GPIO2 -> FIOPIN&(0x0F))==(0x0A))){
			vecesCodigo = 0;
			LPC_GPIO1 -> FIOCLR = (1<<11);
		}
	  }
	LPC_GPIO1 -> FIOSET = (1u<<11); //SI EL CODIGO NO SE PUSO EN 30s SUENA LA ALARMA
}
