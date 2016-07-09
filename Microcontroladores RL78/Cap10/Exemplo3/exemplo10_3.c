/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do DMA com a SAU em modo UART (UART2)
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

#define __9600BPS     51 << 9
#define LED           P7_bit.no7
#define TX_BUF_SIZE   32

unsigned char tx_buffer[TX_BUF_SIZE];

void tx_UART2(unsigned char *string);

#pragma vector = INTSR2_vect
__interrupt void trata_rx_UART2(void)
{
  unsigned char temp;
  temp = RXD2;            // lê o caractere recebido
  // se recebeu 't' envia "Teste!"
  if (temp=='t') tx_UART2("Teste!\r\n");
  // se recebeu 'r' envia "RL78!"
  if (temp=='r') tx_UART2("RL78!\r\n");
  LED = !LED;             // a cada caractere recebido, inverte o estado do led
}

#pragma vector = INTDMA0_vect
__interrupt void trata_DMA_tx_uart2(void)
{
  DEN0 = 0;               // desabilita o DMA
}

void MCU_init(void)
{
  PM1_bit.no3 = 0;        // P13/TXD2 como saída
  P1_bit.no3 = 1;         // coloca TXD2 em 1 (importante!!!)
  PM1_bit.no4 = 1;        // P14/RXD2 como entrada
  PM7_bit.no7 = 0;        // P77 como saída (led)
  LED = 1;                // desliga o led
  SAU1EN = 1;             // ativa a SAU1
  // Clock CK0 da SAU1 = 32MHz / 32 = 1MHz
  SPS1 = SAU_CK0_DIV32;
  // Configura o canal 0 da SAU1 (transmissão da UART2)
  SMR10 = SAU_MD_UART;
  SCR10 = SAU_COMM_TX | SAU_NO_PARITY | SAU_LSB_FIRST | SAU_ONE_STOP | SAU_8BITS;
  SDR10 = __9600BPS;      // seta o baud rate do transmissor
  // Configura o canal 1 da SAU1 (recepção da UART2)
  SMR11 = bSAU_STS | SAU_MD_UART;
  SCR11 = SAU_COMM_RX | SAU_NO_PARITY | SAU_LSB_FIRST | SAU_ONE_STOP | SAU_8BITS;
  SDR11 = __9600BPS;      // seta o baud rate do receptor
  SOE1 = SAU_CH0;         // habilita a saída da UART2
  SO1 = SAU_CH0;          // seta a saída TXD2
  NFEN0 = SNFEN20;        // ativa o filtro digital da entrada RXD2
  // Dispara os canais 0 e 1 da SAU1
  SS1 = SAU_CH1 | SAU_CH0;
  SRMK2 = 0;              // habilita a interrupção de recepção da UART
  DMAMK0 = 0;             // habilita a interrupção do DMA0
  __enable_interrupt();   // habilita as interrupções do RL78
}

// envia uma string pela UART2 utilizando o DMA
void tx_UART2(unsigned char *string)
{
  unsigned char index;
  index = 0;
  // copia a string para o buffer de transmissão
  while (string)
  {
    tx_buffer[index++] = *string++;
    // se atingiu o final da string ou o tamanho do buffer, encerra a cópia
    if (index==TX_BUF_SIZE || !*string) break;
  }
  // se o buffer contém caracteres, configura o DMA para a transferência
  if (index)  
  {
    DRC0 = bDEN;                // habilita o DMA0
    DBC0 = index;               // configura o número de bytes a transferir
    DSA0 = (char)&TXD2;         // configura o endereço do registrador alvo
    DRA0 = (int)tx_buffer;      // configura o endereço do buffer com os dados
    DMC0 = bDRS | DMA_TRIG_TX2; // configura o canal de DMA
    DRC0 = bDEN | bDST;         // seta DST para iniciar a transferência
    STG0 = 1;                   // dispara o DMA e inicia a transferência
  }
}

void main(void)
{
  MCU_init();
  // transmite uma string de saudação
  tx_UART2("Teste da UART com DMA!");
  while (1);  // aguarda uma interrupção
}

