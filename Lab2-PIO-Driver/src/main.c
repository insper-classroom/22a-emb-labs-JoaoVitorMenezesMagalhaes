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

/*  Default pin configuration (no attribute). */
#define _PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define _PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define _PIO_DEGLITCH            (1u << 1)
/*  The internal debouncing filter is active. */
#define _PIO_DEBOUNCE            (1u << 3)


void _pio_set(Pio *p_pio, const uint32_t ul_mask);
void _pio_clear(Pio *p_pio, const uint32_t ul_mask);
void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_pull_up_enable);
void _pio_set_input(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_attribute);
void _pio_set_output(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_default_level, const uint32_t ul_multidrive_enable, const uint32_t ul_pull_up_enable);
uint32_t _pio_get(Pio *p_pio, const pio_type_t ul_type, const uint32_t ul_mask);
void init(void);


void _pio_set(Pio *p_pio, const uint32_t ul_mask){
	p_pio->PIO_SODR = ul_mask;
}

void _pio_clear(Pio *p_pio, const uint32_t ul_mask){
	p_pio->PIO_CODR = ul_mask;
}

void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_pull_up_enable){
	if(ul_pull_up_enable){
		p_pio->PIO_PUER = ul_mask;	
	}
	else{
		p_pio->PIO_PUDR = ul_mask;
	}
}

void _pio_set_input(Pio *p_pio, const uint32_t ul_mask, const uint32_t ul_attribute){
	_pio_pull_up(p_pio, ul_mask, (ul_attribute & _PIO_PULLUP));
	
	if (ul_attribute & (_PIO_DEGLITCH | _PIO_DEBOUNCE)){
		p_pio->PIO_IFER = ul_mask;
	}
	else{
		p_pio->PIO_IFDR = ul_mask;
	}
}

void _pio_set_output(Pio *p_pio, const uint32_t ul_mask,
const uint32_t ul_default_level,
const uint32_t ul_multidrive_enable,
const uint32_t ul_pull_up_enable)
{
	
	p_pio->PIO_PER = ul_mask;
	p_pio->PIO_OER = ul_mask;
	
	if(ul_default_level){
		_pio_clear(p_pio, ul_mask);
	}
	else{
		_pio_set(p_pio, ul_mask);
	}
	
	if(ul_multidrive_enable){
		p_pio->PIO_MDER = ul_mask;
	}
	else{
		p_pio->PIO_MDDR = ul_mask;
	}
	
	_pio_pull_up(p_pio, ul_mask, ul_pull_up_enable);
	
}

uint32_t _pio_get(Pio *p_pio, const pio_type_t ul_type, const uint32_t ul_mask){
	uint32_t ul_io;
	if (ul_type == PIO_OUTPUT_0 || ul_type == PIO_OUTPUT_1){
		ul_io = p_pio->PIO_ODSR;
	} 
	else {
		ul_io = p_pio->PIO_PDSR;
	}
	
	if ((ul_mask & ul_io) == 0) {
		return 0;
	}
	else{
		return 1;
	}
}


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
	_pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	_pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	_pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	_pio_set_output(LED4_PIO, LED4_PIO_IDX_MASK, 0, 0, 0);
	

	_pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	_pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	_pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	_pio_set_input(BUT4_PIO, BUT4_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);

		
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
	if (!_pio_get(BUT_PIO, PIO_INPUT,BUT_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			_pio_clear(PIOC, LED_PIO_IDX_MASK);
			delay_ms(100);
			_pio_set(PIOC, LED_PIO_IDX_MASK);
			delay_ms(100);	
		}
	}
	if (!_pio_get(BUT2_PIO, PIO_INPUT,BUT2_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			_pio_clear(PIOA, LED2_PIO_IDX_MASK);
			delay_ms(100);
			_pio_set(PIOA, LED2_PIO_IDX_MASK);
			delay_ms(100);
		}
	}
	if (!_pio_get(BUT3_PIO, PIO_INPUT,BUT3_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			_pio_clear(PIOC, LED3_PIO_IDX_MASK);
			delay_ms(100);
			_pio_set(PIOC, LED3_PIO_IDX_MASK);
			delay_ms(100);
		}
	}
	if (!_pio_get(BUT4_PIO, PIO_INPUT,BUT4_PIO_IDX_MASK)){
		for (int i=1; i <= 5; i++){
			_pio_clear(PIOB, LED4_PIO_IDX_MASK);
			delay_ms(100);
			_pio_set(PIOB, LED4_PIO_IDX_MASK);
			delay_ms(100);
		}
	}
  }
  return 0;
}
