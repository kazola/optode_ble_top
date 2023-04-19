#include "inc.h"



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



// empty on purpose
void loop() {}



void setup()
{
    _DELAY_ATTACH_UART_MONITOR_


    // configure logging
    _LOG_INIT_WITH_FILTERS_(1)
    _LOG_ROOM_
    

    // --------------------------------------------
    // pins setup, beware of motor limits floating
    // --------------------------------------------

    pinMode(PIN_MOTOR_LIMIT_LEFT_IN, INPUT_PULLUP);
    pinMode(PIN_MOTOR_LIMIT_RIGHT_IN, INPUT_PULLUP);
    pinMode(PIN_LED_STRIP_OUT, OUTPUT);
    pinMode(PIN_MOTOR_EN_OUT, OUTPUT);
    pinMode(PIN_MOTOR_MS1_OUT, OUTPUT);
    pinMode(PIN_MOTOR_MS2_OUT, OUTPUT);
    pinMode(PIN_MOTOR_STEP_OUT, OUTPUT);
    pinMode(PIN_MOTOR_DIR_OUT, OUTPUT);
    pinMode(PIN_BATTERY_EN_OUT, OUTPUT);
    pinMode(PIN_WATER_EN_OUT, OUTPUT);



    // motor safe start configuration
    digital_write_motor_pins_reset();
    motor_set_resolution(3);
    // 0 is too fast
    // 1 is a bit slower
    // 2 is even slower
    // 


    #if _RUN_CHOSEN_ == _RUN_MAIN_
        run_autonomous();
    
    #elif _RUN_CHOSEN_ == _RUN_TESTS_
        run_tests();
        _TRAP_AT_END_OF_TESTS_    
    
    #else
        l_e_("[ DDP ] choose _run_main or _run_tests!");
        assert(FALSE);
    #endif
}
