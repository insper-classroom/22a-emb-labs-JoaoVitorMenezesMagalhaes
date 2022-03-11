#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

#define LED_PIO           PIOC                 
#define LED_PIO_ID        ID_PIOC              
#define LED_PIO_IDX       8                    
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   

#define LED2_PIO           PIOA                 
#define LED2_PIO_ID        ID_PIOA              
#define LED2_PIO_IDX       0                    
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)  

#define LED3_PIO           PIOC                 
#define LED3_PIO_ID        ID_PIOC              
#define LED3_PIO_IDX       30                   
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)  

#define LED4_PIO           PIOB                 
#define LED4_PIO_ID        ID_PIOB              
#define LED4_PIO_IDX       2                    
#define LED4_PIO_IDX_MASK  (1 << LED4_PIO_IDX)   

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
void pisca_led(Pio *p_pio, const uint32_t mask, int n, int t);
void display_update(int freq);
int change_freq(int freq);





volatile char start_stop_flag;
volatile char change_frec_flag;
volatile char decrease_flag;



void edge_freq_callback(void){
		 if (pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)) {
			 change_frec_flag = 0;
		} 
		else {
			change_frec_flag = 1;
		 }
}

void start_stop_callback(){
	start_stop_flag = 1;
}

void decrease_callback(){
	decrease_flag = 1;
}


int change_freq(int freq){
	if(decrease_flag){
		freq -= 100;
		display_update(freq);
		decrease_flag = 0;
		
		return freq;
	}
	for(double i =0; i < 2500000; i++){
		if(!change_frec_flag){
			freq += 100;
			display_update(freq);
			return freq;
		}
	}
	change_frec_flag = 0;
	freq -= 100;
	display_update(freq);
	return freq;
}



void init(void){
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, PIO_DEFAULT);
	
	
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pmc_enable_periph_clk(BUT4_PIO_ID);
	
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, 60);
	
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT3_PIO, BUT3_PIO_IDX_MASK, 60);
	
	pio_configure(BUT4_PIO, PIO_INPUT, BUT4_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT4_PIO, BUT4_PIO_IDX_MASK, 60);
		
	pio_handler_set(BUT2_PIO,
					BUT2_PIO_ID,
					BUT2_PIO_IDX_MASK,
					PIO_IT_EDGE,
					edge_freq_callback);
					
	pio_handler_set(BUT3_PIO,
					BUT3_PIO_ID,
					BUT3_PIO_IDX_MASK,
					PIO_IT_FALL_EDGE,
					start_stop_callback);
					
	pio_handler_set(BUT4_PIO,
					BUT4_PIO_ID,
					BUT4_PIO_IDX_MASK,
					PIO_IT_FALL_EDGE,
					decrease_callback);
					
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT2_PIO);
	
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT3_PIO);
	
	pio_enable_interrupt(BUT4_PIO, BUT4_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT4_PIO);

	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 4); 
	
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 5); 
	
	NVIC_EnableIRQ(BUT4_PIO_ID);
	NVIC_SetPriority(BUT4_PIO_ID, 4);
}

void pisca_led(Pio *p_pio, const uint32_t mask, int n, int t){
	int c = 0;
	gfx_mono_generic_draw_horizontal_line(90, 20, 30, GFX_PIXEL_SET);
	for (int i=0; i<n; i++) {
		if(start_stop_flag) {
			pio_set(p_pio, mask);
			start_stop_flag = 0;
			break;
		}
		
		gfx_mono_generic_draw_vertical_line(90 + c, 10, 10, GFX_PIXEL_SET);
		pio_clear(p_pio, mask);
		delay_ms(t/2);
		pio_set(p_pio, mask);
		delay_ms(t/2);
		c++;
	}

	gfx_mono_generic_draw_filled_rect(90, 10, 30, 11, GFX_PIXEL_CLR);
}


void display_update(int freq) {
	char str[128];
	sprintf(str, "%d ms", freq);
	gfx_mono_draw_string(str, 50,16, &sysfont);
}



int main (void)
{

	WDT->WDT_MR = WDT_MR_WDDIS;
	
	board_init();
	sysclk_init();
	delay_init();
	init();
	
	int freq = 100;
	char str[128];

  // Init OLED
	gfx_mono_ssd1306_init();
  
	sprintf(str, "%d ms", freq);
	gfx_mono_draw_string(str, 50,16, &sysfont);

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		if (change_frec_flag || start_stop_flag || decrease_flag){
			if (change_frec_flag){
				freq = change_freq(freq);
			}	
			else if (start_stop_flag){
				start_stop_flag = 0;
				pisca_led(LED2_PIO, LED2_PIO_IDX_MASK, 30, freq);
			}
			else if (decrease_flag){
				freq = change_freq(freq);
			}
			change_frec_flag = 0;
			start_stop_flag = 0;
			decrease_flag = 0;
		}
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
	}
}
