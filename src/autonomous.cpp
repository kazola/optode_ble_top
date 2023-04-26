#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");



    #if 0
        // ---------------------
        // FSM fully automatic
        // ---------------------
        
        l_i_("[ AUT ] going straight to mode RUN");
        BLE.selectAntenna(BleAntennaType::INTERNAL);
        

        uint16_t run_num = 0;
        while (1)
        {
            ble_central_optode_core_manage_both_optode_minis();
            l_i_("[ AUT ] run #%d", run_num++);


            l_i_("[ AUT ] delay 10 seconds more to rest");
            delay(10000);
        }


    #else
        // --------------------------
        // FSM allows choosing modes
        // --------------------------
        while (1)
        {
            BLE.selectAntenna(BleAntennaType::EXTERNAL);
            uint8_t rv = ble_peripheral_optode_core();


            if (rv == 1)
            {
                l_i_("[ AUT ] boot | going to mode run");
                BLE.selectAntenna(BleAntennaType::INTERNAL);
                ble_central_optode_core_manage_both_optode_minis();
            }

            else if (rv == 2)
            {
                l_i_("[ AUT ] boot | going to mode download");
            }
        }
    #endif
}

