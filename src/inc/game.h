#ifndef _GAME_H_
#define _GAME_H_

#include "display.h"

// Single cell
typedef union cell
{
  uint8_t byte;

  struct
  {
    uint8_t b0 : 1;
    uint8_t b1 : 1;
    uint8_t b2 : 1;
    uint8_t b3 : 1;
    uint8_t b4 : 1;
    uint8_t b5 : 1;
    uint8_t b6 : 1;
    uint8_t b7 : 1;
  };

} cell_t;

// Cell status, death or alive
typedef enum cell_status
{
  death = 0,
  alive = 1
} cell_status_t;

void Game_Init();
void Game_Update(u8g2_t *context, uint8_t fragment);
cell_status_t Game_SetCell(cell_t *p, uint16_t x, uint16_t y, cell_status_t status);
cell_status_t Game_GetCell(cell_t *p, uint16_t x, uint16_t y);
uint8_t Game_NumberAround(cell_t *p, uint16_t x, uint16_t y);
cell_t *Game_SwapPetri();
void Game_NextGeneration();
void Game_PutBuffer(uint8_t x, uint8_t y, uint8_t *buffer, uint8_t stride);

#endif /* _GAME_H_ */
