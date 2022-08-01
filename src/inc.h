#ifndef _APP_INC_H_
#define _APP_INC_H_




#define l_i_ Log.info
#define l_e_ Log.error
#define _LOG_ROOM_MAIN_     l_i_("%s", "\n\n\n\n\n\n");
#define _LOG_ROOM_FXN_      l_i_("%s", "\n\n\n");




#define _APP_BLE_DATA_BUFFER_LEN_           2048
#define _APP_BLE_SCAN_DURATION_S_           10
#define _APP_BLE_SCAN_TIMEOUT_S(n)          (n * 100)
#define _APP_BLE_SCAN_RESULT_MAX_LEN_       100
#define _APP_BLE_LOGGER_NAME_LEN_		    25
#define _APP_BLE_LOGGER_MAC_LEN_		    18
#define _APP_BLE_NOTIFICATION_LEN_	        20






#include "Particle.h"
#include "ble.h"
#include <string.h>
#include <stdint.h>
#include "ble_base.h"





extern BleCharacteristic c_r;
extern BleCharacteristic c_w;
extern uint8_t ble_data[_APP_BLE_DATA_BUFFER_LEN_];
extern uint16_t ble_data_i;







#endif