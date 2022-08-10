#include <Arduino.h>
#include <assert.h>
#include "motor.h"
#include "inc.h"



#define _en_        _dW_(PIN_MOTOR_EN_OUT, 0)
#define _dis_       _dW_(PIN_MOTOR_EN_OUT, 1)
#define _fw_        _dW_(PIN_MOTOR_DIR_OUT, 0)
#define _bw_        _dW_(PIN_MOTOR_DIR_OUT, 1)
#define _rvm_(x)    { _dis_; return x;}



void _step()
{
    _dW_(PIN_MOTOR_STEP_OUT, 1);
    delay(1);
    _dW_(PIN_MOTOR_STEP_OUT, 0);
    delay(1);
}



void digital_write_motor_pins_reset()
{
    // EN high means motor DISabled
    _dW_(PIN_MOTOR_EN_OUT, 1);
    _dW_(PIN_MOTOR_MS1_OUT, 0);
    _dW_(PIN_MOTOR_MS2_OUT, 0);
    _dW_(PIN_MOTOR_STEP_OUT, 0);
    _dW_(PIN_MOTOR_DIR_OUT, 0);
}



uint8_t motor_move_right(uint16_t t_ms)
{
    _bw_;
    _en_;

    uint32_t _et = millis() + t_ms;

    while (1)
    {
        _step();
        if (millis() > _et) _rvm_(1);
        //if (is_motor_right_limit_hit()) _rvm_(2);
    }
}



uint8_t motor_move_left(uint16_t t_ms)
{
    _fw_;
    _en_; 

    uint32_t _et = millis() + t_ms;

    while (1)
    {
        _step();
        if (millis() > _et) _rvm_(1);
        //if (is_motor_left_limit_hit()) _rvm_(2);
    }
}



void motor_set_resolution(uint8_t r)
{
    // 300 steps take 1 second
    // resolution 1 -> center to side in 1.5 seconds
    // resolution 3 -> finer, slower

    assert(r>=0);
    assert(r<=3);

    if (r == 0) 
    {
        // full step, more distance, less precise
        _dW_(PIN_MOTOR_MS1_OUT, 0);
        _dW_(PIN_MOTOR_MS2_OUT, 0);
    }
    else if (r == 1)
    {
        // half step
        _dW_(PIN_MOTOR_MS1_OUT, 1);
        _dW_(PIN_MOTOR_MS2_OUT, 0);
    }
    else if (r == 2)
    {
        // quarter step
        _dW_(PIN_MOTOR_MS1_OUT, 0);
        _dW_(PIN_MOTOR_MS2_OUT, 1);
    }
    else
    {
        // eight step, less distance, more precise
        _dW_(PIN_MOTOR_MS1_OUT, 1);
        _dW_(PIN_MOTOR_MS2_OUT, 1);
    }
}



int is_motor_left_limit_hit()
{
    return !_dR_(PIN_MOTOR_LIMIT_LEFT_IN);
}



int is_motor_right_limit_hit()
{
    return !_dR_(PIN_MOTOR_LIMIT_RIGHT_IN);
}