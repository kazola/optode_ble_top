#include "tests.h"
#include "Particle.h"



void test_ble_as_peripheral()
{
    ble_peripheral_optode_core();
}



void test_ble_as_central()
{
    if ((strlen(MAC_OPTODE_MINI_1) != _APP_BLE_MAC_LEN_) ||    \
        (strlen(MAC_OPTODE_MINI_2) != _APP_BLE_MAC_LEN_))
    {
        l_e_("[ BLE ] optode_mini MACs bad length");
    }

    
    const char * mac = BLE.address().toString().c_str();
    l_i_("[ BLE ] optode core set as central");
    l_i_("[ BLE ] optode core MAC address = %s", mac);


    BLE.on();
    while (1)
    {
        uint8_t mask = 0;
        ble_central_scan_for_optode_minis(&mask);

        l_i_("[ BLE ] optode mini detected mask = %d", mask);
        // if (mask != 3) continue;

        ble_central_interact_with_optode_mini(MAC_OPTODE_MINI_1);
    }
    BLE.off();
}



void test_motor()
{
    while (1)
    {
        uint16_t t_ms = 3000;
        motor_set_resolution(0);
        motor_move_right(t_ms);
        delay(t_ms);
        motor_move_left(t_ms);
        delay(t_ms);
    }
}



void test_led_strip()
{
    while (1)
    {
        l_i_("[ LED ] ON");
        _dW_(PIN_LED_STRIP_OUT, 1);
        delay(1000);
        l_i_("[ LED ] OFF");
        _dW_(PIN_LED_STRIP_OUT, 0);
        delay(1000);
    }
}


void test_battery_measurement()
{
    while (1)
    {
        _dW_(PIN_BATTERY_EN_OUT, 1);
        delay(1000);
        int val = _aR_(PIN_ADC_BATTERY_IN);
        _dW_(PIN_BATTERY_EN_OUT, 0);


        // tell UART
        l_i_("battery ADC value %d", val);
        delay(1000);
    }

}


void test_water_measurement()
{
    while (1)
    {
        _dW_(PIN_WATER_EN_OUT, 1);
        delay(1000);
        int val = _aR_(PIN_ADC_WATER_IN);
        _dW_(PIN_WATER_EN_OUT, 0);


        // tell UART
        l_i_("water ADC value %d", val);
        delay(1000);
    }
}



void test_all()
{
    #if 0
    while (1)
    {
        // useful when particle seems to not flash
        // delete main.cpp file and flash this
        l_i_("a");
        delay(100);
    }
    #endif


    int _i = 0;
    while (1)
    {
        l_i_("[ TEST ] all #%d start", _i);

        uint16_t t_ms = 3000;
        l_i_("[ TEST ] MOTOR to right");
        motor_set_resolution(0);
        motor_move_right(t_ms);
        delay(t_ms);
        l_i_("[ TEST ] MOTOR to left");
        motor_move_left(t_ms);
        delay(t_ms);


        l_i_("[ TEST ] LED 1, check it");
        _dW_(PIN_LED_STRIP_OUT, 1);
        delay(2000);
        _dW_(PIN_LED_STRIP_OUT, 0);
        delay(2000);


        _dW_(PIN_BATTERY_EN_OUT, 1);
        delay(100);
        int val = _aR_(PIN_ADC_BATTERY_IN);
        _dW_(PIN_BATTERY_EN_OUT, 0);
        l_i_("[ TEST ] ADC battery %d", val);


        _dW_(PIN_WATER_EN_OUT, 1);
        delay(100);
        val = _aR_(PIN_ADC_WATER_IN);
        _dW_(PIN_WATER_EN_OUT, 0);
        l_i_("[ TEST ] ADC water %d", val);


        l_i_("[ TEST ] all #%d end\n\n", _i++);
    }
}



void run_tests()
{
    l_i_("[ BLE ] running TESTS");
    l_i_("----------------------");


    #if 0
    test_ble_as_peripheral();
    #endif

    #if 1
    test_ble_as_central();
    #endif

    #if 0
    test_motor();
    #endif

    #if 0
    test_led_strip();
    #endif

    #if 0
    test_battery_measurement();
    #endif

    #if 0
    test_water_measurement();
    #endif

    #if 0
    test_all();
    #endif
}


