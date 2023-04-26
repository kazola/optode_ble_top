#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");



    l_i_("[ AUT ] boot | motor moving left");
    motor_move_left(10000);


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

        l_i_("[ AUT ] allow time to download scanner wifis");
        delay(10000);
    }
}

