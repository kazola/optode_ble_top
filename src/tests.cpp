#include "tests.h"
#include "Particle.h"



// must be defined here, outside
const size_t UART_TX_BUF_SIZE = 20;
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

static volatile uint8_t end_of_conf;
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) 
{
    Serial.write("> ");
    for (size_t ii = 0; ii < len; ii++) 
         Serial.write(data[ii]);
    Serial.write("\n\r");
    end_of_conf = data[0] == '/';
}



static volatile uint8_t _tell_someone_connected;
void test_ble_as_peripheral()
{
    const char * mac = BLE.address().toString().c_str();
    l_i_("[ BLE ] peripheral optode core MAC address = %s", mac);


    uint16_t time_during_we_can_conf_optode = 50000;
    l_i_("[ BLE ] optode waiting to receive configuration via Bluetooth");


    // advertising information
    BLE.on();
    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);
    BleAdvertisingData _a;
    _a.appendServiceUUID(serviceUuid);
    _a.appendLocalName("optode");
	BLE.advertise(&_a);
    while (!end_of_conf)
    {
        if (BLE.connected() && !_tell_someone_connected)
        {
            // show this once
            l_i_("[ BLE ] optode ready to receive configuration");
            _tell_someone_connected = 1;
        }
        delay(1000);
    }
    BLE.off();

    l_i_("[ BLE ] finished optode configuration");
    l_i_("[ BLE ] starting optode running mode");
}



void test_ble_as_master()
{
    if ((strlen(MAC_OPTODE_MINI_1) != _APP_BLE_MAC_LEN_) ||    \
        (strlen(MAC_OPTODE_MINI_2) != _APP_BLE_MAC_LEN_))
    {
        l_e_("[ BLE ] optode_mini MACs bad length");
    }

    
    const char * mac = BLE.address().toString().c_str();
    l_i_("[ BLE ] master optode core MAC address = %s", mac);


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


