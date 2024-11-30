// #include <stdio.h>
#include "throw_mech.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"

using namespace PS4;

void Throw_Mech::init()
{
  joystick.init();
  bldc[0].init();
  bldc[1].init();
}

void Throw_Mech::run()
{
  tick = HAL_GetTick();
  if ((tick - prevTick) < MECH_LOOP_PERIOD)
    return;
  update_state();
  update_speed();
  bldc[0].set_speed(bldc_speeds[0]);
  bldc[1].set_speed(bldc_speeds[1]);
  prevTick = HAL_GetTick();
}

void Throw_Mech::update_speed()
{
  switch (state)
  {
  case IDLE:
    bldc_speeds[0] = 0.0f;
    bldc_speeds[1] = 0.0f;
    break;

  case DIFFERENTIAL_MANUAL:
    bldc_speeds[0] = (float)joystick.lt() / 255;
    bldc_speeds[1] = (float)joystick.rt() / 255;
    break;

  case DIFFERENTIAL_MANUAL_HOLD:
    if (joystick.pressed(L3))
    {
      bldc_speeds[0] = (float)joystick.lt() / 255;
      bldc_speeds[1] = (float)joystick.rt() / 255;
    }

  case SAME_MANUAL:
    bldc_speeds[0] = (float)joystick.lt() / 255;
    bldc_speeds[1] = (float)joystick.lt() / 255;

  case SAME_AUTO:
    if (joystick.pressed(L3))
    {
      bldc_speeds[0] = (float)joystick.lt() / 255;
      bldc_speeds[1] = (float)joystick.lt() / 255;
    }

  default:
    break;
  }
}

void Throw_Mech::update_state()
{
  bldc[0].program();
  bldc[1].program();
  if (!joystick.update())
    return;
  if (!joystick.connected() || joystick.clicked(PS))
  {
    set_state(IDLE);
  }
  else if (joystick.pressed(L1))
  {
    if (joystick.clicked(SQUARE))
      set_state(DIFFERENTIAL_MANUAL);
    if (joystick.clicked(TRIANGLE))
      set_state(DIFFERENTIAL_MANUAL_HOLD);
    if (joystick.clicked(CIRCLE))
      set_state(SAME_AUTO);
    if (joystick.clicked(CROSS))
      set_state(SAME_MANUAL);
  }
}

void Throw_Mech::set_state(Mech_States _state)
{
  state = _state;
}
