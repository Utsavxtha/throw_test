#ifndef THROW_MECH__
#define THROW_MECH__

#ifdef __cplusplus

#include <cstdint>

#include "gpio.h"
#include "usart.h"
#include "stdio.h"
#include "tim.h"

#include "robotlib/communication/uart.hpp"
#include "robotlib/actuators/bldc.hpp"
#include "robotlib/joystick/joystick.hpp"

#define MECH_LOOP_PERIOD 10

enum Mech_States
{
  IDLE,
  DIFFERENTIAL_MANUAL,
  DIFFERENTIAL_MANUAL_HOLD,
  SAME_MANUAL,
  SAME_AUTO,
};

class Throw_Mech
{
private:
  float bldc_speeds[2] = {0.0f, 0.0f};
  Mech_States state;
  uint32_t prevTick;
  uint32_t tick;

public:
  Joystick joystick{&JOY_UART};
  BLDC bldc[2] = {BLDC(&BLDC_TIMER, TIM_CHANNEL_1), BLDC(&BLDC_TIMER, TIM_CHANNEL_2)};

  void set_speed();
  void set_state(Mech_States);
  void update_speed();
  void update_state();
  void init();
  void run();
};

extern Throw_Mech throw_mech;

#endif

#ifdef __cplusplus
extern "C"
{
#endif

  void throw_mech_run();

#ifdef __cplusplus
}
#endif

#endif
