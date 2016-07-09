/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do TAU modo disparo único
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
  LED = 0;        // liga o led
  TS0L = TAU_CH1; // dispara o canal 1, inicia a temporização do atraso
}

#pragma vector = INTTM01_vect
__interrupt void trata_TAU0_canal1(void)
{
  LED = 1;        // desliga o led
}

void main(void)
{
  PM7_bit.no7 = 0;  // P77 como saída
  TAU0EN = 1;   	// habilita a TAU0
	// CK1=1MHz, CK2=16MHz e CK0=CK3=125kHz
  TPS0 = TAU_CK0_DIV256 | TAU_CK1_DIV32;  
  // configura o canal 0 no modo temporizador
  TMR00 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_TIMER;
  // configura o canal 1 no modo atraso/disparo único
  TMR01 = TAU_CK1 | TAU_TRIG_SOFT | TAU_MD_ONECOUNT ;
  TDR00 = 37499;  // canal 0 gera uma interrupção a cada 300ms
  TDR01 = 24999;  // canal 1 gera um atraso de 25ms
  TS0L = TAU_CH0; // dispara o canal 0
  TMMK00 = 0;	  // habilita a interrupção do canal 0 da TAU0
  TMMK01 = 0;     // habilita a interrupção do canal 1 da TAU0
  __enable_interrupt(); // habilita as interrupções do RL78
  while (1);
}


