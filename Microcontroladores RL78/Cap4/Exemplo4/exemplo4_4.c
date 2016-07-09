/*******************************************************************************
  Livro RL78: Guia B�sico
  Autor: F�bio Pereira
  2013

  Exemplo de utiliza��o do watchdog
  Placa RPB RL78/G13

  Pino P10 se aterrado provoca o reset pelo watchdog
*******************************************************************************/

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"

// Configura watchdog
#pragma location = "OPTBYTE"
__root __far const char config0 = bWDTON|bWDTINIT|WDT_WINDOW50|WDT_3799ms; 
// Configura detector de baixa tens�o
#pragma location = "OPTBYTE"
__root __far const char config1 = LVD_RESET_MODE | LVD_VMODE15; 
// oscilador 4MHz flash low speed
#pragma location = "OPTBYTE"
__root __far const char config2 = FLASH_LS | CLK_4MHZ;
// debug ativado, com apagamento em caso de falha de autentica��o
#pragma location = "OPTBYTE"
__root __far const char config3 = DEBUG_ON_ERASE; 
/* Configura security ID */
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};

#define LED P7_bit.no7
#define TCL P1_bit.no0
unsigned char temp;

void main(void)
{
  CMC = 0;	// desativa osciladores X1 e XT1
  PM7_bit.no7 = 0;		// configura LED como sa�da
  PU1_bit.no0 = 1;  		// liga pull up do pino P1.0
  // faz a leitura do registrador RESF e salva em uma vari�vel tempor�ria
  temp = RESF;
  // se WDTRF = 1 liga o led, sen�o apaga
  if (temp && bWDTRF) LED = 0; else LED = 1;
  while(1)
  {
    // caso o flag de interrup��o do WDT esteja setado
    if (WDTIIF)      
    {
      WDTE = 0xAC;  // apaga o watchdog
      WDTIIF = 0;   // apaga o flag
    }
    while (!TCL);   // se a tecla for pressionada, fica em loop
  }
}
