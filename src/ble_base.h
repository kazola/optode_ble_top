#ifndef _APP_BLE_BASE_H_
#define _APP_BLE_BASE_H_



#include "inc.h"



void ble_clr_data_buf();
int ble_data_endswith(const char *);
int ble_data_almost_endswith(const char *, int);
void ble_scan_for_loggers();
uint8_t ble_interact_optode_mini(const char *);





#endif
