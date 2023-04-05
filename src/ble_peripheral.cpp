#include "ble_peripheral.h"



static volatile uint8_t _tell_connection_but_just_once;
void on_data_rx_as_peripheral(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, on_data_rx_as_peripheral, NULL);
static volatile uint8_t end_of_conf;



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
    if (!strncmp((const char *)data, "i", 1))
    {
        txCharacteristic.setValue((const uint8_t *)"inc_time_ok", 11);
    }
    else if (!strncmp((const char *)data, "/", 1))
    {
        txCharacteristic.setValue((const uint8_t *)"run_ok", 6);
        end_of_conf = 1;
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