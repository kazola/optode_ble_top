#include "autonomous.h"



void run_autonomous()
{
    l_i_("[ RUN ] AUTONOMOUS");
    l_i_("------------------");


    motor_move_left();
    while (1)
    {
        ble_peripheral_optode_core();
        ble_central_optode_core_manage_both_optode_minis();
    }
}

