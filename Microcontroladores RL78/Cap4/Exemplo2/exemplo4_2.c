/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Exemplo de utilização do modo de baixo consumo stop
  Placa RPB RL78/G13

  Pisca dez vezes o led ao pressionar a tecla no pino P5.0, em seguida entra
  em modo stop
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

#define LED_D2 P7_bit.no7
 
void pisca(void)
{
  unsigned char cnt;
  unsigned long int temp;
  for (cnt=10;cnt;cnt--)
  {
    LED_D2 = 0;
    for (temp=100000;temp;temp--);
    LED_D2 = 1;
    for (temp=100000;temp;temp--);
  }
}

void main (void)
{
  P0=P1=P2=P3=P4=P5=P6=P7=P12=P14=0;
  // configura todos os pinos como saídas
  PM0=PM1=PM2=PM3=PM4=PM5=PM6=PM7=PM12=PM14=0;
  // configura pinos analógicos para modo digital
  ADPC = 1;
  PMC0=PMC12=PMC14=0;
  CMC = 0;	// desativa osciladores X1 e XT1
  PM5_bit.no0 = 1;    // pino P5.0 como entrada
  PU5_bit.no0 = 1;    // liga pull-up do pino P5.0
  EGN0 = BIT1;        // interrupção INTP1 na borda de descida
  PIF1 = 0;           // limpa o flag da interrupção INTP1
  PMK1 = 0;           // habilita a interrupção INTP1
  while(1)
  {
    pisca();          // pisca 10 vezes o led
    __stop();         // entra em modo stop
    // ocorreu um sinal de wake-up
    PIF1 = 0;         // limpa o flag da interrupção
  }
}


