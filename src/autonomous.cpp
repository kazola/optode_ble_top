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
        ble_peripheral_optode_core();


        BLE.selectAntenna(BleAntennaType::INTERNAL);
        ble_central_optode_core_manage_both_optode_minis();


        l_i_("[ AUT ] allow time to download scanner wifis");
        delay(10000);
    }
}

