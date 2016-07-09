/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização da biblioteca de LCD para RL78
  Placa RSK RL78/G13

*******************************************************************************/

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"
#include "lcd_8x2.c"

// Configura watchdog
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF;
// Configura detector de baixa tensão
#pragma location = "OPTBYTE"
__root __far const char opbyte1 = LVD_OFF;
// oscilador 32MHz flash high speed
#pragma location = "OPTBYTE"
__root __far const char opbyte2 = FLASH_HS | CLK_32MHZ;
// debug ativado, com apagamento em caso de falha de autenticação
#pragma location = "OPTBYTE"
__root __far const char opbyte3 = DEBUG_ON_ERASE;

/* Configura security ID */
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};

void main(void)
{
  LCD_init(DISPLAY_8x5|_2LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_FIXED);
  LCD_write_string("RL78/G13");
  LCD_pos_xy(0,1);
  LCD_write_string("LCD 8x2");
  while(1);
}