#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");
    l_i_("[ AUT ] optode mini A mac %s", MAC_OPTODE_MINI_A);
    l_i_("[ AUT ] optode mini B mac %s", MAC_OPTODE_MINI_B);



    // ensure we boot at leftmost motor position
    l_i_("[ AUT ] cen | boot: motor moving left");
    motor_move_left(10000);



    // --------------------------------------------
    // upon boot, does core accept commands or not
    // --------------------------------------------
    #if 1
        l_i_("[ AUT ] starting with mode CONF");
        BLE.selectAntenna(BleAntennaType::EXTERNAL);
        ble_peripheral_optode_core();
    #endif
    

    // banner mode RUN
    const char * s = "[ AUT ] now going to mode RUN, it = %dm, sw %d";
    l_i_(s, g_v_it, g_sleep_wifi);
    BLE.selectAntenna(BleAntennaType::INTERNAL);
    uint16_t run_num = 0;


    // loop mode RUN
    while (1)
    {
        ble_central_optode_core();
        l_i_("[ AUT ] run #%d", run_num++);


        l_i_("[ AUT ] delay 10 seconds more to rest");
        delay(10000);
    }
}

