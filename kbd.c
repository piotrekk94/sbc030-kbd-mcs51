#include <8051.h>

#define DEBOUNCE 10
#define REPEAT 50

static inline void uart_init(void)
{
    SCON = 0x40;
    PCON = 0x80;
    TMOD = 0x20;
    TH1 = 0xFF;
    TR1 = 1;
}

static inline void uart_write(char c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

static inline void led_set(char val)
{
    P1_0 = val ? 1 : 0;
}

void main(void)
{
    unsigned char keys[8][8];
    unsigned char rst;
    char i, j;
    char led = 0;
    uart_init();

    led_set(led);

    P2 = 0xff;

    P3_2 = 1;

    for(;;){
        led_set(led);
        led = !led;

	if(!P3_2){
		rst++;
	} else {
		if(rst > DEBOUNCE){
			uart_write('R');
		}
		rst = 0;
	}

	if(rst == DEBOUNCE || rst == REPEAT){
		rst = DEBOUNCE + 1;
		uart_write('r');
	}

        for(i = 0; i < 8; i++){
            P0 = 0xff & ~(1 << i);
            unsigned char row = P2;
            for(j = 0; j < 8; j++){
                if(!(row & (1<<j))){
                    keys[i][j]++;
                } else {
                    if(keys[i][j] > DEBOUNCE){
                        uart_write(i * 8 + j + 64 + 128);
                    }
                    keys[i][j] = 0;
                }
                if(keys[i][j] == DEBOUNCE || keys[i][j] == REPEAT){
                    keys[i][j] = DEBOUNCE + 1;
                    uart_write(i * 8 + j + 128);
                }
            }
        }
    }
}
