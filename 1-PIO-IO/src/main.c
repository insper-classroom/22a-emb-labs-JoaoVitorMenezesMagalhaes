/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

#define LED_PIO           PIOC                 // periferico que controla o LED 
#define LED_PIO_ID        ID_PIOC                   // ID do periférico PIOC (controla LED)     
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED    

#define LED2_PIO           PIOA                 // periferico que controla o LED
#define LED2_PIO_ID        ID_PIOA                   // ID do periférico PIOC (controla LED)
#define LED2_PIO_IDX       0                    // ID do LED no PIO
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)   // Mascara para CONTROLARMOS o LED

#define LED3_PIO           PIOC                 // periferico que controla o LED
#define LED3_PIO_ID        ID_PIOC                   // ID do periférico PIOC (controla LED)
#define LED3_PIO_IDX       30                    // ID do LED no PIO
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)   // Mascara para CONTROLARMOS o LED

#define LED4_PIO           PIOB                 // periferico que controla o LED
#define LED4_PIO_ID        ID_PIOB                   // ID do periférico PIOC (controla LED)
#define LED4_PIO_IDX       2                    // ID do LED no PIO
#define LED4_PIO_IDX_MASK  (1 << LED4_PIO_IDX)   // Mascara para CONTROLARMOS o LED                                                    

#define BUT_PIO          PIOA
#define BUT_PIO_ID       ID_PIOA
#define BUT_PIO_IDX		 11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX)

#define BUT2_PIO          PIOD
#define BUT2_PIO_ID       ID_PIOD
#define BUT2_PIO_IDX		 28
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)

#define BUT3_PIO          PIOC
#define BUT3_PIO_ID       ID_PIOC
#define BUT3_PIO_IDX		 31
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

#define BUT4_PIO          PIOA
#define BUT4_PIO_ID       ID_PIOA
#define BUT4_PIO_IDX		 19
#define BUT4_PIO_IDX_MASK (1u << BUT4_PIO_IDX)

void init(void);


// Função de inicialização do uC
void init(void){
	// Initialize the board clock
	sysclk_init();
	
	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o LED foi conectado para que possamos controlar o LED.
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(LED3_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pmc_enable_periph_clk(LED4_PIO_ID);
	pmc_enable_periph_clk(BUT4_PIO_ID);

	// Inicializa PC8 como saída
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED4_PIO, LED4_PIO_IDX_MASK, 0, 0, 0);
	
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT4_PIO, BUT4_PIO_IDX_MASK, PIO_DEFAULT);
	
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);
	pio_pull_up(BUT4_PIO, BUT4_PIO_IDX_MASK, 1);
		
}
/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();

  // super loop
  // aplicacoes embarcadas não devem sair do while(1).
  while (1)
  {
	if (!pio_get(BUT_PIO, PIO_INPUT,BUT_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			pio_clear(PIOC, LED_PIO_IDX_MASK);
			delay_ms(100);
			pio_set(PIOC, LED_PIO_IDX_MASK);
			delay_ms(100);	
		}
	}
	if (!pio_get(BUT2_PIO, PIO_INPUT,BUT2_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			pio_clear(PIOA, LED2_PIO_IDX_MASK);
			delay_ms(100);
			pio_set(PIOA, LED2_PIO_IDX_MASK);
			delay_ms(100);
		}
	}
	if (!pio_get(BUT3_PIO, PIO_INPUT,BUT3_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			pio_clear(PIOC, LED3_PIO_IDX_MASK);
			delay_ms(100);
			pio_set(PIOC, LED3_PIO_IDX_MASK);
			delay_ms(100);
		}
	}
	if (!pio_get(BUT4_PIO, PIO_INPUT,BUT4_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			pio_clear(PIOB, LED4_PIO_IDX_MASK);
			delay_ms(100);
			pio_set(PIOB, LED4_PIO_IDX_MASK);
			delay_ms(100);
		}
	}
  }
  return 0;
}
