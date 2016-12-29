#define P_DELAY		5000
#define PRESSED			1
#define NOT_PRESSED		0
#define BUTTON_PIN	PORTE_IN
#define BUTTON_1	5
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>

/*
Predefine block
*/
unsigned char mode;
unsigned char cmode;
unsigned char EEMEM m;
unsigned char INC_DELAY = 2;
unsigned char S_DELAY = 15;
EEMEM unsigned char	e_mode;
EEMEM unsigned char	e_serie;
EEMEM unsigned char	e_s_serie;
EEMEM unsigned char	e_power;

unsigned char mode;
unsigned char s, ss;
unsigned char power;
unsigned char button_state=NOT_PRESSED;
//пин порта C 4 отвечает за сторону палки со стороны юсб порта! (& 8)
//пин порта C 3 отвечает за сторону палки со стороны без юсб порта! (& 4)

void delay(int i) //delay proxy function for arduino functions
{_delay_ms(i*10);}
/**
End of predefine block
*/


int ar[MAX] =  {0}; //
int ag[MAX] =  {0};	// Arrays of colors for setpixel and showstrip.
int ab[MAX] =  {0};	//

/*
Write functions block
*/

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
    OSC.PLLCTRL = 24 + 128*0;
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
	cl();
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
			if ((mode == SCEN_MODE || mode == SCEN_MODE_2)&&(eeprom_read_byte(&e_serie)!=1)) 
			{
				eeprom_write_byte(&e_serie,   1);
				return 1;
			}
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
