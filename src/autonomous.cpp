#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");
    l_i_("[ AUT ] optode mini A mac %s", MAC_OPTODE_MINI_A);
    l_i_("[ AUT ] optode mini B mac %s", MAC_OPTODE_MINI_B);


    #if 1
        l_i_("[ AUT ] starting with mode CONF");
        BLE.selectAntenna(BleAntennaType::EXTERNAL);
        ble_peripheral_optode_core();
    #endif
    

    // banner mode RUN
    l_i_("[ AUT ] now going to mode RUN, it = %dm", g_v_it);
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

