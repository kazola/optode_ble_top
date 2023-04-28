#include "ble_peripheral.h"



#define OP_CO_NAME_LEN  (10)



void ble_get_name_from_optode_core_mac(const char * mac, char * name)
{
    char found = 0;


    if (!strncmp(mac, "D7:0E:86:06:96:E5", 17)) found = '1';
    if (!strncmp(mac, "F0:BC:8C:34:15:14", 17)) found = '2';
    if (!strncmp(mac, "C1:59:FA:52:4E:10", 17)) found = '3';
    if (!strncmp(mac, "DD:ED:97:4F:86:4A", 17)) found = '4';
    if (!strncmp(mac, "D9:F2:66:AC:A7:39", 17)) found = '5';
    if (!strncmp(mac, "C9:40:A1:53:69:FB", 17)) found = '6';
    if (!strncmp(mac, "E3:04:61:7F:EC:09", 17)) found = '7';

    if (found)
    {
        memset(name, 0, OP_CO_NAME_LEN);
        memcpy((uint8_t *)name, (uint8_t *)"op_co_", 6);
        name[6] = found;
    }
}




static volatile uint8_t _tell_connection_but_just_once;
void on_data_rx_as_peripheral(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, on_data_rx_as_peripheral, NULL);
static volatile uint8_t _go_to_mode_run;
static volatile uint8_t counter_macs;



static void _tx_ans(const char * a)
{
    txCharacteristic.setValue((const uint8_t *)a, strlen(a));
    Serial.write("<- ");
    Serial.write(a);
    Serial.write("\r\n");
}



static uint8_t _cmd_is(const uint8_t * c, const char * s)
{
    return !strncmp((const char *)c, s, 2);
}



// -----------------------------------------------
// this manages COMMANDS when CORE as peripheral
// -----------------------------------------------
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


    // ------------------------------
    // todo: cmd set time interruns
    // todo: cmd set speed
    // todo: cmd set macs
    // * second time onwards, wifi is on here
    // ------------------------------


    // parse incoming commands from laptop or phone
    if (_cmd_is(data, "it"))
    {
        if (g_v_it == 5)
        {
            g_v_it = 15;
            _tx_ans("15m");
        }
        else if (g_v_it == 15)
        {
            g_v_it = 30;
            _tx_ans("30m");
        }
        else if (g_v_it == 30)
        {
            g_v_it = 5;
            _tx_ans("5m");
        }
    }

    else if (_cmd_is(data, "sw"))
    {
        g_sleep_wifi ^= 1;
        if (g_sleep_wifi == 0)
        {
            _tx_ans("sw_on");
        }
        else
        {
            _tx_ans("sw_off");
        }
    }

    else if (_cmd_is(data, "ru"))
    {
        _tx_ans("run_ok");
        _go_to_mode_run = 1;
    }

    else if (_cmd_is(data, "ma"))
    {
        if (counter_macs % 2)
        {
            _tx_ans(MAC_OPTODE_MINI_A);
        }
        else
        {
            _tx_ans(MAC_OPTODE_MINI_B);
        }
        counter_macs++;
    }

    else if (_cmd_is(data, "st"))
    {
        _tx_ans("st_booting");
    }

    else if (_cmd_is(data, "ml"))
    {
        motor_move_left(10000);
        _tx_ans("ml_ok");
    }

    else if (_cmd_is(data, "mr"))
    {
        motor_move_right(10000);
        _tx_ans("mr_ok");
    }

    else if (_cmd_is(data, "ll"))
    {
        if (is_motor_left_limit_hit())
        {
            _tx_ans("ll_1");
        }
        else
        {
            _tx_ans("ll_0");
        }
    }

    else if (_cmd_is(data, "lr"))
    {
        if (is_motor_right_limit_hit())
        {
            _tx_ans("lr_1");
        }
        else
        {
            _tx_ans("lr_0");
        }
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


static uint8_t only_add_characteristics_once = 0;
void ble_peripheral_optode_core()
{
    const char * mac = BLE.address().toString().c_str();
    l_i_("[ BLE ] per | optode core start");
    l_i_("[ BLE ] per | optode core MAC @ = %s", mac);


    // name: "op_co_" + last 2 digits of MAC
    char name[OP_CO_NAME_LEN] = {0};
    memcpy((uint8_t *)name, (uint8_t *)"op_co_", 6);
    name[6] = mac[15];
    name[7] = mac[16];


    // try to do even a friendlier name
    ble_get_name_from_optode_core_mac(mac, name);


    // advertising information
    BLE.on();
    if (only_add_characteristics_once == 0)
    {
        BLE.addCharacteristic(txCharacteristic);
        BLE.addCharacteristic(rxCharacteristic);
        only_add_characteristics_once = 1;
    }

    BleAdvertisingData _a;
    _a.appendServiceUUID(serviceUuid);
    _a.appendLocalName((const char *)name);
	BLE.advertise(&_a);


    // loop till we change the mode of operation
    _tell_connection_but_just_once = 0;
    _go_to_mode_run = 0;

    while (1)
    {
        if (BLE.connected() && !_tell_connection_but_just_once)
        {
            // show this once
            l_i_("[ BLE ] per | optode ready to be configured");
            _tell_connection_but_just_once = 1;
        }

        if (_go_to_mode_run)
        {
            break;
        }
        delay(1000);
    }
    BLE.off();


    l_i_("[ BLE ] per | optode end configuration");
}