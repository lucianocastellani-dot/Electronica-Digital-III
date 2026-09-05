
/*Calculo del promedio movil con los ultimos 8 datos leidos del puerto 0.
 * Entrada: Los 8 bits menos significativos del Puerto 0, leídos cada cierto tiempo (usando un delay).
• Salida: El resultado del promedio, también de 8 bits, debe mostrarse en los 8 pines más bajos del
Puerto 2.
• Consideración: Un promedio móvil se calcula sumando los últimos n valores y dividiendo por n.
En este caso, n=8.
 *
 */
/*
#define tiempo 500000
uint8_t muestras[8] = {0};
uint8_t indice = 0;
void delay();
void configGPIO();
float promedio();
void configGPIO(void){
	//ENTRADAS EN EL PUERTO 0
	LPC_PINCON -> PINSEL0  &= ~(0xFFFF); //CON ESTO CONFIGURO LOS 8 BITS
	LPC_GPIO0  -> FIODIR   &= ~(0xFF); //MARCO COMO ENTRADA LOS PINES 0:7
	LPC_PINCON -> PINMODE0 |= (0x5555); //PONGO 0101 EN LOS BITS P/ MODO REPEATER
	//SALIDA EN EL PUERTO 2:
	LPC_PINCON -> PINSEL4  &= ~(0xFFFF);
	LPC_GPIO2  -> FIODIR   |=  (0xFF); //MARCO COMO SALIDA
	//LPC_PINCON -> PINMODE  |=  (0x5555);




}
int main(){
	configGPIO();


	while(1){
		// Leo los 8 bits menos significativos del puerto 0
		muestras[indice] = (uint8_t)(LPC_GPIO0 -> FIPIN & 0xFF);
		//Avanzo el indice de forma circular de 0 a 7
		indice = (indice + 1) % 8;
		uint8_t suma = 0;
		for (int i = 0; i < 8; i++){
			suma += muestras[i];
		}

		uint8_t prom = (suma/8);

		LPC_GPIO2 -> FIOCLR = (0xFF);
		LPC_GPIO2 -> FIOSET = (prom);



		delay();

	}


}
void delay(void){
	//int t = tiempo;
	for(volatile uint32_t i = 0; i < tiempo; i++){}
}

*/
/*
void gpio();

int main(){
	gpio();
	uint32_t entrada = LPC_GPIO2 -> FIOPIN;

	uint32_t A_in = entrada & 0x0F;
	uint32_t B_in = (entrada >> 4) & 0x0F;

	int operacion = (entrada >> 8) & 0x01; //Si es cero sumo, si es 1 resto

		uint8_t  resultado = 0;
		uint8_t  bandera = 0;

	if(operacion == 0){
		uint8_t suma = A_in + B_in;

		if(suma > 15){
				bandera = 1; //Bandera del OVF

			}
				resultado = suma & 0x0F;

	}
	else {
		if (A_in < B_in){
				bandera = 1; //Si A<B si o si tengo un numero negativo
				resultado = A_in - B_in;
			} else{
				bandera = 0; //Si A>B nunca voy a tener un numero negativo
				resultado = A_in - B_in;
			}
		}

	LPC_GPIO2 -> FIOCLR = (0x0F << 9 | 1 << 13);

	LPC_GPIO2 -> FIOSET = (resultado << 9 | bandera << 13);


}


void gpio(void){
		LPC_PINCON -> PINSEL4 &= ~(0x0FFFFFFF);
		LPC_GPIO2  -> FIODIR &= ~(0x01FF); //Pongo como entradas los 8 primeros bist del puerto 2
		LPC_GPIO2 ->  FIODIR |= (0x0F << 9 | 1 << 13); //Pongo como salidas los bits 9:12 y el 13 para switch
}
*/








