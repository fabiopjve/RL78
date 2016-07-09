/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização da interrupção de teclado
  Placa RPB RL78/G13

  Pino P70/KR0 liga o led e P71/KR1 apaga o led
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

#define LED     P7_bit.no7
#define PIN_P70 P7_bit.no0
#define PIN_P71 P7_bit.no1

#pragma vector = INTKR_vect
__interrupt void trata_intkr(void)
{
  if (!PIN_P70) LED = 0;  // acende led
  if (!PIN_P71) LED = 1;  // apaga led
}

void main(void)
{
  PM7_bit.no7 = 0;    // configura LED como saída
  PU7 = BIT1 | BIT0;  // ativa pull-ups dos pinos P7.0 e P7.1
  KRM = BIT1 | BIT0;  // ativa entradas KR1 e KR0 no modo de interrupção
  KRIF = 0;           // apaga flag da interrupção de teclado
  KRMK = 0;           // habilita interrupção de teclado
  LED = 1;            // led desligado
  __enable_interrupt(); // habilita interrupções globais
  while (1);
}