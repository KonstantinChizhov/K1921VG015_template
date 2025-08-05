#include <K1921VG015.h>

#define LEDS_MSK 0xF000

class InitTest
{
public:
    InitTest()
    {
        RCU->CGCFGAHB_bit.GPIOAEN = 1;
        // Включаем  GPIOA
        RCU->RSTDISAHB_bit.GPIOAEN = 1;
        GPIOA->OUTENSET = LEDS_MSK;
        GPIOA->DATAOUTSET = LEDS_MSK;
    }

    ~InitTest()
    {
        GPIOA->DATAOUTTGL = 0x1000;
    }
};


InitTest init;

int main()
{
    for(int n =0; n < 20; n++)
    {
        for (volatile int i = 0; i < 10'000; i = i + 1)
            ;

        GPIOA->DATAOUTTGL = LEDS_MSK;
    }

    GPIOA->DATAOUTTGL = 0x2000;
    return 0;
}