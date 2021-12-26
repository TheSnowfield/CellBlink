#include "inc/common.h"
#include "inc/timer.h"
#include "inc/display.h"
#include "inc/game.h"
#include "inc/random.h"

void main()
{
  // Initialize i2c pin
  GPIO_DeInit(SSD1306_PORT);
  GPIO_Init(SSD1306_PORT, SSD1306_SCL, GPIO_MODE_OUT_OD_HIZ_FAST);
  GPIO_Init(SSD1306_PORT, SSD1306_SDA, GPIO_MODE_OUT_OD_HIZ_FAST);

  // Initialize ADC pin
  GPIO_DeInit(ADC_PORT);
  GPIO_Init(ADC_PORT, ADC_PIN, GPIO_MODE_IN_FL_NO_IT);

  // Initialize ADC for random generator
  Random_Init();

  // Initialize internal clock
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSECmd(DISABLE);
  CLK_LSICmd(DISABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);

  // Initialize timer
  Timer_Init();
  __enable_interrupt();

  // Initialize display
  Display_Init();
  Display_OnPresent((PresentCallback)Game_Update);

  // Initialize game
  Game_Init();

  // Update display
  while (TRUE)
    Display_Present();
}
