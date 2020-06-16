#ifndef LCD_H_
#define LCD_H_
//------------------------------------------------
#include "stm32f4xx_hal.h"
#include "printf.h"

//------------------------------------------------
#define d4_set() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET)
#define d5_set() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET)
#define d6_set() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET)
#define d7_set() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET)
#define d4_reset() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET)
#define d5_reset() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET)
#define d6_reset() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET)
#define d7_reset() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET)
#define e1 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET)  //установка линии Е в 1
#define e0 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET) //установка линии Е в 0
#define rs1 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET) //установка линии RS в 1
#define rs0 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET) //установка линии RS в 0
//------------------------------------------------
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SendChar(char ch);
void LCD_String(char *st);
void LCD_SetPos(uint8_t x, uint8_t y);
void LCD_Stdout(void);
//------------------------------------------------
#endif /* LCD_H_ */
