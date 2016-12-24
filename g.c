// t13	atxmega128a3u	38400 prog
// board	MMCU		programm_after_make

/*	команды сборки:
	avr-gcc -std=$standard -Os -mmcu=$mmcu  -Wall -foptimize-sibling-calls -fdata-sections -ffunction-sections -Wl,-gc-sections $filename.c -o $filename.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock $filename.elf $filename.hex
	*
	avr-gcc -Os -mmcu=atxmega128a3  -Wall -foptimize-sibling-calls -fdata-sections -ffunction-sections -Wl,-gc-sections g.c -o g.elf

	вместо $filename подставляется нужное имя файла. как ты там его назовешь
	*/
//пин порта C 4 отвечает за сторону палки со стороны юсб порта! (& 8)
//пин порта C 3 отвечает за сторону палки со стороны без юсб порта! (& 4)

# define F_CPU 48000000UL

#define MAX	57
#define MODE_NUM 5
#define S_NUM 8
#define PICT_MODE 3
//# define F_CPU 32000000UL

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "doubles.c"
#include "pictures.c"
int * Wheel(int WheelPos) {
    WheelPos = 255 - WheelPos;
    static int color[3] = {0};
    if (WheelPos < 85) {
        color[0] = 255 - WheelPos * 3;
        color[1] = 0;
        color[2] = WheelPos * 3;
        return color;
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        color[0] = 0;
        color[1] = WheelPos * 3;
        color[2] = 255 - WheelPos * 3;
        return color;
    }
    WheelPos -= 170;
    color[0] = WheelPos * 3;
    color[1] = 255 - WheelPos * 3;
    color[2] = 0;
    return color;
}

unsigned char ololo[16]={0,1,2,3,4,5,6,7,6,5,4,3,2,1,0};



void pila(unsigned char c) {
    unsigned char i,j;
    if (c<4) {
        for (j=0; j<57; j+=2) {
            for ( i=0; i<=MAX; i++) {
                if (i<j) {
                    (*colors [c ])();
                } else {
                    (*colors [11-c ])();
                }
            }
            _delay_us(50);
        }
        for (j=56; j>0; j-=2) {
            for ( i=0; i<=MAX; i++) {
                if (i<j) {
                    (*colors [c ])();
                } else {
                    (*colors [11-c ])();
                }
            }
            _delay_us(50);
        }
    } else {
        for (j=0; j<57; j++) {
            for ( i=0; i<=MAX; i++) {
                if (i<j) {
                    (*colors [c ])();
                } else {
                    (*colors [0 ])();
                }
            }
            _delay_us(50);
        }
    }
}

unsigned char t_nof[1152] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0
};

void lines ()
{
    int i, j;
    for (i=209; i>0; i--) {

        for (j=MAX; j>0; j--) {
            //	(*colors [ pgm_read_byte(&arr[(i*MAX)+j]) ])();
        }
        if (i%95 == 0)
        {
            if (check_button()==1) return;
        }
        _delay_us(50);
    }
}


void nof() {
    unsigned int i, j = 0;
    for (i=0; i<48; i++) {
        for (j=0; j<MAX; j++) {
            if (j<=24) {
                (*colors [ t_nof[i*24+j] ])();
            } else {
                (*colors [ 0 ])();
            }
        }
        _delay_us(50);
    }
    if (check_button()==1) return;
}

void rainbow(int wait) {
    int i, j;
    int *col;
    for (j=0; j<256; j++) {
        for (i=0; i<MAX; i++) {
            col = Wheel((i+j) & 255);
            setpixel(*col,*(col+1),*(col+2), i);
        }
        showstrip();
        for (int i =0; i<wait; i++)
        {
            _delay_ms(25);
            if (check_button()==1) return;
        }
    }
}

void goinglight(int speed, int wait, int r, int g, int b)
{
    left();
    for (int i = MAX ; i >=0; i--)
    {
        if (i!=MAX)setpixel(0,0,0,i+1);
        setpixel(r,g,b,i);
        showstrip();
        _delay_us(speed*100);
    }
    setpixel(0,0,0,1);
    showstrip();
    for (int i =0; i<wait; i++)
    {
        _delay_ms(25);
        if (check_button()==1) return;
    }
    right();
    for (int i = 0 ; i <=MAX; i++)
    {
        if (i!=0)setpixel(0,0,0,i-1);
        setpixel(r,g,b,i);
        showstrip();
        _delay_us(speed*100);
    }
    both();
}

//Theatre-style crawling lights.
void theaterChase(int r, int g, int b, int wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (int i=0; i < MAX; i=i+3) {
                setpixel( r,g,b,i+q);    //turn every third pixel on
            }
            showstrip();

            for (int i =0; i<wait; i++)
            {
                _delay_ms(25);
                if (check_button()==1) return;
            }

            for (int i=0; i < MAX; i=i+3) {
                setpixel(0,0,0, i+q);        //turn every third pixel off
            }
        }
    }
}

void gunshot(int r, int g, int b, int speed)
{
    for (int i = 0 ; i < MAX; i++)
    {
        if (i!=0) setpixel(0,0,0,i-1);
        setpixel(r,g,b,i);
        showstrip();
        _delay_us(speed*100);
    }
    drop();
    for (int i = 0 ; i < MAX; i++)
    {
        setpixel(255,255,255,i);
    }
    showstrip();
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(int wait) {
    int *col;
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (int i=0; i < MAX; i=i+3) {
                col = Wheel( (i+j) % 255);
                setpixel( *col,*(col+1),*(col+2),i+q);    //turn every third pixel on
            }
            showstrip();

            for (int i =0; i<wait; i++)
            {
                _delay_ms(25);
                if (check_button()==1) return;
            }

            for (int i=0; i < MAX; i=i+3) {
                setpixel( 0,0,0, i+q);        //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

void do_chess()
{
    drop();
    int i, j;
    for (i=56; i>0; i--) {

        for (j=56; j>0; j--) {
            setpixel_c(chess[(i*56)+j],j+1);
        }
        showstrip();
        if (i%20 == 0)
        {
            if (check_button()==1) return;
        }
        _delay_us(50);
    }
}

void do_cards()
{
    drop();
    int i, j;
    for (i=308; i>0; i--) {

        for (j=57; j>0; j--) {
            setpixel_c(pgm_read_byte(&(cards[(i*57)+j])),j);
        }
        showstrip();
        if (i%20 == 0)
        {
            if (check_button()==1) return;
        }
        _delay_us(50);
    }
}

void do_batman()
{
    drop();
    int i, j;
    for (i=89; i>0; i--) {

        for (j=57; j>0; j--) {
            setpixel_c(pgm_read_byte(&(batman[(i*57)+j])),j);
        }
        showstrip();
        if (i%20 == 0)
        {
            if (check_button()==1) return;
        }
        _delay_us(50);
    }
}

void do_flame()
{
    int i, j;
    for (i=60; i>0; i--) {

        for (j=57; j>0; j--) {
            setpixel_c(flame[(i*56)+j],j+1);
        }
        showstrip();
        if (i%20 == 0)
        {
            if (check_button()==1) return;
        }
        _delay_us(50);
    }
}

void colorWipe(int r, int g, int b, int wait) {
    for (int i=0; i<MAX; i++) {
        setpixel(r,g,b,i);
        showstrip();
        _delay_ms(25);
        if (check_button()==1) return;
    }
}

void scenario1()
{
    INC_DELAY = 999;
    cmode = eeprom_read_byte(&e_mode);
    colorWipe(255,255,255,1);
    if (cmode!=eeprom_read_byte(&e_mode)) return;
    rainbow(1);
    if (cmode!=eeprom_read_byte(&e_mode)) return;
    for (int i = 0; i < 1; i++)
    {
        colorWipe(255,0,0,1);
        if (cmode!=eeprom_read_byte(&e_mode)) return;
        colorWipe(0,255,0,1);
        if (cmode!=eeprom_read_byte(&e_mode)) return;
        colorWipe(0,0,255,1); //Начальные простенькие эффекты
        if (cmode!=eeprom_read_byte(&e_mode)) return;
        _delay_ms(100);//Задержка после простеньких эффектов
    }
    drop();
    showstrip();
    for (int i = 0; i < 1; i++)
    {								//
        theaterChaseRainbow(1);	//РГБ-эффекты, театр-радуга
        if (cmode!=eeprom_read_byte(&e_mode)) return;
    }								//
    for (int i = 0; i < 50; i++)
    {
        do_chess();
        if (cmode!=eeprom_read_byte(&e_mode)) return;
    }
    gunshot(255,0,0,20);
    INC_DELAY = 2;
}

void scenario2()
{
    INC_DELAY = 999;
    for (int i = 0; i < 3334; i++)
        do_chess();
    int cmode = eeprom_read_byte(&e_mode);
    while (cmode == eeprom_read_byte(&e_mode))
        do_flame();
}


int main() {
    init();
    unsigned char i, cmode;
    i = 0;
    while (1) {
        switch (eeprom_read_byte(&e_mode))
        {
        case 1:
            for ( i=0; i<=MAX; i++) {
                (*colors [eeprom_read_byte(&e_serie)])();
            }
            _delay_ms(25);
            check_button();
            break;
        case 2:
            switch (eeprom_read_byte(&e_serie))
            {
            case 1:
                rainbow(5);
                //theaterChase(255,0,0,3);
                break;
            case 2:
                theaterChase(255,0,0,3);
                //rainbow(5);
                break;
            case 3:
                theaterChase(0,255,0,3);
                break;
            case 4:
                theaterChase(0,0,255,3);
                break;
            case 5:
            {
                cmode = eeprom_read_byte(&e_mode);
                colorWipe(255,0,0,5);
                if (cmode!=eeprom_read_byte(&e_mode) || eeprom_read_byte(&e_serie)!=5) break;
                colorWipe(0,255,0,5);
                if (cmode!=eeprom_read_byte(&e_mode) || eeprom_read_byte(&e_serie)!=5) break;
                colorWipe(0,0,255,5);
            }
            break;
            case 6:
            case 7:
            case 8:
                eeprom_write_byte(&e_serie,1);
                break;
            }
            break;
        case 3:
            switch (eeprom_read_byte(&e_serie))
            {
            case 1:
                do_chess();
                break;
            case 2:
                do_flame();
                break;
            case 3:
                do_batman();
                break;
			case 4:
				do_cards();
				break;
            case 5:
                eeprom_write_byte(&e_serie,1);
                break;
            }
            _delay_ms(25);
            check_button();
            break;
        case 4:
            switch (eeprom_read_byte(&e_serie))
            {
            case 1:
                drop();
                setpixel(64,0,0,1);
                showstrip();
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                scenario1();
            }
            INC_DELAY = 2;
            _delay_ms(25);
            check_button();
            break;
        case 5:
            switch (eeprom_read_byte(&e_serie))
            {
            case 1:
                drop();
                setpixel(0,64,0,1);
                showstrip();
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                scenario2();
                break;
            }
            INC_DELAY = 2;
            _delay_ms(25);
            check_button();
            break;
        default:
            _delay_ms(25);
            check_button();
            break;
        }
    }
}



