#include "led_strip.h"


void led_strip_on()
{
    l_i_("[ LED ] ON");
    _dW_(PIN_LED_STRIP_OUT, 1);
}



void led_strip_off()
{
    l_i_("[ LED ] OFF");
    _dW_(PIN_LED_STRIP_OUT, 0);
}
