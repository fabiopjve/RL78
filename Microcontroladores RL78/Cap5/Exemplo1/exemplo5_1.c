/************************************************************************
 *  Exemplo de E/S com o RL78
 *  Autor: Fábio Pereira
 *  Livro: RL78: Guia Básico
 *  Placa RSK RL78/G13
 */

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"

// Configura watchdog
#pragma location = "OPTBYTE"
__root __far const char config0 = WDT_OFF;
// Configura detector de baixa tensão
#pragma location = "OPTBYTE"
__root __far const char config1 = LVD_OFF;
// oscilador 32MHz flash high speed
#pragma location = "OPTBYTE"
__root __far const char config2 = FLASH_HS | CLK_32MHZ;
// debug ativado, com apagamento em caso de falha de autenticação
#pragma location = "OPTBYTE"
__root __far const char config3 = DEBUG_ON_ERASE;
/* Configura security ID */
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};

#define LED P5_bit.no2  /* LED no pino P5.2 */
#define SW1 P5_bit.no0  /* tecla no pino P5.0 */

void main(void)
{
  CMC = 0;	// desativa osciladores X1 e XT1
  PM5_bit.no2 = 0;  // configura LED (P5.2) como saída
  while (1)
  {
    // se SW1 estiver em "0" (fechada) liga o led, caso contrário desliga
    if (!SW1) LED=0; else LED=1;
  }
}


