/*******************************************************************************

  Exemplo de utilização da FSL no RL78/G13

  LED3 = P6.3
  SW1 = P5.0

*******************************************************************************/
#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"
#include "flash.c"

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

#define LED3_RSK  P6_bit.no3
#define LED_RPB   P7_bit.no7
#define SW1       P5_bit.no0
// Tempo para debounce da tecla 5 = 50ms
#define KEY_TIMER 5     

volatile unsigned char save_status;

#pragma vector = INTIT_vect
__interrupt void trata_IT(void)
{
  static unsigned char debounce_counter, key_release;
  // faz o debounce da tecla SW1
  if (!SW1)
  {
    // se a tecla está pressionada
    if (debounce_counter) debounce_counter--; else
    {
      if (key_release)          // se a tecla estava liberada
      {
        LED3_RSK = !LED3_RSK;   // inverte o estado do led
        LED_RPB = !LED_RPB;     // inverte o estado do led
        key_release = 0;        // tecla não liberada
      }
      save_status = 1;  // solicita salvamento do estado do led
    }
  } else
  {
    // se a tecla não está pressionada
    debounce_counter = KEY_TIMER;   // novo tempo de debounce
    key_release = 1;    // tecla está liberada
  }
}

void MCU_init(void)
{
  PM7_bit.no7 = 0;  // P7.7 como saída
  PM6_bit.no3 = 0;  // P6.3 como saída
  LED_RPB = 1;      // led da RPB desligado
  LED3_RSK = 1;     // led 3 da RSK desligado
  // configura o LOCO (15kHz) como fonte de clock do IT/RTC
  OSMC = bWUTMMCK0;
  RTCEN = 1;        // habilita o RTC e o IT  
  // Configura o IT para uma interrupção a cada 150 contagens, ou seja:
  // 150/15000 = 10ms -> uma interrupção a cada 10ms
  ITMC = bRINTE | 149;
  ITMK = 0;         // habilita a interrupção do IT
  __enable_interrupt(); // habilita as interrupções do RL78
  // restaura o estado do led conforme o conteúdo da flash  
  if (flash_read8(0xFC00)==0) LED_RPB = LED3_RSK = 0; else
    LED_RPB = LED3_RSK = 1;
}

void main(void)
{
  MCU_init();   // inicializa o microcontrolador      
  flash_init(); // inicializa a FSL
  while (1)
  {
    if (save_status)    // se há requisição de salvamento na flash
    {
      save_status = 0;  // limpa a requisição
      flash_erase();    // apaga a flash
      // se o led está ligado, salva 0 no endereço 0xFC00
      // se o led está desligado, o endereço 0xFC00 permanece com 0xFF
      if (!LED_RPB) flash_write32(0,0xFC00);
    }
  }
}


