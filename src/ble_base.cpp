#include "ble_base.h"



#define BLE_MINI_SHOW(i, mac, n)                \
    l_i_("[ BLE ] optode mini #%d found", i);   \
    l_i_("[ BLE ] mini mac   %s", mac);         \
    l_i_("[ BLE ] mini name  %s", n);
#define BLE_CMD     c_w.setValue
#define BLE_ANS(t)                              \
    delay(100);                                 \
    c_r.getValue(ans, 10);                      \
    l_i_("[ BLE ] r_ans -> %s", ans);           \
    delay(t);




void ble_scan_for_optode_minis(uint8_t * mask)
{
    uint8_t t = _APP_BLE_SCAN_DURATION_S_;
    l_i_("\n\n\n");
    l_i_("[ BLE ] detecting optode minis ...");
    BLE.setScanTimeout(_APP_BLE_SCAN_TIMEOUT_S(t));
    BleScanResult s_r[_APP_BLE_SCAN_RESULT_MAX_LEN_];
    uint8_t n = BLE.scan(s_r, _APP_BLE_SCAN_RESULT_MAX_LEN_);


    for (uint8_t i = 0; i < n; i++)
    {
        uint8_t lnl = _APP_BLE_NAME_LEN_;
        uint8_t lml = _APP_BLE_MAC_LEN_ + 1;
        char _s[lnl] = {0};
        char mac[lml] = {0};
        char _r[lnl] = {0};
        strncpy(_s, s_r[i].advertisingData().deviceName().c_str(), lnl - 1);
        strncpy(_r, s_r[i].scanResponse().deviceName().c_str(), lnl - 1);
        strncpy(mac, s_r[i].address().toString().c_str(), lml - 1);

        // l_i_("[ BLE ] device found %s ADV name %s", mac, _s);

        if (!strncmp(MAC_OPTODE_MINI_1, mac, _APP_BLE_MAC_LEN_))
        {
            BLE_MINI_SHOW(1, mac, _r);
            *mask |= 1 << 0;
        }
        if (!strncmp(MAC_OPTODE_MINI_2, mac, _APP_BLE_MAC_LEN_))
        {
            BLE_MINI_SHOW(2, mac, _r);
            *mask |= 1 << 1;
        }
    }
}



static void _on_data_rx(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context)
{
	char s[_APP_BLE_NOTIFICATION_LEN_] = {0};
	assert(len <= _APP_BLE_NOTIFICATION_LEN_);
	memcpy(s, data, len);
    l_i_("[ BLE ] notif -> %s (len %d)", s, len);
}



uint8_t ble_interact_optode_mini(const char * mac)
{
    BleAddress _a = BleAddress(mac, BleAddressType::PUBLIC);
    BlePeerDevice _p = BLE.connect(_a);


 	if (!_p.connected())
    {
        l_e_("[ BLE ] failed connecting %s", mac);
        return 1;

    }


    // make BLE mini do what we want
    l_i_("[ BLE ] connected BLE optode mini, mac = %s", mac);
    c_r.onDataReceived(_on_data_rx, NULL);
    const char * UUID_R = "2324";
    const char * UUID_W = "2325";
    _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
    _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));


    // 'l': led
    uint8_t ans[10] = {0};
    BLE_CMD("l");
    BLE_ANS(2000);


    // command answer is in variable 'ans'


    // end!
    BLE.disconnect(_p);
    l_i_("[ BLE ] disconnected BLE optode mini, mac = %s", mac);
    return 0;
}