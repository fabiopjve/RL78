/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do timer de intervalo IT
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

#pragma vector = INTIT_vect
__interrupt void trata_IT(void)
{
  LED = !LED;     // inverte o estado do led
}

void main(void)
{
  PM7_bit.no7 = 0;  	// P7.7 como saída
  LED = 1;          	// led desligado
  CMC = 0;	        // desativa osciladores X1 e XT1
  // configura o LOCO (15kHz) como fonte de clock do IT/RTC
  OSMC = bWUTMMCK0;
  RTCEN = 1;            // habilita o RTC e o IT
  // Configura o IT para uma interrupção a cada 3000 contagens, ou seja:
  // 3000/15000 = 200ms -> uma interrupção a cada 200ms
  ITMC = bRINTE | 2999;
  ITMK = 0;             // habilita a interrupção do IT
  __enable_interrupt(); // habilita as interrupções do RL78
  while (1);
}
