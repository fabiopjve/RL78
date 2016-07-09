/*******************************************************************************
  Livro RL78: Guia B�sico
  Autor: F�bio Pereira
  2013

  Exemplo de utiliza��o da SAU em modo UART (UART2)
  Placa RPB RL78/G13
*******************************************************************************/

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"

// Configura watchdog = desligado
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF; 
// Configura detector de baixa tens�o = desligado
#pragma location = "OPTBYTE"
__root __far const char opbyte1 = LVD_OFF; 
// oscilador 32MHz flash high speed
#pragma location = "OPTBYTE"
__root __far const char opbyte2 = FLASH_HS | CLK_32MHZ;
// debug ativado, com apagamento em caso de falha de autentica��o
#pragma location = "OPTBYTE"
__root __far const char opbyte3 = DEBUG_ON_ERASE; 
/* Configura security ID */
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};

#define __9600BPS     51 << 9
#define LED           P7_bit.no7

#pragma vector = INTSR2_vect
__interrupt void trata_rx_UART2(void)
{
  unsigned char temp;
  temp = RXD2;            // l� o caractere recebido
  TXD2 = temp+1;          // envia o caractere de c�digo seguinte
  if (temp=='a') LED = 0; // se recebeu "a", liga o led
  if (temp=='b') LED = 1; // se recebeu "b", desliga o led
}

#pragma vector = INTST2_vect
__interrupt void trata_tx_UART2(void)
{
}

void MCU_init(void)
{
  PM1_bit.no3 = 0;        // P13/TXD2 como sa�da
  P1_bit.no3 = 1;         // coloca TXD2 em 1 (importante!!!)
  PM1_bit.no4 = 1;        // P14/RXD2 como entrada
  PM7_bit.no7 = 0;        // P77 como sa�da (led)
  LED = 1;                // desliga o led
  SAU1EN = 1;             // ativa a SAU1
  // Clock CK0 da SAU1 = 32MHz / 32 = 1MHz
  SPS1 = SAU_CK0_DIV32;
  // Configura o canal 0 da SAU1 (transmiss�o da UART2)
  SMR10 = SAU_MD_UART | SAU_INT_BUFFER;
  SCR10 = SAU_COMM_TX | SAU_NO_PARITY | SAU_LSB_FIRST | SAU_ONE_STOP | SAU_8BITS;
  SDR10 = __9600BPS;      // seta o baud rate do transmissor
  // Configura o canal 1 da SAU1 (recep��o da UART2)
  SMR11 = bSAU_STS | SAU_MD_UART;
  SCR11 = SAU_COMM_RX | SAU_NO_PARITY | SAU_LSB_FIRST | SAU_ONE_STOP | SAU_8BITS;
  SDR11 = __9600BPS;      // seta o baud rate do receptor
  SOE1 = SAU_CH0;         // habilita a sa�da da UART2
  SO1 = SAU_CH0;          // seta a sa�da TXD2
  NFEN0 = SNFEN20;        // ativa o filtro digital da entrada RXD2
  // Dispara os canais 0 e 1 da SAU1
  SS1 = SAU_CH1 | SAU_CH0;
  SRMK2 = 0;              // habilita a interrup��o de recep��o da UART
  __enable_interrupt();   // habilita as interrup��es do RL78
}

void main(void)
{
  MCU_init();
  while (1);
}

