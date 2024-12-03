#include "LUI.h"

class DrawMask
{
public:
    char getAreaMaskValue(int relX, int relY);

private:
    char mask[LUI_SCREEN_WIDTH*LUI_SCREEN_WIDTH];
};