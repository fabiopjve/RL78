/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do TAU modo PWM
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
  PM1 = 0x3F;	// P17 e P16 como saídas (canais 1 e 2 da TAU0)
  TAU0EN = 1;   	// habilita a TAU0
  TPS0 = TAU_CK0_DIV32;	// CK0=1MHz, CK1=32MHz, CK2=16MHz e CK3=125kHz
  // configura o canal 0 no modo mestre para PWM
  TMR00 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_TIMER_TRIG_INT;
  // configura os canais 1 e 2 no modo escravo para PWM
  TMR01 = TAU_CK0 | TAU_TRIG_MASTER | TAU_MD_ONECOUNT_TRIG;
  TMR02 = TAU_CK0 | TAU_TRIG_MASTER | TAU_MD_ONECOUNT_TRIG;
  TDR00 = 999;	// período do sinal PWM = 1kHz
  TDR01 = 99;	// canal 1 = 10% de ciclo ativo
  TDR02 = 899;  // canal 2 = 90% de ciclo ativo
  TOE0L = TAU_CH2 | TAU_CH1; // habilita a saída dos canais 1 e 2 da TAU0
  // saída dos canais 1 e 2 controladas pelo mestre/escravo
  TOM0L = TAU_CH2 | TAU_CH1;
  // dispara os canais mestre e escravos
  TS0L = TAU_CH0 | TAU_CH1 | TAU_CH2;
  while (1);
}




