/************************************************************************
 *  Exemplo de pisca led por software
 *  Autor: Fábio Pereira
 *  Livro: RL78: Guia Básico
 *  Placa YRPBRL78G13 
 */

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"

// Configura watchdog = desligado
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF; 
// Configura detector de baixa tensão = desligado
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

#define LED_D2  P7_bit.no7
 
volatile unsigned long int temp, cnt;
 
void main( void )
{
  PM7_bit.no7 = 0;  // porta P77 como saída
  cnt = 0;
  while (1)
  {
    LED_D2 = 1;
    for (temp=100000;temp;temp--);
    LED_D2 = 0;
    for (temp=100000;temp;temp--);
    cnt++;
  }
}
