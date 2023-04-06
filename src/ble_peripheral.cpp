#include "ble_peripheral.h"



static volatile uint8_t _tell_connection_but_just_once;
void on_data_rx_as_peripheral(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, on_data_rx_as_peripheral, NULL);
static volatile uint8_t end_of_conf;
static volatile uint8_t counter_macs;



static void _tx_ans(const char * a)
{
    txCharacteristic.setValue((const uint8_t *)a, strlen(a));
}



static uint8_t _cmd_is(const uint8_t * c, const char * s)
{
    return !strncmp((const char *)c, s, 2);
}



void on_data_rx_as_peripheral
(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) 
{
    // print command from central BLE device
    Serial.write("-> ");
    for (size_t ii = 0; ii < len; ii++)
    {
         Serial.write(data[ii]);
    }
    Serial.write("\n\r");


    // parse incoming commands from laptop or phone
    if (_cmd_is(data, "it"))
    {
        _tx_ans("inc_time_ok");
    }

    else if (_cmd_is(data, "ru"))
    {
        _tx_ans("run_ok");
        end_of_conf = 1;
    }

    else if (_cmd_is(data, "ma"))
    {
        if (counter_macs % 2)
        {
            _tx_ans(MAC_OPTODE_MINI_1);
        }
        else
        {
            _tx_ans(MAC_OPTODE_MINI_2);
        }
        counter_macs++;
    }

    else if (_cmd_is(data, "st"))
    {
        _tx_ans("st_booting");
    }

    else if (_cmd_is(data, "ml"))
    {
        motor_move_left(1000);
        _tx_ans("ml_ok");
    }

    else if (_cmd_is(data, "mr"))
    {
        _tx_ans("mr_ok");
        motor_move_right(1000);
    }

    else if (_cmd_is(data, "lo"))
    {
        led_strip_on();
        _tx_ans("lo_ok");
    }

    else if (_cmd_is(data, "lf"))
    {
        led_strip_off();
        _tx_ans("lf_ok");
    }

    else if (_cmd_is(data, "ba"))
    {
        _tx_ans("3000");
    }

    else if (_cmd_is(data, "m1"))
    {
        // todo: save to eeprom
        _tx_ans("m1_ok");
    }

    else if (_cmd_is(data, "m2"))
    {
        _tx_ans("m2_ok");
    }

    else
    {
        _tx_ans("ERR");
    }
}



void ble_peripheral_optode_core()
{
    const char * mac = BLE.address().toString().c_str();
    l_i_("[ BLE ] peripheral | optode core start");
    l_i_("[ BLE ] peripheral | optode core MAC address = %s", mac);


    // advertising information
    BLE.on();
    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);
    BleAdvertisingData _a;
    _a.appendServiceUUID(serviceUuid);
    _a.appendLocalName("op_co_");
	BLE.advertise(&_a);



    // loop till we receive the end_of_conf character == '/'
    while (1)
    {
        if (BLE.connected() && !_tell_connection_but_just_once)
        {
            // show this once
            l_i_("[ BLE ] peripheral | optode ready to be configured");
            _tell_connection_but_just_once = 1;
        }

        if (end_of_conf)
        {
            break;
        }
        delay(1000);
    }
    BLE.off();


    l_i_("[ BLE ] peripheral | optode end configuration");
}