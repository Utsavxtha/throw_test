#include <memory.h>
#include "usart.h"
#include "gpio.h"
#include "throw_mech.h"

Throw_Mech throw_mech;

void throw_mech_run()
{
  throw_mech.init();

  while (1)
  {
    throw_mech.run();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  __HAL_UART_FLUSH_DRREGISTER(huart);

  static uint32_t prevTick = 0;
  uint32_t curTick = HAL_GetTick();

  if ((curTick - prevTick) >= 50)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    prevTick = curTick;
  }

  if (huart->Instance == throw_mech.joystick.uart.get_uart_instance())
  {
    throw_mech.joystick.uart_callback();
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  __HAL_UART_FLUSH_DRREGISTER(huart);

  if (huart->Instance == throw_mech.joystick.uart.get_uart_instance())
  {
    throw_mech.set_state(IDLE);
    throw_mech.joystick.init();
  }
}
