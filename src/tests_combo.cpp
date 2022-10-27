#include "tests_combo.h"



void test_motor_n_led_n_batt_n_water()
{
    while (1)
    {
        l_i_("battery");
        _dW_(PIN_ADC_BATTERY_OUT, 1);
        delay(100);
        int val = _aR_(PIN_ADC_BATTERY_IN);
        _dW_(PIN_ADC_BATTERY_OUT, 0);
        l_i_("battery ADC value %d", val);
        delay(100);



        l_i_("water sensor");
        _dW_(PIN_WATER_EN_OUT, 1);
        delay(100);
        val = _aR_(PIN_ADC_WATER_IN);
        _dW_(PIN_WATER_EN_OUT, 0);
        l_i_("water ADC value %d", val);
        delay(100);
        


        l_i_("motor");
        uint16_t t_ms = 1000;
        motor_set_resolution(0);
        motor_move_right(t_ms);
        delay(t_ms);
        motor_move_left(t_ms);
        


        l_i_("leds");
        for (uint8_t i = 0; i < 3; i++)
        {
            _dW_(PIN_LED_STRIP_OUT, 1);
            delay(100);
            _dW_(PIN_LED_STRIP_OUT, 0);
            delay(100);
        }

        delay(3000);
    }
}

