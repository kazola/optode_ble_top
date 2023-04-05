#ifndef _APP_INC_H_
#define _APP_INC_H_




#define l_i_ Log.info
#define l_e_ Log.error
#define _LOG_ROOM_MAIN_     l_i_("%s", "\n\n\n\n\n\n");
#define _LOG_ROOM_FXN_      l_i_("%s", "\n\n\n");



// stuff for BLE within IRIS scanners
#define MAC_OPTODE_MINI_1       "4b:45:2d:e9:38:a0"
#define MAC_OPTODE_MINI_2       "_________________"



// stuff for BLE APP
#define _APP_BLE_SCAN_DURATION_S_           5
#define _APP_BLE_SCAN_TIMEOUT_S(n)          (n * 100)
#define _APP_BLE_SCAN_RESULT_MAX_LEN_       100
#define _APP_BLE_NAME_LEN_		            25
#define _APP_BLE_NOTIFICATION_LEN_	        20
#define _APP_BLE_MAC_LEN_                   17




#define _dW_    digitalWrite
#define _dR_    digitalRead
#define _aR_    analogRead



// -----------
// IN pins
// -----------

#define PIN_MOTOR_LIMIT_LEFT_IN             12  /* closer to motor */
#define PIN_MOTOR_LIMIT_RIGHT_IN            13
#define PIN_ADC_BATTERY_IN                  A4
#define PIN_ADC_WATER_IN                    A3




// -----------
// OUT pins
// -----------

#define PIN_MOTOR_EN_OUT        D4
#define PIN_MOTOR_MS1_OUT       D5
#define PIN_MOTOR_MS2_OUT       D6
#define PIN_MOTOR_SLEEP_OUT     A2
#define PIN_MOTOR_STEP_OUT      D7
#define PIN_MOTOR_DIR_OUT       D8
#define PIN_LED_STRIP_OUT       11
#define PIN_BATTERY_EN_OUT      A5
#define PIN_WATER_EN_OUT        D3





// -----------
// includes
// -----------

#include "Particle.h"
#include "ble.h"
#include <string.h>
#include <stdint.h>
#include "ble_central.h"
#include "ble_peripheral.h"
#include "motor.h"
#include "tests.h"



extern BleCharacteristic c_r;
extern BleCharacteristic c_w;



#endif