#include "tests.h"



void test_ble_as_master()
{
        if ((strlen(MAC_OPTODE_MINI_1) != _APP_BLE_MAC_LEN_) ||    \
        (strlen(MAC_OPTODE_MINI_2) != _APP_BLE_MAC_LEN_))
    {
        l_e_("[ BLE ] scanner MACs bad length");
    }

    BLE.on();
    while (1)
    {
        uint8_t mask = 0;
        ble_scan_for_optode_minis(&mask);

        l_i_("[ BLE ] optode mini detected mask = %d", mask);
        // if (mask != 3) continue;

        ble_interact_optode_mini(MAC_OPTODE_MINI_1);
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


void test_ble_as_peripheral()
{
    BLE.on();

    const char * mac = BLE.address().toString().c_str();
    l_i_("[ BLE ] optode core MAC address = %s", mac);

    BleAdvertisingData _a;
    const char * s = "bat = 13V";
    _a.appendLocalName("optode");
    _a.appendCustomData((const uint8_t *) s, strlen(s));
	BLE.advertise(&_a);
    delay(20000);

    BLE.off();
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


