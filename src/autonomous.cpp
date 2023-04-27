#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");
    l_i_("optode mini A mac %s", MAC_OPTODE_MINI_A);
    l_i_("optode mini B mac %s", MAC_OPTODE_MINI_B);



    #if 0
        // ---------------------
        // FSM fully automatic
        // ---------------------
        
        l_i_("[ AUT ] going straight to mode RUN");
        BLE.selectAntenna(BleAntennaType::INTERNAL);
        

        uint16_t run_num = 0;
        while (1)
        {
            ble_central_optode_core();
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
            l_i_("[ BLE ] cen | motor moving left");
            motor_move_left(10000);


            BLE.selectAntenna(BleAntennaType::EXTERNAL);
            uint8_t rv = ble_peripheral_optode_core();


            if (rv == 1)
            {
                l_i_("[ AUT ] boot | going to mode run");
                BLE.selectAntenna(BleAntennaType::INTERNAL);
                ble_central_optode_core();
            }

            else if (rv == 2)
            {
                l_i_("[ AUT ] boot | going to mode download");
            }
        }
    #endif
}

