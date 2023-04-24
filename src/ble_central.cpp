#include "ble_central.h"



// needed to communicate with optode mini
const char * UUID_R = "2324";
const char * UUID_W = "2325";



#define BLE_ANS_LEN                             (20)

#define BLE_MINI_SHOW(i, mac, n)                \
    l_i_("[ BLE ] optode mini #%d found", i);   \
    l_i_("[ BLE ] mini mac   %s", mac);         \
    l_i_("[ BLE ] mini name  %s", n);

#define BLE_CMD                                 \
    memset(ans, 0, BLE_ANS_LEN);                \
    c_w.setValue

#define BLE_ANS(t)                              \
    delay(100);                                 \
    c_r.getValue(ans, 10);                      \
    delay(t);

#define CK_ANS(b, n) !strncmp((const char *)ans, b, n);



// will host characteristics from the mini
BleCharacteristic c_r;
BleCharacteristic c_w;



void ble_central_scan_for_optode_minis(uint8_t * mask)
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

        if (!strncmp(MAC_OPTODE_MINI_A, mac, _APP_BLE_MAC_LEN_))
        {
            BLE_MINI_SHOW(1, mac, _r);
            *mask |= 1 << 0;
        }
        if (!strncmp(MAC_OPTODE_MINI_B, mac, _APP_BLE_MAC_LEN_))
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
    l_i_("[ BLE ] cen | rx -> %s (len %d)", s, len);
}



uint8_t ble_central_test_optode_mini(const char * mac)
{
    BleAddress _a = BleAddress(mac, BleAddressType::PUBLIC);
    BlePeerDevice _p = BLE.connect(_a);


 	if (!_p.connected())
    {
        l_e_("[ BLE ] failed connecting %s", mac);
        return 1;

    }


    // make BLE mini do what we want
    l_i_("[ BLE ] central | connected to BLE optode mini peripheral");
    l_i_("[ BLE ] central | the mini mac is %s", mac);
    c_r.onDataReceived(_on_data_rx, NULL);
    const char * UUID_R = "2324";
    const char * UUID_W = "2325";
    _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
    _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));


    // command answer ends up in MACRO variable 'ans'
    uint8_t ans[20];
    while (1)
    {
        l_i_("\n");
        l_i_("\n[ BLE ] central | tx <- %s", "le_wo_do_wi_di");
        memset(ans, 0, 20);
        BLE_CMD("le");
        BLE_ANS(2000);
        BLE_CMD("wo");
        BLE_ANS(500);
        BLE_CMD("do");
        BLE_ANS(500);
        BLE_CMD("wi");
        BLE_ANS(500);
        BLE_CMD("di");
        BLE_ANS(500);
        delay(3000);
    }


    // end!
    BLE.disconnect(_p);
    l_i_("[ BLE ] central | disconnected from BLE optode mini");
    return 0;
}



static void _central_pre_mini(BleAddress m, char letter)
{
    // required vars
    uint8_t rv;
    uint8_t ans[BLE_ANS_LEN] = {0};
    BlePeerDevice _p;
    l_e_("[ BLE ] cen | running pre_mini %c", letter);



     // ---------------
    // connect to mini
    // ----------------
    _p = BLE.connect(m);
    if (!_p.connected())
    {
        l_e_("[ BLE ] cen | failed connecting mini %c", letter);
        goto MY_END;
    }
    l_i_("[ BLE ] cen | connected mini %c", letter);
    c_r.onDataReceived(_on_data_rx, NULL);
    _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
    _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));



    // --------------------------
    // ensure mini display is ON
    // --------------------------
    BLE_CMD("di");
    BLE_ANS(500);
    rv = CK_ANS("di_on", 5);
    if (!rv)
    {
        // try to start the display
        l_i_("[ BLE ] cen | powering display on mini %c, wait 15 seconds", letter);
        BLE_CMD("do");
        BLE_ANS(15000);
        rv = CK_ANS("do_ok", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | no display on mini %c", letter);
            goto MY_END;
        }

        BLE_CMD("di");
        BLE_ANS(500);
        rv = CK_ANS("di_on", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | cannot turn on display on mini %c", letter);
            goto MY_END;
        }
    }



    // ------------------------
    // ensure mini wifi is OFF
    // ------------------------
    BLE_CMD("wi");
    BLE_ANS(500);
    rv = CK_ANS("wi_of", 5);
    if (!rv)
    {
        // try to turn off the wi-fi
        BLE_CMD("wo");
        BLE_ANS(4000);
        rv = CK_ANS("wo_ok", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | no wifi on mini %c", letter);
            goto MY_END;
        }

        BLE_CMD("wi");
        BLE_ANS(500);
        rv = CK_ANS("wi_of", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | cannot turn off wi-fi on mini %c", letter);
            goto MY_END;
        }
    }



    // ------------------------------
    // toggle ON mini scanner wheel
    // ------------------------------
    BLE_CMD("wh");
    BLE_ANS(2000);
    rv = CK_ANS("wh_ok", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | no wheel on mini %c", letter);
        goto MY_END;
    }


MY_END:
    BLE.disconnect(_p);
    l_i_("[ BLE ] cen | disconnected from mini %c", letter);

}



static void _central_post_mini(BleAddress m, char letter)
{
    // required vars
    uint8_t rv;
    uint8_t ans[BLE_ANS_LEN] = {0};
    BlePeerDevice _p;
    l_e_("[ BLE ] cen | running post_mini %c", letter);



    // ---------------
    // connect to mini
    // ----------------
    _p = BLE.connect(m);
    if (!_p.connected())
    {
        l_e_("[ BLE ] cen | failed connecting mini %c", letter);
        goto MY_END;
    }
    l_i_("[ BLE ] cen | connected mini %c", letter);
    c_r.onDataReceived(_on_data_rx, NULL);
    _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
    _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));



    // --------------------------
    // ensure mini display is ON
    // --------------------------
    BLE_CMD("di");
    BLE_ANS(500);
    rv = CK_ANS("di_on", 5);
    if (!rv)
    {

        l_e_("[ BLE ] cen | unexpected display mini %c", letter);
        goto MY_END;
    }


    // ------------------------------
    // toggle OFF mini scanner wheel
    // ------------------------------
    BLE_CMD("wh");
    BLE_ANS(500);
    rv = CK_ANS("wh_ok", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | no wheel on mini %c", letter);
        goto MY_END;
    }



    // ------------------------
    // ensure mini wifi is ON
    // ------------------------
    BLE_CMD("wi");
    BLE_ANS(500);
    rv = CK_ANS("wi_on", 5);
    if (!rv)
    {
        // try to turn off the wi-fi
        BLE_CMD("wo");
        BLE_ANS(4000);
        rv = CK_ANS("wo_ok", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | no wifi on mini %c", letter);
            goto MY_END;
        }

        // wifi read again
        BLE_CMD("wi");
        BLE_ANS(500);
        rv = CK_ANS("wi_of", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | cannot turn off wi-fi on mini %c", letter);
            goto MY_END;
        }
    }


MY_END:
    BLE.disconnect(_p);
    l_i_("[ BLE ] cen | disconnected from mini %c", letter);

}



void ble_central_optode_core_manage_both_optode_minis()
{
    BLE.on();
    BleAddress _a = BleAddress(MAC_OPTODE_MINI_A, BleAddressType::PUBLIC);
    BleAddress _b = BleAddress(MAC_OPTODE_MINI_B, BleAddressType::PUBLIC);


    l_i_("[ BLE ] cen | motor moving left");
    motor_move_left(10000);


    // wifi off, display on, wheel on
    _central_pre_mini(_a, 'A');
    _central_pre_mini(_b, 'B');


    l_i_("[ BLE ] cen | motor moving right");
    motor_move_right(10000);


    // wheel off, wifi on
    _central_post_mini(_a, 'A');
    _central_post_mini(_b, 'B');


   

    BLE.off();
}
