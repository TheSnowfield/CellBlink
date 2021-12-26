#include "inc/common.h"
#include "inc/game.h"
#include "inc/random.h"
#include <string.h>

#define PETRI_WIDTH 32
#define PETRI_HEIGHT 32
#define PETRI_BYTE_WIDTH (PETRI_WIDTH / 8)
#define PETRI_BYTE_HEIGHT (PETRI_HEIGHT)

static cell_t *petri;
static cell_t pertri_a[PETRI_BYTE_HEIGHT * PETRI_BYTE_WIDTH];
static cell_t pertri_b[PETRI_BYTE_HEIGHT * PETRI_BYTE_WIDTH];
const int8_t table_x[] = {-1, 0, 1, 1, 1, 0, -1, -1};
const int8_t table_y[] = {-1, -1, -1, 0, 1, 1, 1, 0};

#define Game_GetCell(p, x, y) ((cell_status_t)((p[(y * PETRI_BYTE_WIDTH) + (x / 8)].byte) >> (x % 8)) & 1)
#define Game_SetCell(p, x, y, s)                                \
  if (s == alive)                                               \
    (p[(y * PETRI_BYTE_WIDTH) + (x / 8)].byte) |= 1 << (x % 8); \
  else                                                          \
    (p[(y * PETRI_BYTE_WIDTH) + (x / 8)].byte) &= ~(1 << (x % 8));

#define Game_ToggleCell(p, x, y) ((p[(y * PETRI_BYTE_WIDTH) + (x / 8)].byte) ^= 1 << (x % 8))

uint8_t Hu[] = {0x00, 0x01, 0x08, 0xF1, 0x30, 0x91, 0x00, 0x91,
                0xC0, 0xF3, 0x04, 0x91, 0x08, 0x91, 0xC0, 0x92,
                0x50, 0xF2, 0x50, 0x92, 0xC8, 0x93, 0x48, 0x8A,
                0x04, 0x88, 0x00, 0xC6, 0x00, 0x00, 0x00, 0x00}; /*"湖",0*/

void Game_Init()
{
  // Cleanup petries
  memset(pertri_a, 0x00, sizeof(pertri_a));
  memset(pertri_b, 0x00, sizeof(pertri_b));
  petri = pertri_a;

  // Cells born
  Game_PutBuffer(2, 4, Hu, 2);
}

void Game_Update(u8g2_t *context, uint8_t fragment)
{
  // Iterator the cells in first fragment
  if (fragment == 0)
  {
    Game_NextGeneration();
  }

  // Update display
  for (int y = 0; y < PETRI_HEIGHT; ++y)
    for (int x = 0; x < PETRI_BYTE_WIDTH; ++x)
      for (int b = 0; b < 8; ++b)
      {
        // uint8_t byte = petri[y * 8 + x].byte;
        if ((petri[y * 4 + x].byte >> b) & 1)
        {
          // Scale to 2x
          u8g2_DrawBox(context, (x * 8 + b) * 2, y * 2, 2, 2);
        }
      }
}

uint8_t Game_NumberAround(cell_t *p, uint16_t x, uint16_t y)
{
  uint8_t count = 0;

  for (int i = 0; i < sizeof(table_x); ++i)
  {
    int16_t xpos = x + table_x[i];
    int16_t ypos = y + table_y[i];

    if (xpos >= 0 && xpos < PETRI_WIDTH &&
        ypos >= 0 && ypos < PETRI_HEIGHT &&
        Game_GetCell(p, xpos, ypos) == alive)
    {
      ++count;
    }
  }

  return count;
}

void Game_PutBuffer(uint8_t x, uint8_t y, uint8_t *buffer, uint8_t stride)
{
  for (int i = y; i < 16; ++i, buffer += stride)
    memcpy(petri + (i * PETRI_BYTE_WIDTH) + (x * 8), buffer, stride);
}

cell_t *Game_SwapPetri()
{
  // Swap the pointer
  cell_t *old = petri;
  petri = (petri == pertri_a) ? pertri_b : pertri_a;

  // Refresh cell status
  memcpy(petri, old, sizeof(pertri_a));

  return old;
}

void Game_NextGeneration()
{
  // Swap the petri
  cell_t *old_petri = Game_SwapPetri();

  // Iteratoration
  for (int y = 0; y < PETRI_HEIGHT; ++y)
    for (int x = 0; x < PETRI_WIDTH; ++x)
    {
      switch (Game_NumberAround(old_petri, x, y))
      {
      case 2:
        continue;

      case 3:
        Game_SetCell(petri, x, y, alive);
        break;

      default:
        Game_SetCell(petri, x, y, death);
        break;
      }
    }
}
