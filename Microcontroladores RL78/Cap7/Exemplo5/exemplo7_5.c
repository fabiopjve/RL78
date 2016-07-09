/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do TAU modo pulso configurável
  Placa RPB RL78/G13
*******************************************************************************/

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

void main(void)
{
  PM1_bit.no6 = 0;    // P16 como saída
  PU0_bit.no0 = 1;    // liga pull-up do pino P00
  TAU0EN = 1;   	// habilita a TAU0
  TPS0 = TAU_CK0_DIV32;  // CK0=1MHz, CK1=32MHz, CK2=16MHz e CK3=125kHz
  // configura o canal 0 no modo de atraso/disparo único
  TMR00 = TAU_CK0 | TAU_TRIG_VALID_EDGE | TAU_EDGE_FALLING | TAU_MD_ONECOUNT;
  // configura o canal 1 no modo de atraso/disparo único controlado pelo mestre
  TMR01 = TAU_CK0 | TAU_TRIG_MASTER | TAU_MD_ONECOUNT;
  TDR00 = 9999;   // atraso de 10ms
  TDR01 = 499;    // pulso de 500us
  TOE0L = TAU_CH1;	// habilita a saída do canal 1
  TOM0L = TAU_CH1;	// configura a saída do canal 1 para operar no modo
  TS0L = TAU_CH0 | TAU_CH1; // dispara os canais 0 e 1
  while (1);
}



