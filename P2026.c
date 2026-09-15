#include "LPC17xx.h"

void config();
void conf_timer0_pwm();

int banderaFalla = 0;
volatile uint8_t duty_actual = 0;
volatile uint8_t periodo = 0;
uint16_t perfil[2][10] = {
    {10, 30, 60, 85, 100, 100, 70, 40, 20, 0},//ciclo de trabajo
    {300, 450, 600, 600, 1200, 900, 600, 450, 300, 0}//Tiempo
};

int main(){
	config();
	conf_timer0_pwm();


	NVIC_EnableIRQ(EINT0_IRQn,TIMER0_IRQn);
	NVIC_SetPriority(EINT0_IRQn,0,TIMER0_IRQn,1);

	for(int i = 0; i<10; j++){

		if(banderaFalla == 1){break;} //SI SE APRETÓ EL BOTON DE FALLA ROMPO EL FOR

		duty_actual = (perfil[0][i]);
		periodo = (perfil[1][i]);



		if(periodo == 0){break;}

		conf_timer0_pwm(duty_actual);

	}

	LPC_TIM0 -> TCR = 0; //APAGO TIMER0
	LPC_GPIO2-> FIOCLR = (1<<0); //APAGO, ASUMIENDO UNA CARGA EN EL P2.0
	while(1){}

}

void EINT0_IRQHandler(void){
	LPC_SC ->EXTINT = (1<<0);//BAJO BANDERA

	banderaFalla = 1;

	LPC_TIM0 -> TCR = 0; //APAGO TIMER0
	LPC_GPIO2 -> FIOSET = (1 << 1);//PRENDO ALARMA, ASUMIENDO UNA EN EL P2.1
	LPC_GPIO2 -> FIOCLR = (1<<0); //APAGO RESISTENCIA DEL P2.0

}
void TIMER0_IRQHandler(void){
	if((LPC_TIM0 -> IR & (1<<0))){
		LPC_TIM0 -> IR=(1<<0);

		if((LPC_TIM0 -> MR1 > 0)){//SI MI DUTY ESTÁ EN ALTO
			LPC_GPIO2 -> FIOSET = (1<<0); //PRENDO LA CARGA
		}

	}

	if((LPC_TIM0 -> IR &(1<<1))){
		LPC_TIM0 -> IR = (1<<1);

		if(LPC_TIM0 -> MR1 < LPC_TIM0->MR0){//SI EL TIEMPO EN ALTO ES MENOR QUE EL PERIODO:
			LPC_GPIO2 -> FIOCLR = (1<<0); //APAGO LA CARGA

		}
	}
}










/*
 *2) Según la consigna solicitada, es correcta la siguiente función de configuración de TIMER0?
 *Corríjala para que funcione correctamente.
 */

/*
void conf_timer0_pwm(uint8_t duty) {
	LPC_SC->PCONP |= (1 << 1); //BIEN, PRENDE TIM0
	LPC_TIM0->TCR = 0x02;
	LPC_TIM0->PR = 0; //MAL PRESCALER
	LPC_TIM0->MR0 = 25000; // 1ms @ 25MHz
	LPC_TIM0->MR1 = (23000 * duty) / 100;
	LPC_TIM0->MCR = (1 << 0) | (0 << 1) | (1 << 3); //FALTA 1<<4, P/ HABILITAR INT DEL MR1 Y NO RESETEA
	//bit0 y bit3: reset = 1; bit1: int = 1; bit2: stop = 0
	LPC_TIM0->TCR &= 0x01; //NO VA EL &
}
*/
//CORRECCION:
void conf_timer0_pwm(uint8_t duty) {
	LPC_SC->PCONP |= (1 << 1); //BIEN, PRENDE TIM0
	LPC_TIM0->PR  = 25-1; //PASOS DE 1us
	LPC_TIM0->MR0 = 1000-1; //MR0 = 1ms ; 1ms @ 25MHz
	LPC_TIM0->MR1 = (25 * duty) / 100;
	LPC_TIM0->MCR = (1 << 0) | (1 << 1) | (1 << 3) |(1<<4);
	LPC_TIM0->TCR = 0x02;
	LPC_TIM0->TCR = 0x01;
}
