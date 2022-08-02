#include "ble_base.h"



void ble_clr_data_buf()
{
    memset(ble_data, 0, _APP_BLE_DATA_BUFFER_LEN_);
    ble_data_i = 0;
}



void ble_scan_for_loggers()
{
    uint8_t t = _APP_BLE_SCAN_DURATION_S_;
    l_i_("\n\n\n");
    l_i_("[ BLE ] scanning for devices during %d seconds...", t);
    BLE.setScanTimeout(_APP_BLE_SCAN_TIMEOUT_S(t));
    BleScanResult s_r[_APP_BLE_SCAN_RESULT_MAX_LEN_];
    uint8_t n = BLE.scan(s_r, _APP_BLE_SCAN_RESULT_MAX_LEN_);


    for (uint8_t i = 0; i < n; i++)
    {
        uint8_t lnl = _APP_BLE_LOGGER_NAME_LEN_;
        uint8_t lml = _APP_BLE_LOGGER_MAC_LEN_;
        char _s[lnl] = {0};
        char mac[lml] = {0};
        char _r[lnl] = {0};
        strncpy(_s, s_r[i].advertisingData().deviceName().c_str(), lnl - 1);
        strncpy(_r, s_r[i].scanResponse().deviceName().c_str(), lnl - 1);
        strncpy(mac, s_r[i].address().toString().c_str(), lml - 1);
        l_i_("[ BLE ] device found %s - name %s - name2 %s", mac, _s, _r);
    }
}



static void _on_data_rx(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context)
{
	char s[_APP_BLE_NOTIFICATION_LEN_] = {0};
	assert(len <= _APP_BLE_NOTIFICATION_LEN_);
	memcpy(s, data, len);
     l_i_("[ BLE ] incoming notif -> data %s (len %d)", s, len);
    assert(ble_data_i + len <= _APP_BLE_DATA_BUFFER_LEN_);
    memcpy(&ble_data[ble_data_i], data, len);
    ble_data_i += len;
}



uint8_t ble_interact_optode_mini(const char * mac)
{
    BleAddress _a = BleAddress(mac, BleAddressType::PUBLIC);
    BlePeerDevice _p = BLE.connect(_a);


 	if (_p.connected())
    {
        l_i_("[ BLE ] connected to BLE optode mini, mac = %s", mac);
        c_r.onDataReceived(_on_data_rx, NULL);
        const char * UUID_R = "2324";
        const char * UUID_W = "2325";
        _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
        _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));


        c_w.setValue("L");
        delay(100);
        uint8_t ans[10] = {0};
        c_r.getValue(ans, 10);
        l_i_("[ BLE ] answer read -> %s", ans);


        BLE.disconnect(_p);
        l_i_("[ BLE ] disconnected BLE optode mini, mac = %s", mac);
        return 0;
    }

    l_e_("[ BLE ] failed connecting %s", mac);
    return 1;
}