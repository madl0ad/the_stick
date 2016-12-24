#define S_DELAY		15
#define P_DELAY		5000
#define PRESSED			1
#define NOT_PRESSED		0
#define BUTTON_PIN	PORTE_IN
#define BUTTON_1	5
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>

unsigned char mode;
unsigned char s, ss;
unsigned char power;
unsigned char button_state=NOT_PRESSED;
//пин порта C 4 отвечает за сторону палки со стороны юсб порта! (& 8)
//пин порта C 3 отвечает за сторону палки со стороны без юсб порта! (& 4)
/*
Predefine block
*/
unsigned char mode;
unsigned char cmode;
unsigned char EEMEM m;
unsigned char INC_DELAY = 2;
EEMEM unsigned char	e_mode;
EEMEM unsigned char	e_serie;
EEMEM unsigned char	e_s_serie;
EEMEM unsigned char	e_power;
void delay(int i) //delay proxy function for arduino functions
{_delay_ms(i*10);}
void ws_write_0();
void ws_write_1();
void ws_write_h1();
void ws_write_q1();
/**
End of predefine block
*/

/*
Predefined colors block
*/
//					GREEN			RED				BLUE
void  ws_c()	{ ws_write_0();	ws_write_0();	ws_write_0(); }

void  ws_r()	{ ws_write_0();	ws_write_1();	ws_write_0(); }
void  ws_g()	{ ws_write_1();	ws_write_0();	ws_write_0(); }
void  ws_b()	{ ws_write_0();	ws_write_0();	ws_write_1(); }

void  ws_y()	{ ws_write_1();	ws_write_1();	ws_write_0(); }
void  ws_a()	{ ws_write_1();	ws_write_0();	ws_write_1(); }
void  ws_v()	{ ws_write_0();	ws_write_1();	ws_write_1(); }

void  ws_w()	{ ws_write_1();	ws_write_1();	ws_write_1(); }

void  ws_hr()	{ ws_write_0();	ws_write_h1();	ws_write_0(); }
void  ws_hg()	{ ws_write_h1();ws_write_0();	ws_write_0(); }
void  ws_hb()	{ ws_write_0();	ws_write_0();	ws_write_h1(); }
void  ws_hw()	{ ws_write_h1();	ws_write_h1();	ws_write_h1(); }

void  ws_ry() 	{ ws_write_h1();ws_write_1();	ws_write_0(); }
void  ws_gy() 	{ ws_write_1();ws_write_h1();	ws_write_0(); }
void  ws_gv() 	{ ws_write_0();	ws_write_1();	ws_write_h1(); }
void  ws_bv() 	{ ws_write_0();	ws_write_h1();	ws_write_1(); }
void  ws_ba() 	{ ws_write_h1();ws_write_0();	ws_write_1(); }
void  ws_ga() 	{ ws_write_1();	ws_write_0();	ws_write_h1(); }
void  ws_wr() 	{ ws_write_q1();	ws_write_1();	ws_write_q1(); }
void  ws_wg() 	{ ws_write_1();	ws_write_q1();	ws_write_q1(); }
void  ws_wb() 	{ ws_write_q1();	ws_write_q1();	ws_write_1(); }
void write_w(int fill);

void (*colors [21]) (void) ={ //color array
  ws_c,
ws_r,
ws_g,
ws_b,
ws_y, 
ws_a,
ws_v,
ws_w,
  ws_hr	,
  ws_hg	,
  ws_hb	,
  ws_hw	,
  
  ws_ry 	,
  ws_gy 	,
  ws_gv 	,
  ws_bv 	,
  ws_ba 	,
  ws_ga 	,
  ws_wr 	,
  ws_wg 	,
  ws_wb 	
};
/*
End of predefined colors block
*/

int ar[MAX] =  {0}; //
int ag[MAX] =  {0};	// Arrays of colors for setpixel and showstrip.
int ab[MAX] =  {0};	//

/*
Write functions block
*/

void write_c(int intensity, int color) //Outputs a single pixel, with brightness set by intensity and color. 
									   //Use 1 for green, 2 for red, 4 for blue, their sum for combos
{
	write_w(intensity * ((1 & color) > 0));
	write_w(intensity * ((2 & color) > 0));
	write_w(intensity * ((4 & color) > 0));
}



void write_w(int fill) //Various intensity output, fill = [0,255]
{
	int i;
	for (i = 8; i>0; i--)
		if((fill&(1<<i))>0) 
		{
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0;
		}
		else 
		{
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;
		
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		}
} 

void write_c_def(int r, int g, int b) // immediately prints a single pixel with color (r,g,b)
{
	write_w(g);
	write_w(r);
	write_w(b);
}
void setpixel(int r, int g, int b, int n) //sets pixel n with color (r,g,b). Do not forget to showstrip() after setting all pixels.
{
	n = n>MAX?MAX:(n<1?1:n);
	ar[n-1] = r;
	ag[n-1] = g;
	ab[n-1] = b;
}

void drop() //clears LED color array
{
	for (int i = 0; i < MAX; i++)
	{
		ar[i] = 0;
		ag[i] = 0;
		ab[i] = 0;
	}
}

ISR(TCC0_OVF_vect)
{
	//check_button();
}

void right() //Sets on only USB side of the stick
{
	PORTC_DIR = 8;
}

void left() //Sets on only non-USB side of the stick
{
	PORTC_DIR = 4;
}
 
void both() //Sets on both sides of the stick
{
	PORTC_DIR = 255;
}

void showstrip() //Shows defined LED colors, set by setpixel();
{
	for (int i = 0; i < MAX; i++)
	{
		write_c_def(ar[i], ag[i], ab[i]);
	}
}

/*
End of write functions block
*/
void	cl(){ 		//system setup function
	
	OSC_CTRL  = 15; //Setup 32Mhz crystal
    while(!(OSC_RC32KRDY_bm));
    CLK.PSCTRL=0;//'No PreScaler in use
    
    //OSC.XOSCCTRL = 195; //'12-16MHz, 256 Clks
    OSC.PLLCTRL = 24 + 128*0;

//	OSC.DFLLCTRL |= OSC_RC2MCREF_RC32K_gc;
//	OSC.DFLLCTRL = 1;

    OSC_CTRL |= OSC_PLLEN_bm;
	while(!(OSC_STATUS & OSC_PLLRDY_bm));
	
	CCP=CCP_IOREG_gc;      //'Config Change Protection
	CLK.CTRL = 4;       //'Use PLL as Clock Source
} 

/*
Button functions block
*/

void init()
{
	PORTC_DIR=255;
	PORTE_DIR=0;
	mode=eeprom_read_byte(&e_mode);
	if (mode>MODE_NUM || mode<1) mode=1;
	//setUp32MhzInternalOsc();
	cl();
	//PORTE.PIN5CTRL = PORT_ISC_BOTHEDGES_gc;
	//PORTE.INT0MASK = PIN5_bm;
	//PORTE.INTCTRL = PORT_INT0LVL_LO_gc;
	//PMIC.CTRL |= PMIC_LOLVLEN_bm;
	/*TCC0.CNT = 0;// Zeroise count
	TCC0.PER = 150; //Period      
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc; //Divider 
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc; //Liow level interrupt
	TCC0.INTFLAGS = 0x01; // clear any initial interrupt flags 
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc; // Normal operation
    sei();*/
	return;
}

void resetMcu()
{
  uint8_t oldInterruptState = SREG;  // no real need to store the interrupt context as the reset will pre-empt its restoration
  __asm("cli");                       // Disable interrupts

  CCP = 0xD8;                        // Configuration change protection: allow protected IO regiser write
  RST.CTRL = RST_SWRST_bm;           // Request software reset by writing to protected IO register

  SREG=oldInterruptState;            // Restore interrupts enabled/disabled state (out of common decency - this line will never be reached because the reset will pre-empt it)
}

void setpixel_c(char i, char j)
{
	switch (i)
			{
				case 0:	setpixel(0,0,0,j);	break;
				case 1:	setpixel(255,0,0,j);	break;
				case 2:	setpixel(0,255,0,j);	break;
				case 3:	setpixel(0,0,255,j);	break;
				case 4:	setpixel(255,255,0,j);	break;
				case 5:	setpixel(0,255,255,j);	break;
				case 6:	setpixel(255,0,255,j);	break;
				case 7:	setpixel(255,255,255,j);	break;
				case 8:	setpixel(128,0,0,j);	break;
				case 9:	setpixel(0,128,0,j);	break;
				case 10:setpixel(0,0,128,j);	break;
				case 11:setpixel(128,128,128,j);	break;
				case 12:setpixel(255,128,0,j);	break;
				case 13:setpixel(128,255,0,j);	break;
				case 14:setpixel(255,0,128,j);	break;
				case 15:setpixel(128,0,255,j);	break;
				case 16:setpixel(0,128,255,j);	break;
				case 17:setpixel(0,255,128,j);	break;
				case 18:setpixel(255,64,64,j);	break;
				case 19:setpixel(64,255,64,j);	break;
				case 20:setpixel(64,64,255,j);	break;
			}
}

unsigned char process_button( unsigned int hold){
	if ( hold > S_DELAY ){	// next serie
			if ( ++mode > MODE_NUM || mode < 1) { 
				mode=1; 
			}
			eeprom_write_byte(&e_mode,   mode);//Запись
			eeprom_write_byte(&e_serie,   1);
			if (mode == PICT_MODE) resetMcu();
			return 1;
			}
	else if ( hold > INC_DELAY ){	// next mode
			s = eeprom_read_byte(&e_serie);
				if ( ++s > S_NUM || s < 1){
				s = 1;
			} 				
			eeprom_write_byte(&e_serie,   s);//Запись
			if (eeprom_read_byte(&e_mode) == PICT_MODE) resetMcu();
			return 1;
	}
	return 0;
}

unsigned char check_button(){
	static unsigned char last_button_state=NOT_PRESSED;
	static unsigned int hold=0;
	unsigned char res=0;
	
	button_state=NOT_PRESSED;
	
	if ( (PORTE_IN & (1<<5)) != (1<<5) ) {
			button_state = PRESSED;
	}
	
	if ( button_state == PRESSED ){
		hold++;
	}
	
	if ( (last_button_state == button_state) && (button_state == NOT_PRESSED) ){
		res = process_button( hold );
	}
	
	if ( button_state == NOT_PRESSED && last_button_state == NOT_PRESSED ){
		hold=0;
	}
	last_button_state=button_state;
	return res;
}


/* 
End of button functions block
*/

/*
 Functions below are obsolete, use write_w(x) instead!
*/
 void ws_write_1(){ //emits full byte
	unsigned char i;
	for (i=0; i<7; i++){
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; //PORTC_OUT=0; 
	}
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		//PORTC_OUT=0; //PORTC_OUT=0; PORTC_OUT=0; 
		}

void ws_write_0(){ //emits zero byte
	unsigned char i;
	for (i=0; i<7; i++){
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;
		
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
	}
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;
		
 		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;
}

void ws_write_h1(){ //emits half byte
	unsigned char i;
	
	PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
	PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
	PORTC_OUT=255;PORTC_OUT=255;
	
	PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
	PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
	PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
	PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
	PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		
	for (i=0; i<6; i++){
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; //PORTC_OUT=0; 
	}
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		//PORTC_OUT=0; //PORTC_OUT=0; PORTC_OUT=0; 
}

void ws_write_q1(){ //emits quarter byte
	unsigned char i;
	
	for (i=0; i<2; i++){
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;
		
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
		PORTC_OUT=0;PORTC_OUT=0;PORTC_OUT=0;
	}
		
	for (i=0; i<5; i++){
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; //PORTC_OUT=0; 
	}
	
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;PORTC_OUT=255;
		PORTC_OUT=255;PORTC_OUT=255;

		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; PORTC_OUT=0; 
		//PORTC_OUT=0; //PORTC_OUT=0; PORTC_OUT=0; 
}