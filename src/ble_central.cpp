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



const char * ble_get_mini_mac_from_core_mac(char letter)
{
    const char * mac = BLE.address().toString().c_str();

    if (!strncmp(mac, "D7:0E:86:06:96:E5", 17)) 
    {
        if (letter == 'a') return MAC_OPTODE_MINI_1;
        if (letter == 'b') return MAC_OPTODE_MINI_2;
    }
    if (!strncmp(mac, "F0:BC:8C:34:15:14", 17)) 
    {
        if (letter == 'a') return MAC_OPTODE_MINI_3;
        if (letter == 'b') return MAC_OPTODE_MINI_4;
    }
    if (!strncmp(mac, "C1:59:FA:52:4E:10", 17)) 
    {
        if (letter == 'a') return MAC_OPTODE_MINI_5;
        if (letter == 'b') return MAC_OPTODE_MINI_6;
    }
    if (!strncmp(mac, "DD:ED:97:4F:86:4A", 17)) 
    {
        if (letter == 'a') return MAC_OPTODE_MINI_7;
        if (letter == 'b') return MAC_OPTODE_MINI_8;
    }
    if (!strncmp(mac, "D9:F2:66:AC:A7:39", 17)) 
    {
        if (letter == 'a') return MAC_OPTODE_MINI_9;
        if (letter == 'b') return MAC_OPTODE_MINI_10;
    }
    if (!strncmp(mac, "C9:40:A1:53:69:FB", 17)) 
    {
        if (letter == 'a') return MAC_OPTODE_MINI_11;
        if (letter == 'b') return MAC_OPTODE_MINI_12;
    }
    
    l_e_("[ BLE ] cen | cannot map optode mini mac!");
    return "";
}




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



static uint8_t _iris_power_on_set_scan(BleAddress m, char letter)
{
    // required vars
    uint8_t rv = 0;
    uint8_t ans[BLE_ANS_LEN] = {0};
    BlePeerDevice _p;
    l_e_("[ BLE ] cen | _iris_power_on_set_scan(%c)", letter);


    
    // ---------------
    // connect to mini
    // ----------------
     _p = BLE.connect(m);
     if (!_p.connected())
     {
         l_e_("[ BLE ] cen | failed connecting mini %c", letter);
         rv = 1;
         goto MY_END;
     }
     l_i_("[ BLE ] cen | connected mini %c", letter);
     c_r.onDataReceived(_on_data_rx, NULL);
     _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
     _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));



    // --------------------------
    // switch on mini display
    // --------------------------
    BLE_CMD("do");
    BLE_ANS(15000);
    rv = CK_ANS("do_ok", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | no display at mini %c", letter);
        rv = 2;
        goto MY_END;
    }
     BLE_CMD("di");
     BLE_ANS(500);
     rv = CK_ANS("di_on", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | cannot turn ON display at mini %c", letter);
        rv = 3;
        goto MY_END;
    }


    // ------------------------------
    // toggle ON mini scanner wheel
    // ------------------------------
     BLE_CMD("wh");
     BLE_ANS(2000);
     rv = CK_ANS("wh_ok", 5);
     if (!rv)
     {
         l_e_("[ BLE ] cen | no wheel at mini %c", letter);
         rv = 4;
         goto MY_END;
     }


MY_END:
     BLE.disconnect(_p);
     l_i_("[ BLE ] cen | disconnected from mini %c", letter);
     return rv;
}



static uint8_t _iris_unset_scan_wifi_on(BleAddress m, char letter)
{
    // required vars
    uint8_t rv = 0;
    uint8_t ans[BLE_ANS_LEN] = {0};
    BlePeerDevice _p;
    l_e_("[ BLE ] cen | _iris_unset_scan_wifi_on(%c)", letter);


    
    // ---------------
    // connect to mini
    // ----------------
     _p = BLE.connect(m);
     if (!_p.connected())
     {
         l_e_("[ BLE ] cen | failed connecting mini %c", letter);
         rv = 1;
         goto MY_END;
     }
     l_i_("[ BLE ] cen | connected mini %c", letter);
     c_r.onDataReceived(_on_data_rx, NULL);
     _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
     _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));



    // ------------------------------
    // toggle OFF mini scanner wheel
    // ------------------------------
    BLE_CMD("wh");
    BLE_ANS(2000);
    rv = CK_ANS("wh_ok", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | no wheel at mini %c", letter);
        rv = 2;
        goto MY_END;
    }



    // ---------------------------------------------------
    // leave some time for image to get SAVE to SD CARD
    // ---------------------------------------------------
    l_e_("[ BLE ] cen | saving images at mini %c", letter);
    delay(10000);



    // ---------------
    // switch on wifi
    // ---------------
    if (g_wifi_while_sleep == 0)
    {
        l_i_("[ BLE ] cen | flag sleep_wifi is OFF, not activating it");
    }
    else
    {
        BLE_CMD("wo");
        BLE_ANS(2000);
        rv = CK_ANS("wo_ok", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | no wifi at mini %c", letter);
            rv = 3;
            goto MY_END;
        }
        BLE_CMD("wi");
        BLE_ANS(500);
        rv = CK_ANS("wi_on", 5);
        if (!rv)
        {
            l_e_("[ BLE ] cen | cannot turn ON wifi at mini %c", letter);
            rv = 4;
            goto MY_END;
        }
    }



MY_END:
     BLE.disconnect(_p);
     l_i_("[ BLE ] cen | disconnected from mini %c", letter);
     return rv;
}



static uint8_t _iris_power_off(BleAddress m, char letter)
{
    // required vars
    uint8_t rv = 0;
    uint8_t ans[BLE_ANS_LEN] = {0};
    BlePeerDevice _p;
    l_e_("[ BLE ] cen | _iris_power_off(%c)", letter);


    
    // ---------------
    // connect to mini
    // ----------------
     _p = BLE.connect(m);
     if (!_p.connected())
     {
         l_e_("[ BLE ] cen | failed connecting mini %c", letter);
         rv = 1;
         goto MY_END;
     }
     l_i_("[ BLE ] cen | connected mini %c", letter);
     c_r.onDataReceived(_on_data_rx, NULL);
     _p.getCharacteristicByUUID(c_r, BleUuid(UUID_R));
     _p.getCharacteristicByUUID(c_w, BleUuid(UUID_W));



    // --------------------------
    // switch OFF mini display
    // --------------------------
    BLE_CMD("do");
    BLE_ANS(4000);
    rv = CK_ANS("do_ok", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | no display at mini %c", letter);
        rv = 2;
        goto MY_END;
    }
     BLE_CMD("di");
     BLE_ANS(500);
     rv = CK_ANS("di_of", 5);
    if (!rv)
    {
        l_e_("[ BLE ] cen | cannot turn OFF display at mini %c", letter);
        rv = 3;
        goto MY_END;
    }


MY_END:
     BLE.disconnect(_p);
     l_i_("[ BLE ] cen | disconnected from mini %c", letter);
     return rv;
}



void ble_central_optode_core()
{
    // activate Bluetooth features
    BLE.on();
    BleAddress _a = BleAddress(MAC_OPTODE_MINI_A, BleAddressType::PUBLIC);
    BleAddress _b = BleAddress(MAC_OPTODE_MINI_B, BleAddressType::PUBLIC);



    // reset motor to left
    l_i_("[ BLE ] cen | inter-run, motor moving left");
    motor_move_left(12000);



    // power up irises and set them in scan mode
    _iris_power_on_set_scan(_a, 'A');
    _iris_power_on_set_scan(_b, 'B');



    // turn on the LED strip
    led_strip_on();
    delay(100);



    // move the motor towards right
    l_i_("[ BLE ] cen | motor moving right");
    motor_move_right(12000);



    // turn off the LED strip
    delay(100);
    led_strip_off();



    // make iris stop scanning, which saves to SD, and turn on wifi
    _iris_unset_scan_wifi_on(_a, 'A');
    _iris_unset_scan_wifi_on(_b, 'B');



    // ------------------------------------------
    // calculate time to sleep in this RUN cycle
    // ------------------------------------------
    uint32_t adjust_cycle_time = 137 * 1000;
    uint32_t time_to_dl = (g_v_it * 60 * 1000) - adjust_cycle_time;



    // order matters, always power off scanners to start next run clean
    if (g_wifi_while_sleep)
    {
        const char * s = "[ BLE ] cen | allow %ld seconds to download files";
        l_i_(s, time_to_dl / 1000);
        delay(time_to_dl);

        l_i_("[ BLE ] cen | powering off irises");
        _iris_power_off(_a, 'A');
        _iris_power_off(_b, 'B');
    }
    else
    {
        l_i_("[ BLE ] cen | powering off irises");
        _iris_power_off(_a, 'A');
        _iris_power_off(_b, 'B');
        const char * s = "[ BLE ] cen | allow %ld seconds to save power";
        l_i_(s, time_to_dl / 1000);
        delay(time_to_dl);
    }



    // deactivate Bluetooth features
    BLE.off();
}
