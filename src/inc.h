#ifndef _APP_INC_H_
#define _APP_INC_H_



#define _RUN_AUTO_          0
#define _RUN_TESTS_         1
#define _RUN_CHOSEN_        _RUN_AUTO_




#define l_i_ Log.info
#define l_e_ Log.error
#define _LOG_ROOM_     l_i_("%s", "\n\n\n\n\n\n");



// do not touch: list of mini's MAC addresses for iris scanners
#define MAC_OPTODE_MINI_1       "2f:b3:cf:49:55:4d"
#define MAC_OPTODE_MINI_2       "eb:94:21:4a:63:12"
#define MAC_OPTODE_MINI_3       "fe:7d:29:d8:7d:67"
#define MAC_OPTODE_MINI_4       "72:51:d2:5f:7f:b3"
#define MAC_OPTODE_MINI_5       "bf:f0:b0:33:1b:07"
#define MAC_OPTODE_MINI_6       "4B:45:2D:E9:38:A0"
#define MAC_OPTODE_MINI_7       "3D:DA:B1:96:FB:5F"
#define MAC_OPTODE_MINI_8       "8E:74:50:F6:FA:AF"
#define MAC_OPTODE_MINI_9       "F2:C3:07:C2:FE:7E"
#define MAC_OPTODE_MINI_10      "9B:38:CD:67:72:79"
#define MAC_OPTODE_MINI_11      "B9:8D:97:A1:06:A2"  
#define MAC_OPTODE_MINI_12      "CA:DA:DE:9A:E1:37"  
#define MAC_OPTODE_MINI_E       "B1:56:CC:26:C4:74"   



// you can touch these 2
#define MAC_OPTODE_MINI_A   ble_get_mini_mac_from_core_mac('a')
#define MAC_OPTODE_MINI_B   ble_get_mini_mac_from_core_mac('b')



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
#include "led_strip.h"
#include "motor.h"
#include "tests.h"
#include "autonomous.h"



extern BleCharacteristic c_r;
extern BleCharacteristic c_w;



extern uint8_t g_v_it;
extern uint8_t g_sleep_wifi;



#endif