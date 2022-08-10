#ifndef _APP_INC_H_
#define _APP_INC_H_




#define l_i_ Log.info
#define l_e_ Log.error
#define _LOG_ROOM_MAIN_     l_i_("%s", "\n\n\n\n\n\n");
#define _LOG_ROOM_FXN_      l_i_("%s", "\n\n\n");




#define _APP_BLE_DATA_BUFFER_LEN_           2048
#define _APP_BLE_SCAN_DURATION_S_           5
#define _APP_BLE_SCAN_TIMEOUT_S(n)          (n * 100)
#define _APP_BLE_SCAN_RESULT_MAX_LEN_       100
#define _APP_BLE_LOGGER_NAME_LEN_		    25
#define _APP_BLE_LOGGER_MAC_LEN_		    18
#define _APP_BLE_NOTIFICATION_LEN_	        20



#define _dW_  digitalWrite
#define _dR_  digitalRead



// -----------
// IN pins
// -----------

#define PIN_MOTOR_LIMIT_LEFT_IN             12  /* closer to motor */
#define PIN_MOTOR_LIMIT_RIGHT_IN            13
// #define PIN_ADC_MOTOR_IN                    A11
// #define PIN_ADC_BATTERY_IN                  A10



// -----------
// OUT pins
// -----------

#define PIN_MOTOR_EN_OUT        D4
#define PIN_MOTOR_MS1_OUT       D5
#define PIN_MOTOR_MS2_OUT       D6
#define PIN_MOTOR_STEP_OUT      D7
#define PIN_MOTOR_DIR_OUT       D8
// #define PIN_LED_STRIP_OUT                   11




// -----------
// includes
// -----------

#include "Particle.h"
#include "ble.h"
#include <string.h>
#include <stdint.h>
#include "ble_base.h"
#include "motor.h"





extern BleCharacteristic c_r;
extern BleCharacteristic c_w;
extern uint8_t ble_data[_APP_BLE_DATA_BUFFER_LEN_];
extern uint16_t ble_data_i;







#endif