/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de saída de clock PCLBUZ
  Placa RPB RL78/G13
*******************************************************************************/

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"

// Configura watchdog
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF;
// Configura detector de baixa tensão
#pragma location = "OPTBYTE"
__root __far const char opbyte1 = LVD_OFF;
// oscilador 4MHz flash low speed
#pragma location = "OPTBYTE"
__root __far const char opbyte2 = FLASH_HS | CLK_32MHZ;
// debug ativado, com apagamento em casa de falha de autenticação
#pragma location = "OPTBYTE"
__root __far const char opbyte3 = DEBUG_OFF;
/* Configura security ID */
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};

void main (void)
{
  CMC = 0;	// desativa osciladores X1 e XT1
  PM14_bit.no0 = 0;		// pino P14.0 como saída
  CKS0 = bPCLOE | 7;		// ativa a saída PCLBUZ0 com clock = 3,9kHz
  while (1);
}

