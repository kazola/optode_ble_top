#ifndef _APP_BLE_CENTRAL_H_
#define _APP_BLE_CENTRAL_H_



#include "inc.h"



void ble_central_scan_for_optode_minis(uint8_t *);
uint8_t ble_central_test_optode_mini(const char *);
void ble_central_optode_core();


const char * ble_get_mini_mac_from_core_mac(char);





#endif
