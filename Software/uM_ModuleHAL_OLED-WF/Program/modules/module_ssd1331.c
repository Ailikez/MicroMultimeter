/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f3_system.h"
#include "drivers\stm32f3_spi.h"
#include "algorithms\algorithm_string.h"

#include "module_ssd1331.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define OLED_SPIx                   SPI3
#define OLED_CLK_ENABLE()           __HAL_RCC_SPI3_CLK_ENABLE()

#define OLED_DC_PIN                 GPIO_PIN_4
#define OLED_DC_GPIO_PORT           GPIOB
#define OLED_DC_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_DC_H()                 __GPIO_SET(OLED_DC_GPIO_PORT, OLED_DC_PIN)
#define OLED_DC_L()                 __GPIO_RST(OLED_DC_GPIO_PORT, OLED_DC_PIN)

#define OLED_RST_PIN                GPIO_PIN_6
#define OLED_RST_GPIO_PORT          GPIOB
#define OLED_RST_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_RST_H()                __GPIO_SET(OLED_RST_GPIO_PORT, OLED_RST_PIN)
#define OLED_RST_L()                __GPIO_RST(OLED_RST_GPIO_PORT, OLED_RST_PIN)

#define OLED_CST_PIN                GPIO_PIN_15
#define OLED_CST_GPIO_PORT          GPIOA
#define OLED_CST_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define OLED_CST_H()                __GPIO_SET(OLED_CST_GPIO_PORT, OLED_CST_PIN)
#define OLED_CST_L()                __GPIO_RST(OLED_CST_GPIO_PORT, OLED_CST_PIN)

#define OLED_SCK_PIN                GPIO_PIN_3
#define OLED_SCK_GPIO_PORT          GPIOB
#define OLED_SCK_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SCK_AF                 GPIO_AF6_SPI3

#define OLED_SDI_PIN                GPIO_PIN_5
#define OLED_SDI_GPIO_PORT          GPIOB
#define OLED_SDI_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define OLED_SDI_AF                 GPIO_AF6_SPI3
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SSD1331_Config
**功能 : GPIO & SPI Config
**輸入 : None
**輸出 : None
**使用 : SSD1331_Config();
**====================================================================================================*/
/*====================================================================================================*/
void SSD1331_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_HandleTypeDef SPI_HandleStruct;

  /* SPI Clk ******************************************************************/
  OLED_CLK_ENABLE();
  OLED_DC_GPIO_CLK_ENABLE();
  OLED_RST_GPIO_CLK_ENABLE();
  OLED_CST_GPIO_CLK_ENABLE();
  OLED_SCK_GPIO_CLK_ENABLE();
  OLED_SDI_GPIO_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = OLED_CST_PIN;
  HAL_GPIO_Init(OLED_CST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = OLED_DC_PIN;
  HAL_GPIO_Init(OLED_DC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = OLED_RST_PIN;
  HAL_GPIO_Init(OLED_RST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;

  GPIO_InitStruct.Pin       = OLED_SCK_PIN;
  GPIO_InitStruct.Alternate = OLED_SCK_AF;
  HAL_GPIO_Init(OLED_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = OLED_SDI_PIN;
  GPIO_InitStruct.Alternate = OLED_SDI_AF;
  HAL_GPIO_Init(OLED_SDI_GPIO_PORT, &GPIO_InitStruct);

  OLED_CST_H();  // low enable

  /* SPI Init ****************************************************************/
  SPI_HandleStruct.Instance               = OLED_SPIx;
  SPI_HandleStruct.Init.Mode              = SPI_MODE_MASTER;
  SPI_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  SPI_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  SPI_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  SPI_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  SPI_HandleStruct.Init.NSS               = SPI_NSS_SOFT;
  SPI_HandleStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  SPI_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SPI_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  SPI_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  SPI_HandleStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&SPI_HandleStruct);

  __HAL_SPI_ENABLE(&SPI_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_WriteCmd
**功能 : Write Command or Address
**輸入 : writeCmd
**輸出 : None
**使用 : OLED_WriteCmd(0xCB);
**====================================================================================================*/
/*====================================================================================================*/
static void OLED_WriteCmd( uint8_t writeCmd )
{
  OLED_CST_L();
  OLED_DC_L();
  SPI_RW8(OLED_SPIx, writeCmd);
  OLED_CST_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_WriteData
**功能 : Write Data
**輸入 : writeData
**輸出 : None
**使用 : OLED_WriteData(Byte8H(Color));
**====================================================================================================*/
/*====================================================================================================*/
//static void OLED_WriteData( uint8_t writeData )
//{
//  OLED_CST_L();
//  OLED_DC_H();
//  SPI_RW(OLED_SPIx, writeData);
//  OLED_CST_H();
//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_WriteColor
**功能 : Write Color
**輸入 : color
**輸出 : None
**使用 : OLED_WriteColor(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
static void OLED_WriteColor( uint16_t color )
{
  OLED_CST_L();
  OLED_DC_H();
  SPI_RW8(OLED_SPIx, Byte8H(color));
  SPI_RW8(OLED_SPIx, Byte8L(color));
  OLED_CST_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SSD1331_Init
**功能 : SSD1331 Init
**輸入 : None
**輸出 : None
**使用 : SSD1331_Init();
**====================================================================================================*/
/*====================================================================================================*/
void SSD1331_Init( void )
{
  // Hardware Reset
  OLED_RST_L();
  delay_ms(15);
  OLED_RST_H();
  delay_ms(50);

  // Device Init
  OLED_Display(DISABLE); // Display Off

  OLED_WriteCmd(0x81);   // Set Contrast for Color A
  OLED_WriteCmd(0x91);   // 145
  OLED_WriteCmd(0x82);   // Set Contrast for Color B
  OLED_WriteCmd(0x50);   // 80
  OLED_WriteCmd(0x83);   // Set Contrast for Color C
  OLED_WriteCmd(0x7D);   // 125

	OLED_WriteCmd(0x87);   // Master Contrast Current Control
	OLED_WriteCmd(0x06);   // 6

  OLED_WriteCmd(0x8A);   // Set Second Pre-change Speed for Color A
  OLED_WriteCmd(0x64);   // 100
  OLED_WriteCmd(0x8B);   // Set Second Pre-change Speed for Color B
  OLED_WriteCmd(0x78);   // 120
  OLED_WriteCmd(0x8C);   // Set Second Pre-change Speed for Color C
  OLED_WriteCmd(0x64);   // 100

	OLED_WriteCmd(0xA0);   // Set Remap & Color Depth
	OLED_WriteCmd(0x72);   // 0x72

	OLED_WriteCmd(0xA1);   // Set Display Start Line
	OLED_WriteCmd(0x00);   // 0

	OLED_WriteCmd(0xA2);   // Set Display Offset
	OLED_WriteCmd(0x00);   // 0

	OLED_WriteCmd(0xA4);   // Set Display Mode

	OLED_WriteCmd(0xA8);   // Set Multiplex Ratio
	OLED_WriteCmd(0x3F);   // 0x3F

	OLED_WriteCmd(0xAD);   // Set  Master Configuration
	OLED_WriteCmd(0x8E);   // 0x8E

  OLED_WriteCmd(0xB0);   // Set Power Save Mode
	OLED_WriteCmd(0x00);   // 0x00

	OLED_WriteCmd(0xB1);   // Phase 1 and 2 Period Adjustment
	OLED_WriteCmd(0x31);   // 0x31

	OLED_WriteCmd(0xB3);   // Set Display Clock Divider / Oscillator Frequency
	OLED_WriteCmd(0xF0);   // 0xF0

	OLED_WriteCmd(0xBB);   // Set Pre-change Level
	OLED_WriteCmd(0x3A);   // 0x3A

	OLED_WriteCmd(0xBE);   // Set vcomH
	OLED_WriteCmd(0x3E);   // 0x3E

	OLED_WriteCmd(0x2E);   // Disable Scrolling

  OLED_Display(ENABLE);
  OLED_Clear(WHITE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_Display
**功能 : OLED Display
**輸入 : onoff
**輸出 : None
**使用 : LCD_Display(ENABLE);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_Display( uint8_t onoff )
{
  if(onoff == ENABLE)
    OLED_WriteCmd(0xAF);
  else
    OLED_WriteCmd(0xAE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_Clear
**功能 : Clear Window
**輸入 : color
**輸出 : None
**使用 : OLED_Clear(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_Clear( uint16_t color )
{
  uint32_t point = OLED_W * OLED_H;

  OLED_SetWindow(0, 0, OLED_W - 1, OLED_H - 1);

  while(point--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_SetWindow
**功能 : Set Window
**輸入 : posX1, posY1, posX2, posY2
**輸出 : None
**使用 : OLED_SetWindow(X1, Y1, X2, Y2);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_SetWindow( uint8_t posX1, uint8_t posY1, uint8_t posX2, uint8_t posY2 )
{
  OLED_WriteCmd(0x15);
  OLED_WriteCmd(posX1);
  OLED_WriteCmd(posX2);
  OLED_WriteCmd(0x75);
  OLED_WriteCmd(posY1);
  OLED_WriteCmd(posY2);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawPixel
**功能 : Draw a Pixel
**輸入 : posX, posY, color
**輸出 : None
**使用 : OLED_DrawPixel(posX, posY, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawPixel( uint8_t posX, uint8_t posY, uint16_t color )
{
  OLED_SetWindow(posX, posY, posX, posY);
  OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawLine
**功能 : Draw Line
**輸入 : posX1, posY1, posX2, posY2, color
**輸出 : None
**使用 : OLED_DrawLine(posX1, posY1, posX2, posY2, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawLine( uint8_t posX1, uint8_t posY1, uint8_t posX2, uint8_t posY2, uint16_t color )
{
  OLED_WriteCmd(0x21);
  OLED_WriteCmd(posX1);
  OLED_WriteCmd(posY1);
  OLED_WriteCmd(posX2);
  OLED_WriteCmd(posY2);
  OLED_WriteCmd(RGB565_R(color));
  OLED_WriteCmd(RGB565_G(color));
  OLED_WriteCmd(RGB565_B(color));
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawLineX
**功能 : Draw X-Axis Line
**輸入 : posX, posY, length, color
**輸出 : None
**使用 : OLED_DrawLineX(posX, posY, length, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawLineX( uint8_t posX, uint8_t posY, uint8_t length, uint16_t color )
{
  OLED_SetWindow(posX, posY, posX + length - 1, posY);

  while(length--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawLineY
**功能 : Draw Y-Axis Line
**輸入 : posX, posY, length, color
**輸出 : None
**使用 : OLED_DrawLineY(posX, posY, length, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawLineY( uint8_t posX, uint8_t posY, uint8_t length, uint16_t color )
{
  OLED_SetWindow(posX, posY, posX, posY + length - 1);

  while(length--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawRect
**功能 : Draw Rectangle
**輸入 : posX, posY, width, height, color
**輸出 : None
**使用 : OLED_DrawRect(posX, posY, width, height, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawRect( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, uint16_t color )
{
  OLED_DrawLineX(posX,             posY,              width,  color);
  OLED_DrawLineX(posX,             posY + height - 1, width,  color);
  OLED_DrawLineY(posX,             posY,              height, color);
  OLED_DrawLineY(posX + width - 1, posY,              height, color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawRectFill
**功能 : Draw Rectangle
**輸入 : posX, posY, Length, width, height, color
**輸出 : None
**使用 : OLED_DrawRectFill(posX, posY, width, height, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawRectFill( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, uint16_t color )
{
  uint32_t point = width * height;

  OLED_SetWindow(posX, posY, posX + width - 1, posY + height - 1);

  while(point--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawCircle
**功能 : Draw Circle
**輸入 : posX, posY, radius, color
**輸出 : None
**使用 : OLED_DrawCircle(posX, posY, radius, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawCircle( uint8_t posX, uint8_t posY, uint8_t radius, uint16_t color )
{
  int32_t D;
  uint32_t curX;
  uint32_t curY;

  D = 3 - (radius << 1);
  curX = 0;
  curY = radius;

  while(curX <= curY) {
    OLED_DrawPixel(posX + curX, posY - curY, color);
    OLED_DrawPixel(posX - curX, posY - curY, color);
    OLED_DrawPixel(posX + curY, posY - curX, color);
    OLED_DrawPixel(posX - curY, posY - curX, color);
    OLED_DrawPixel(posX + curX, posY + curY, color);
    OLED_DrawPixel(posX - curX, posY + curY, color);
    OLED_DrawPixel(posX + curY, posY + curX, color);
    OLED_DrawPixel(posX - curY, posY + curX, color);

    if(D < 0) {
      D += (curX << 2) + 6;
    }
    else {
      D += ((curX - curY) << 2) + 10;
      curY--;
    }
    curX++;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutChar
**功能 : Put Char
**輸入 : posX, posY, word_h, word_w, pWord, fontColor, backColor
**輸出 : None
**使用 : OLED_PutChar(x, y, 5, 3, ASCII_5x3['C'], WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutChar( uint8_t posX, uint8_t posY, uint8_t word_h, uint8_t word_w, const uint8_t *pWord, uint16_t fontColor, uint16_t backColor )
{
  uint8_t tmp = 0;

  for(uint8_t i = 0; i < word_h; i++) {
    tmp = pWord[i];
    for(uint8_t j = 0; j < word_w; j++) {
      if(((tmp >> (word_w - 1 - j)) & 0x01) == 0x01)
        OLED_DrawPixel(posX + j, posY + i, fontColor);
      else
        OLED_DrawPixel(posX + j, posY + i, backColor);
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutChar16
**功能 : Put Char
**輸入 : posX, posY, word_h, word_w, pWord, fontColor, backColor
**輸出 : None
**使用 : OLED_PutChar16(x, y, 12, 6, ASCII_12x6['C'], WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutChar16( uint8_t posX, uint8_t posY, uint8_t word_h, uint8_t word_w, const uint16_t *pWord, uint16_t fontColor, uint16_t backColor )
{
  uint16_t tmp = 0;

  for(uint8_t i = 0; i < word_h; i++) {
    tmp = pWord[i];
    for(uint8_t j = 0; j < word_w; j++) {
      if(((tmp >> (word_w - 1 - j)) & 0x0001) == 0x0001)
        OLED_DrawPixel(posX + j, posY + i, fontColor);
      else
        OLED_DrawPixel(posX + j, posY + i, backColor);
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutChar32
**功能 : Put Char
**輸入 : posX, posY, word_h, word_w, pWord, fontColor, backColor
**輸出 : None
**使用 : OLED_PutChar32(x, y, 32, 24, ASCII_32x24['C'], WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutChar32( uint8_t posX, uint8_t posY, uint8_t word_h, uint8_t word_w, const uint32_t *pWord, uint16_t fontColor, uint16_t backColor )
{
  uint32_t tmp = 0;

  for(uint8_t i = 0; i < word_h; i++) {
    tmp = pWord[i];
    for(uint8_t j = 0; j < word_w; j++) {
      if(((tmp >> (word_w - 1 - j)) & 0x00000001) == 0x00000001)
        OLED_DrawPixel(posX + j, posY + i, fontColor);
      else
        OLED_DrawPixel(posX + j, posY + i, backColor);
    }
  }
}

//void OLED_PutCharNum_7x6( uint8_t CoordiX, uint8_t CoordiY, int8_t ChWord, uint16_t FontColor, uint16_t BackColor )
//{
//  uint8_t Tmp_Char = 0;
//  uint8_t i = 0, j = 0;

//  for(i = 0; i < 7; i++) {
//    Tmp_Char = ASCII_NUM_7x6[ChWord-48][i];
//    for(j = 0; j < 6; j++) {
//      if(((Tmp_Char >> (5 - j)) & 0x01) == 0x01)
//        OLED_DrawPixel(CoordiX+j, CoordiY+i, FontColor); // 字符顏色
//      else
//        OLED_DrawPixel(CoordiX+j, CoordiY+i, BackColor); // 背景顏色
//    }
//  }
//}
void OLED_PutChar_5x7( uint8_t posX, uint8_t posY, int8_t word, uint16_t fontColor, uint16_t backColor )
{
  uint8_t tmp = 0;

  for(uint8_t i = 0; i < 5; i++) {
    tmp = ASCII_NUM_5x7[word - 32][i];
    for(uint8_t j = 0; j < 7; j++) {
      if(((tmp >> (6 - j)) & 0x01) == 0x01)
        OLED_DrawPixel(posX + i, posY + 7 - j, fontColor);
      else
        OLED_DrawPixel(posX + i, posY + 7 - j, backColor);
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutStr
**功能 : Put String
**輸入 : posX, posY, pString, fontColor, backColor
**輸出 : None
**使用 : OLED_PutStr(10, 10, "TFT OLED TEST ... ", WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutStr( uint8_t posX, uint8_t posY, char *pString, uint16_t fontColor, uint16_t backColor )
{
  uint16_t offsetX = 0;

  while(*pString) {
    OLED_PutChar(posX + offsetX, posY, 12, 6, ASCII_12x6[*pString], fontColor, backColor);
    pString++;
    offsetX += 8;
  }
}

void OLED_PutStr_5x7( uint8_t posX, uint8_t posY, char *pString, uint16_t fontColor, uint16_t backColor )
{
  uint16_t offsetX = 0;

  while(*pString) {
    OLED_PutChar_5x7(posX + offsetX, posY, *pString, fontColor, backColor);
    pString++;
    offsetX += 6;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutNum
**功能 : Put Number
**輸入 : posX, posY, type, lens, number, fontColor, backColor
**輸出 : None
**使用 : OLED_PutNum(posX, posY, Type_D, lens, number, LCD_WHITE, LCD_BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutNum( uint8_t posX, uint8_t posY, StringType type, uint8_t lens, int32_t number, uint16_t fontColor, uint16_t backColor )
{
  char strBuf[16] = {0};

  num2Str(type, lens, strBuf, number);
  OLED_PutStr_5x7(posX, posY, strBuf, fontColor, backColor);
}

//void OLED_PutNum_5x7( uint8_t CoordiX, uint8_t CoordiY, StringType Type, uint8_t Length, int32_t NumData, uint16_t FontColor, uint16_t BackColor )
//{
//  char TmpNumber[16] = {0};

//  num2Str(Type, Length, TmpNumber, NumData);
//  OLED_PutStr_5x7(CoordiX, CoordiY, TmpNumber, FontColor, BackColor);
//}
//void OLED_PutNum_7x6( uint8_t CoordiX, uint8_t CoordiY, uint8_t Length, int32_t NumData, uint16_t FontColor, uint16_t BackColor )
//{
//  int8_t TmpNumber[16] = {0};
//  int8_t* ChWord = TmpNumber;
//  uint16_t OffsetX = 0;

//  Str_NumToChar(Type_D, Length, TmpNumber, NumData);

//  while(*ChWord) {
//    OLED_PutCharNum_7x6(CoordiX+OffsetX, CoordiY, *ChWord, FontColor, BackColor);
//    ChWord++;
//    OffsetX += 6;
//  }
//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_TestColoBar
**功能 : Draw Color Bar
**輸入 : None
**輸出 : None
**使用 : OLED_TestColoBar();
**====================================================================================================*/
/*====================================================================================================*/
#define COLOR_NUMBER      (16)
#define COLOR_BAR_HEIGHT  (OLED_W / COLOR_NUMBER)  // 96/16 = 6

void OLED_TestColoBar( void )
{
  uint16_t drawColor[COLOR_NUMBER] = {
    WHITE,
    RED,
    GREEN,
    BLUE,
    MAGENTA,
    GRED,
    GBLUE,
    BRED,
    BROWN,
    BRRED,
    CYAN,
    GRAY,
    YELLOW,
    DARKBLUE,
    LIGHTBLUE,
    GRAYBLUE
  };

  for(uint8_t i = 0; i < COLOR_NUMBER; i++)
    OLED_DrawRectFill(i * COLOR_BAR_HEIGHT, 0, COLOR_BAR_HEIGHT, OLED_H, drawColor[i]);
}
/*====================================================================================================*/
/*====================================================================================================*/
