#include <nds.h>

touchPosition touch; // used to move square
void createSquare(int xLoc, int yLoc, OamState* screen, u16* gfx, u16 color);


int main(void) {
	u16* mainGFX = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp); // top screen square
	u16* subGFX = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_Bmp); // bottom screen square

    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_Bmp_1D_128, false);

    videoSetModeSub(MODE_0_2D);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);


    int x = 0;
    int y = 0;
    int dx = 1; // x-direction: 1 = moving right, -1 = moving left
    int dy = 1; // y-direction: 1 = moving down, -1 = moving up

while (1) {
    scanKeys();
    int key = keysHeld();
    if (key & KEY_TOUCH) touchRead(&touch);
    if (key & KEY_A) {
        x = touch.px; // Move square to touch x position
        y = touch.py; // Move square to touch y position
    }
    if(key & KEY_B) {
        y = touch.px; // Move square to touch x position
        x = touch.py; // Move square to touch y position
    }
    if(key & KEY_UP) y -= 2;    // Move up
    if(key & KEY_DOWN) y += 2;  // Move down
    if(key & KEY_LEFT) x -= 2;  // Move left
    if(key & KEY_RIGHT) x += 2; // Move right
    if (key & KEY_X) dx*=-1; // Reverse horizontal direction
    if (key & KEY_Y) dy*=-1; // Reverse vertical direction

    // Draw the square
    createSquare(x, y, &oamMain, mainGFX, ARGB16(1, 31, 12, 12));
    createSquare(touch.px, touch.py, &oamSub, subGFX, ARGB16(1, 12, 12, 31));

    swiWaitForVBlank();
    oamUpdate(&oamSub);
    oamUpdate(&oamMain);

    // Update position
    x += dx;
    y += dy;

    // Bounce on edges
    if (x <= 0 || x >= 256 - 16) dx = -dx; // 256 = screen width, 16 = sprite size
    if (y <= 0 || y >= 192 - 16) dy = -dy; // 192 = screen height, 16 = sprite size
    if (x < 0 || y < 0 || x > 256 || y > 192) {
        x = 0;
        y = 0;
        dx = 1;
        dy = 1;
    }

    }
}

void createSquare(int xLoc, int yLoc, OamState* screen, u16* gfx, u16 color) {
    dmaFillHalfWords(color, gfx, 512);
	oamSet(screen, // which display
		1, // the oam entry to set
		xLoc, yLoc, // x & y location
		0, // priority
		15, // palette for 16 color sprite or alpha for bmp sprite
		SpriteSize_16x16, // size
		SpriteColorFormat_Bmp, // color type
		gfx, // the oam gfx
		0, //affine index
		false, //double the size of rotated sprites
		false, //don't hide the sprite
		false, false, //vflip, hflip
		false //apply mosaic
		);
}
