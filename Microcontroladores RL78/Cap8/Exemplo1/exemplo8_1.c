/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do conversor A/D
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

#define LED P7_bit.no7

#pragma vector = INTAD_vect
__interrupt void trata_ADC(void)
{
  unsigned int result;
  result = ADCR >> 6;   // lê o resultado da conversão
  if (result>511) LED = 0; else LED = 1;
}

void main(void)
{
  PM7_bit.no7 = 0;  // pino P77 como saída
  LED = 0;          // apaga o led
  ADPC = 4;         // pinos P20, P21 e P22 no modo analógico
  ADCEN = 1;        // habilita o ADC
  // Configura o ADC (conversões múltiplas, um canal, disparo por software)
  ADM0 = ADCLK_DIV64 | ADC_LV0 | bADCE;
  ADM1 = ADC_TRIG_SOFT;
  ADS = ADC_CH2;    // seleciona o canal 2
  ADMK = 0;         // habilita interrupção do ADC
  __enable_interrupt(); // habilita as interrupções do RL78
  ADCS = 1;         // inicia uma conversão
  while (1);
}

