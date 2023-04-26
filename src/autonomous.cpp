#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");



    #if 1
        // ---------------------
        // FSM fully automatic
        // ---------------------
        while (1)
        {
            l_i_("[ AUT ] going straight to mode run");
            BLE.selectAntenna(BleAntennaType::INTERNAL);
            ble_central_optode_core_manage_both_optode_minis();

            
            uint32_t time_to_dl = 330000;
            l_i_("[ AUT ] allow %d secs to download", time_to_dl / 1000);
            delay(time_to_dl);
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
            }

            else if (rv == 2)
            {
                l_i_("[ AUT ] boot | going to mode download");
            }

            
            l_i_("[ AUT ] allow time to download scanner wifis");
            delay(10000);
        }
    #endif
}

