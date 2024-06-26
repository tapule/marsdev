#include "mars.h"
#include "string.h"

#define PIXEL_WRITE_BUFFER_SIZE_B 8
#define FRAMEBUFFER_ROW_EXTENSION 16
#define IS_MIRRORED 1

#define IS_TRANSPARENT 1
static const int TRANSPARENT_PIXEL_COLOR = 0;
static const vu8 pixelWords[PIXEL_WRITE_BUFFER_SIZE_B] = {0,0,0,0,0,0,0,0};
/*
* Loads all colors from region in ROM defined by paletteStart to paletteEnd
* into the CRAM
* @param paletteStart - pointer to starting position of palette data
* @param paletteEnd - pointer to end position of palette data
* @param paletteOffset - offset in the CRAM to start writing the palette data to. Normally is 0.
*/
void loadPalette(const vu16 *paletteStart, const vu16 *paletteEnd, const int paletteOffset)
{
	int numColors;
	int i;
	vu16 *cram16 = &MARS_CRAM;
	vu16 *pal16 = (vu16*) paletteStart;
	
	numColors = ((paletteEnd)-(paletteStart));
	for (i = paletteOffset; i < numColors+paletteOffset; i++)
	{
		cram16[i] = pal16[i-paletteOffset] & 0x7FFF;
	}
}

/* 
* Draws blank pixels to rectangle specified by x, y, xWidth and yWidth (height)
*/
void clearArea(vu16 x, vu16 y, int xWidth, int yWidth){
	vu16 *frameBuffer16 = &MARS_FRAMEBUFFER;
	vu16 xOff;
	//int i;
	int bufCnt;
	int size = (xWidth * yWidth) / 2;
	const uint16 lineTableEnd = 0x100;
	uint16 fbOff = lineTableEnd;
	int drawWidth = 0;
	
	//offset the number of pixels in each line to start to draw the image
	xOff = x / 2;
	
	fbOff = lineTableEnd;
	//y-offset for top of sprite to correct line in framebuffer
	fbOff = fbOff + (y * 160);
	//x-offset from start of first line
	fbOff = fbOff + xOff;
	//draw spriteBuffer to the framebuffer
	drawWidth = 0;
	for (bufCnt = 0; bufCnt < size; bufCnt++)
	{
		//write word to framebuffer
		frameBuffer16[fbOff] = TRANSPARENT_PIXEL_COLOR;
		fbOff++;
		drawWidth++;
		if(drawWidth >= (xWidth/2)){
			//reset line
			drawWidth = 0;
			fbOff = fbOff + (160 - ((xWidth/2) + xOff)) + xOff;
		}
	}
}




/*
* Draws an image to position on MARS framebuffer allowing you to flip the image using mirror param.
* 
* @param spriteBuffer - pointer to starting position of image data
* @param x - x pixel coordinate of top-left corner of the image 
* @param y - y pixel coordinate of top-left corner of the image
* @param xWidth - vertical size of image to be drawn in pixels
* @param yWidth - horizontal size of image to be drawn in pixels
* @param mirror - 0 for normal 1 for flipped along y-axis
* @param checkTransparency - 0 for not checked ie overwrite every pixel, including with zero, 1 for checking
* @param screenWrap - 0 for no screenWrap, 1 for screen wrapping  
*/
int drawSpriteMaster(const vu8 *spriteBuffer, const int16 x, const int16 y, const int xWidth, const int yWidth, const int mirror, const int checkTransparency)
{

	//MARS_VDP_MODE_256
	//each byte represents the color in CRAM for each pixel.
	vu8 *frameBuffer8 = NULL;//(vu8*) &MARS_FRAMEBUFFER;
	int xOff;
	int bufCnt=0;
	int rowPos=0;
	int xCount=0;
	int xOverflow=0;
	int lineEnd = 0;
	int spriteModEight = 0;
	int spriteStart = 0;
	int absX = 0;
	//Line table is 256 words ie 256 * 2 bytes
	const uint16 lineTableEnd = 0x200;
	int fbOff;
	//int p=0;
	//TODO this is always 1 (for 8 byte segments using the word_8byte copy functions
	const int pixelWriteBufferSizeWords =  1;//PIXEL_WRITE_BUFFER_SIZE_B/2;
	
	//overwrite buffer - ie zero is not written - what you need if you want transparency on sprites
	//TODO might have some problems if over screen edges?
	if(checkTransparency == IS_TRANSPARENT){
		frameBuffer8 = (vu8* ) &MARS_OVERWRITE_IMG;
	}else{
		frameBuffer8 = (vu8* ) &MARS_FRAMEBUFFER;
	}

	//offset the number of pixels in each line to start to draw the image
	xOff = (int)x;
	//if off the left edge of the screen, special care is needed
	if(x < -PIXEL_WRITE_BUFFER_SIZE_B){
		//need to draw in multiples of 8 so find, the portion of the sprite image to draw
		//offset the start of the line to compensate
		//this code fixed a glitch where you would see artifacts of the left edge of image off to the right
		absX = abs(x);
		spriteModEight = x%PIXEL_WRITE_BUFFER_SIZE_B;
		spriteStart = absX + spriteModEight;
		xOff = spriteModEight;
	}
	
	
	//move the framebuffer offset to start of the visible framebuffer?? 
	//Line table is 256 words ie 256 * 2 bytes
	fbOff = lineTableEnd;// - ( PIXEL_WRITE_BUFFER_SIZE_B - 1 );
	//y-offset for top of sprite to correct line in framebuffer
	fbOff = fbOff + (((int)y * (SCREEN_WIDTH + 16)) + 8);
	//x-offset from start of first line
	fbOff = fbOff + xOff;
	bufCnt = 0;
	xCount = 0;
	rowPos = 0;
	
	//if the image is totally off the left side of the screen skip it
	if( (xWidth + x) < 0 || x > SCREEN_WIDTH){
		return -1;
	}
	
	//loop for all the rows
	for (rowPos = 0; rowPos < yWidth; rowPos++)
	{
		//p = 0;
		
		if(mirror == IS_MIRRORED){
			//increment a row
			bufCnt = bufCnt + xWidth;
		}
		
		xCount = 0;
		lineEnd = 0;
		//if off the left edge of the screen + our buffer, we should skip to the correct part of the spriteBuffer for each row
		if(x < -PIXEL_WRITE_BUFFER_SIZE_B){
			xCount = spriteStart;
			lineEnd = -PIXEL_WRITE_BUFFER_SIZE_B;
		}
		//for the row iterate over the columns
		for( ; xCount < xWidth; xCount+=PIXEL_WRITE_BUFFER_SIZE_B)
		{
			lineEnd += PIXEL_WRITE_BUFFER_SIZE_B;
			xOverflow = 0;
			//if mirror is 1 that tells us to flip the column
			if(mirror == IS_MIRRORED){
				//copy the next 8 bytes in reverse
				word_8byte_copy_bytereverse((void *)(frameBuffer8+fbOff), (void *)(&spriteBuffer[bufCnt-(xCount+PIXEL_WRITE_BUFFER_SIZE_B)]), pixelWriteBufferSizeWords);
				
			}else{
				//copy the next 8 bytes
				word_8byte_copy((void *)(frameBuffer8+fbOff), (void *)(&spriteBuffer[bufCnt+xCount]), pixelWriteBufferSizeWords);
				
			}
			
			
			//don't draw if you've gone over the screenwidth to the right side
			if(xOff + xCount + PIXEL_WRITE_BUFFER_SIZE_B > (SCREEN_WIDTH)){
				xOverflow = SCREEN_WIDTH - (xOff+xCount);
				//advance up to the end of this row
				//fbOff += PIXEL_WRITE_BUFFER_SIZE_B - (PIXEL_WRITE_BUFFER_SIZE_B - xOverflow);
				xCount = xWidth;
				
			} // if drawing something where x is off the left side 
			else if(x < -PIXEL_WRITE_BUFFER_SIZE_B) { 
				//increment to next position in FrameBuffer
				fbOff += PIXEL_WRITE_BUFFER_SIZE_B;//- xOverflow;
				xOverflow = SCREEN_WIDTH - (xOff+lineEnd) - PIXEL_WRITE_BUFFER_SIZE_B;
			} // default case
			else{
				//increment to next position in FrameBuffer
				fbOff += PIXEL_WRITE_BUFFER_SIZE_B;//- xOverflow;
				xOverflow = SCREEN_WIDTH - (xOff+xCount) - PIXEL_WRITE_BUFFER_SIZE_B;
			}

		}//end for xCount
		
		//increment a row if not "reversed" 
		if(mirror != IS_MIRRORED){
			bufCnt = bufCnt + xWidth;
		}
		//=((E10+G10+(16-G10)))-(C10+B10)+C10
		//reset the "line" in framebuffer if past the width of the image
		//fbOff = fbOff + ((SCREEN_WIDTH + xOverflow+(16-xOverflow)) - (xWidth + xOff)) + xOff;
		fbOff += (xOverflow + FRAMEBUFFER_ROW_EXTENSION) +xOff;
		
	}//end for rowPos
	//write any "leftover pixels? shouldn't happen
	
	return 0;
	
}

/*
* Draws an image to position on MARS framebuffer allowing you to flip the image using mirror param.
* 
* @param spriteBuffer - pointer to starting position of image data to read
* @param x - x pixel coordinate of top-left corner of the image 
* @param y - y pixel coordinate of top-left corner of the image
* @param xWidth - vertical size of image to be drawn in pixels
* @param yWidth - horizontal size of image to be drawn in pixels
* @param mirror - 0 for normal 1 for flipped along y-axis
* @param screenWrap - 0 for no screenWrap, 1 for screen wrapping    
*/
int drawSprite(const vu8 *spriteBuffer, const int16 x, const int16 y, const int xWidth, const int yWidth, const int mirror)
{
	//call drawSprite with transparency enabled
	return drawSpriteMaster(spriteBuffer,  x, y, xWidth, yWidth, mirror, 1);
}



/*
* Draws a background image on MARS framebuffer allowing you to flip the image using mirror param, no transparency
* 
* @param spriteBuffer - pointer to starting position of image data
*
*/
int drawBG(const vu8 *spriteBuffer)
{
	//draw full screen background image with no transparency
	return drawSpriteMaster(spriteBuffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
}

/* 
* Draws pixels to fill rectangle specified by x, y, xWidth and yWidth (height). Must be on screen.
* xWidth must be sized in multiples of 8 wide (8 pixels, 16 pixels, etc..)
*
*/
void drawFillRect(const int16 x, const int16 y, const int xWidth, const int yWidth, vu8* color){
	//MARS_VDP_MODE_256
	//each byte represents the color in CRAM for each pixel.
	vu8 *frameBuffer8 = (vu8* )&MARS_FRAMEBUFFER;
	int xOff;
	int rowPos=0;
	int xCount=0;
	const uint16 lineTableEnd = 0x200;
	const int pixelWriteBufferSizeWords =  1;
	int fbOff = 0;
	//uint8 color[8] = {8,8,8,8,8,8,8,8};
	
	//offset the number of pixels in each line to start to draw the image
	xOff = (int)x;
	//move the framebuffer offset to start of the visible framebuffer?? 
	//Line table is 256 words ie 256 * 2 bytes
	fbOff = lineTableEnd;// - ( PIXEL_WRITE_BUFFER_SIZE_B - 1 );
	//y-offset for top of sprite to correct line in framebuffer
	fbOff = fbOff + (((int)y * (SCREEN_WIDTH + 16)) + 8);
	//x-offset from start of first line
	fbOff = fbOff + xOff;
	

	//draw rectangle to the framebuffer
	//loop for all the rows (y-axis)
	for (rowPos = 0; rowPos < yWidth; rowPos++)
	{
		//for the row iterate over the columns (x-axis)
		for(xCount=0 ; xCount < xWidth; xCount+=PIXEL_WRITE_BUFFER_SIZE_B)
		{
			//copy the color to framebuffer
			word_8byte_copy((void *)(frameBuffer8+fbOff), (void *)(color), pixelWriteBufferSizeWords);
			fbOff += PIXEL_WRITE_BUFFER_SIZE_B;
		}
		//reset framebuffer offset to next line
		fbOff += (SCREEN_WIDTH - (xOff + xWidth) + FRAMEBUFFER_ROW_EXTENSION) + xOff;
	}
}

/* 
* Draws pixels to outline a rectangle specified by x, y, xWidth and yWidth (height). Must be on screen.
* xWidth must be sized in multiples of 8 wide (8 pixels, 16 pixels, etc..)
*
*/
void drawRect(const int16 x, const int16 y, const int xWidth, const int yWidth, vu8* color){
	//MARS_VDP_MODE_256
	//each byte represents the color in CRAM for each pixel.
	vu8 *frameBuffer8 = (vu8* )&MARS_FRAMEBUFFER;
	int xOff;
	int rowPos=0;
	int xCount=0;
	const uint16 lineTableEnd = 0x200;
	const int pixelWriteBufferSizeWords =  1;
	int fbOff = 0;
	//uint8 color[8] = {8,8,8,8,8,8,8,8};
	
	//offset the number of pixels in each line to start to draw the image
	xOff = (int)x;
	//move the framebuffer offset to start of the visible framebuffer?? 
	//Line table is 256 words ie 256 * 2 bytes
	fbOff = lineTableEnd;// - ( PIXEL_WRITE_BUFFER_SIZE_B - 1 );
	//y-offset for top of sprite to correct line in framebuffer
	fbOff = fbOff + (((int)y * (SCREEN_WIDTH + FRAMEBUFFER_ROW_EXTENSION)) + PIXEL_WRITE_BUFFER_SIZE_B);
	//x-offset from start of first line
	fbOff = fbOff + xOff;
	

	//draw rectangle to the framebuffer
	//loop for all the rows (y-axis)
	for (rowPos = 0; rowPos < yWidth; rowPos++)
	{
		//draw horizontal line
		if(rowPos == 0 || rowPos + 1 == yWidth)
		{
			//for the row iterate over the columns (x-axis)
			for(xCount=0 ; xCount < xWidth; xCount+=PIXEL_WRITE_BUFFER_SIZE_B)
			{
				//copy the color to framebuffer
				word_8byte_copy((void *)(frameBuffer8+fbOff), (void *)(color), pixelWriteBufferSizeWords);
				fbOff += PIXEL_WRITE_BUFFER_SIZE_B;
			}
		}
		else //draw pixel at start of line and at end
		{
			frameBuffer8[fbOff] = color[0];
			//skip to end of line
			fbOff += xWidth;
			frameBuffer8[fbOff-1] = color[0];
		}
		//reset framebuffer offset to next line
		fbOff += (SCREEN_WIDTH - (xOff + xWidth) + FRAMEBUFFER_ROW_EXTENSION) + xOff;
	}
}

/*
* Draw line table to framebuffer
* @param xOff - the x offset in words for each line in the line table
*/
void drawLineTable(const int xOff){
	volatile uint16_t *frameBuffer16 = &MARS_FRAMEBUFFER;
	const uint16 lineTableEnd = 0x100;
	uint16 lineOffs = lineTableEnd + xOff;
	// Set up the line table
	for(int i = 0; i < 256; i++) {
		frameBuffer16[i] = lineOffs;
		lineOffs += 168;
	}
}
