#include "inc/common.h"
#include "inc/timer.h"
#include "inc/display.h"

static u8g2_t context;
static PresentCallback present_proc = (void*)0;

void Display_OnPresent(PresentCallback callback)
{
    present_proc = callback;
}

void Display_Present()
{
    uint8_t index = 0;
    u8g2_FirstPage(&context);
    u8g2_ClearBuffer(&context);
    do {

      // Call present proc
      if(present_proc)
        present_proc(&context, index++);

    } while(u8g2_NextPage(&context));
}

uint8_t Display_TransferCallback(u8x8_t *u8x8, uint8_t msg,
                              uint8_t arg_int, void *arg_ptr)
{
  switch (msg)
  {

  case U8X8_MSG_BYTE_SET_DC:
    /* ignored for i2c */
    break;

  case U8X8_MSG_BYTE_INIT:

    // Initialize hardware i2c
    I2C_DeInit();
    I2C_Init(400000, 0x00, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16);
    I2C_Cmd(ENABLE);
    break;

  // Start transfer
  case U8X8_MSG_BYTE_START_TRANSFER:
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    // Send slave address
    I2C_Send7bitAddress(0x3C << 1, I2C_DIRECTION_TX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    break;

  // Send data
  case U8X8_MSG_BYTE_SEND:
    // uint8_t *data = (uint8_t *)arg_ptr;
    for (int i = 0; i < arg_int; ++i)
    {
      I2C_SendData(((uint8_t *)arg_ptr)[i]);
      while (I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET);
    }

    break;

  // End transfer
  case U8X8_MSG_BYTE_END_TRANSFER:
    I2C_GenerateSTOP(ENABLE);
    (void)I2C->SR1;
    (void)I2C->SR3;
    break;

  default:
    return 0;
  }

  return 1;
}

uint8_t Display_DelayCallback(u8x8_t *u8x8, uint8_t msg,
                           uint8_t arg_int, void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_DELAY_MILLI:
    Timer_DelayMilliseconds(arg_int);
    break;
  case U8X8_MSG_DELAY_I2C:
    break;
  default:
    return 0;
  }

  return 1;
}

void Display_Init()
{
  // Initialize display
  u8g2_Setup_ssd1306_i2c_128x64_noname_2(&context, U8G2_R0,
                                         Display_TransferCallback, Display_DelayCallback);
  u8g2_InitDisplay(&context);
  u8g2_SetPowerSave(&context, 0);
  u8g2_SetDrawColor(&context, 0xFF);
}
