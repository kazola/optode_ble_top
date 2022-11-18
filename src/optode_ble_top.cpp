/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/kaz/git/optode_ble_top/src/optode_ble_top.ino"
#include "inc.h"



static void _tests();
static void _main();
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
        /* careful hal.ble may mask errors */               \
        lcf.append({ "hal", LOG_LEVEL_PANIC });             \
    }                                                       \
    SerialLogHandler logHandler(LOG_LEVEL_INFO, lcf);





SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);



static void _tests()
{
    _LOG_INIT_WITH_FILTERS_(1)
    _LOG_ROOM_MAIN_
    l_i_("[ BLE ] running TESTS");
    l_i_("----------------------");


    #if 0
    test_ble_as_peripheral();
    #endif

    #if 0
    test_ble_as_master();
    #endif

    #if 0
    test_motor();
    #endif

    #if 0
    test_led_strip();
    #endif

    #if 0
    test_battery_measurement();
    #endif

    #if 0
    test_water_measurement();
    #endif

    #if 1
    test_all();
    #endif


    _TRAP_AT_END_OF_TESTS_
}



static void _main()
{
    _LOG_INIT_WITH_FILTERS_(1)
    _LOG_ROOM_MAIN_
    l_i_("[ BLE ] running MAIN");
    l_i_("----------------------");
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
    pinMode(PIN_BATTERY_EN_OUT, OUTPUT);
    pinMode(PIN_WATER_EN_OUT, OUTPUT);
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
