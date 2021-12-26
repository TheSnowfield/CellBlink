#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <u8g2.h>

typedef void (*PresentCallback)(u8g2_t *context, uint8_t fragment);

void Display_Init();
void Display_Present();
void Display_OnPresent(PresentCallback callback);

#endif /* _DISPLAY_H_ */
