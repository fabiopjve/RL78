/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do TAU modo timer
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

#define LED   P7_bit.no7

#pragma vector = INTTM00_vect
__interrupt void trata_TAU0_canal0(void)
{
  LED = !LED;     // inverte o estado do led
}

void main(void)
{
  PM1_bit.no6 = 0;	// P16 como saída (canal 1 da TAU0)
  PM7_bit.no7 = 0;  	// P77 como saída
  TAU0EN = 1;   	// habilita a TAU0
  TPS0 = TAU_CK0_DIV32;	// CK0=1MHz, CK1=32MHz, CK2=16MHz e CK3=125kHz
  // configura os canais 0 e 1 da TAU0 no modo temporizador
  TMR00 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_TIMER;
  TMR01 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_TIMER;
  TDR00 = 49999;		// uma interrupção a cada 50ms no canal 0
  TDR01 = 499;		// sinal de 1kHz no canal 1
  TOE0L = TAU_CH1;	// habilita a saída do canal 1 da TAU0
  TMMK00 = 0;		// habilita a interrupção do canal 0 da TAU0
  TS0L = TAU_CH0 | TAU_CH1; // Dispara os canais 0 e 1
  __enable_interrupt(); // habilita as interrupções do RL78
  while (1);
}

