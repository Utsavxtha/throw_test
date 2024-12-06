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
  // print_speeds();
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

  case DIFFERENTIAL:
    bldc_speeds[0] = joystick.lt();
    bldc_speeds[1] = joystick.rt();
    break;

  case DIFFERENTIAL_HOLD:
    if (joystick.pressed(L1))
    {
      bldc_speeds[0] = joystick.lt();
    }
    if (joystick.pressed(R1))
    {
      bldc_speeds[1] = joystick.rt();
    }

  case SAME:
    bldc_speeds[0] = joystick.lt();
    bldc_speeds[1] = joystick.lt();

  case SAME_HOLD:
    if (joystick.pressed(L1))
    {
      bldc_speeds[0] = joystick.lt();
      bldc_speeds[1] = joystick.lt();
    }

  default:
    break;
  }
}

void Throw_Mech::update_state()
{
  bldc[0].program();
  bldc[1].program();
  joystick.update();
  if (!joystick.connected() || joystick.clicked(PS) || !bldc[0].is_ready() || !bldc[1].is_ready())
  {
    set_state(IDLE);
  }
  else if (joystick.pressed(L1))
  {
    if (joystick.clicked(SQUARE))
      set_state(DIFFERENTIAL);
    if (joystick.clicked(TRIANGLE))
      set_state(DIFFERENTIAL_HOLD);
    if (joystick.clicked(CIRCLE))
      set_state(SAME);
    if (joystick.clicked(CROSS))
      set_state(SAME_HOLD);
  }
}

void Throw_Mech::set_state(Mech_States _state)
{
  bldc_speeds[0] = 0.0f;
  bldc_speeds[1] = 0.0f;
  state = _state;
}

void Throw_Mech::print_speeds()
{
  // printf("State:%d BLDC speeds 1: %f 2: %f\n", (int)state, bldc_speeds[0], bldc_speeds[1]);
}
