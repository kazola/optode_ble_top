/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/kaz/git/optode_ble_top/src/optode_ble_top.ino"
#include "inc.h"



static void _tests();
void setup();
#line 5 "c:/Users/kaz/git/optode_ble_top/src/optode_ble_top.ino"
#define _RUN_MAIN_          0
#define _RUN_TESTS_         1
#define _RUN_CHOSEN_        _RUN_TESTS_



#define _TRAP_AT_END_OF_TESTS_          while(1) { delay (10); }
#define _DELAY_ATTACH_UART_MONITOR_     delay(2000);



#define _LOG_INIT_WITH_FILTERS_(b)                          \
    LogCategoryFilters lcf = {};                            \
    if (b)                                                  \
    {                                                       \
        lcf.append({ "app", LOG_LEVEL_INFO });              \
        lcf.append({ "ncp", LOG_LEVEL_WARN });              \
        lcf.append({ "system", LOG_LEVEL_WARN });           \
        lcf.append({ "net", LOG_LEVEL_WARN });              \
        lcf.append({ "wiring", LOG_LEVEL_WARN });           \
    }                                                       \
    SerialLogHandler logHandler(LOG_LEVEL_INFO, lcf);





SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);



static void _tests()
{
    _LOG_INIT_WITH_FILTERS_(1)
    _LOG_ROOM_MAIN_
    l_i_("[ BOOT ] running TESTS");
    l_i_("----------------------");





    // ---------------------------------
    // start periodic background thread
    // ---------------------------------
    // new Thread("th_periodic", th_fxn_periodic);
    // new Thread("th_leds", th_fxn_leds);


    // -----------
    // BLE test
    // -----------

    // BLE.on();
    // while (1)
    // {
    //     ble_scan_for_loggers();
    //     const char * mac = "FE:7D:29:D8:7D:67";
    //     ble_interact_optode_mini(mac);
    // }
    // BLE.off();


    // -----------
    // motor test
    // -----------

    #if 0
    while (1)
    {
        uint16_t t_ms = 3000;
        motor_set_resolution(0);
        motor_move_right(t_ms);
        delay(t_ms);
        motor_move_left(t_ms);
        delay(t_ms);
    }
    #endif


    // ----------------
    // LED stripe test
    // ----------------
    #if 0
    while (1)
    {
        _dW_(PIN_LED_STRIP_OUT, 1);
        delay(1000);
        _dW_(PIN_LED_STRIP_OUT, 0);
        delay(1000);
    }
    #endif


    // -----------------------
    // BAT measurement test
    // -----------------------
    while (1)
    {
        _dW_(PIN_ADC_BATTERY_OUT, 1);
        int val = _aR_(PIN_ADC_BATTERY_IN);
        _dW_(PIN_ADC_BATTERY_OUT, 0);


        // tell UART
        l_i_("battery ADC value %d", val);
        delay(100);
    }


    _TRAP_AT_END_OF_TESTS_
}



void setup()
{
    _DELAY_ATTACH_UART_MONITOR_

    // ------------
    // pins setup
    // -------------
    //pinMode(PIN_MOTOR_LIMIT_LEFT_IN, INPUT_PULLUP);
    //pinMode(PIN_MOTOR_LIMIT_RIGHT_IN, INPUT_PULLUP);
    pinMode(PIN_LED_STRIP_OUT, OUTPUT);
    pinMode(PIN_MOTOR_EN_OUT, OUTPUT);
    pinMode(PIN_MOTOR_MS1_OUT, OUTPUT);
    pinMode(PIN_MOTOR_MS2_OUT, OUTPUT);
    pinMode(PIN_MOTOR_STEP_OUT, OUTPUT);
    pinMode(PIN_MOTOR_DIR_OUT, OUTPUT);
    pinMode(PIN_ADC_BATTERY_OUT, OUTPUT);
    digital_write_motor_pins_reset();


    #if _RUN_CHOSEN_ == _RUN_MAIN_
        _main();
    
    #elif _RUN_CHOSEN_ == _RUN_TESTS_
        _tests();
    
    #elif _RUN_CHOSEN_ == _RUN_CUSTOM_
        _setup_custom();
    
    #else
        l_e_("[ DDP ] choose _run_main or _run_tests!");
        assert(FALSE);
    #endif
}
