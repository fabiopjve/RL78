/*******************************************************************************
  Livro RL78: Guia B�sico
  Autor: F�bio Pereira
  2013

  Exemplo de utiliza��o do RTC
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

#define LED   P7_bit.no7

struct
{
  unsigned char hour, minute, second;
  unsigned char year, month, week, day;
  unsigned char set : 1;
  unsigned char update : 1;
} time;

#pragma vector = INTRTC_vect
__interrupt void trata_RTC(void)
{
  LED = !LED;     // inverte o estado do led
  // solicita o bloqueio dos contadores caso a atualiza��o esteja ativa
  if (time.update) RWAIT = 1;
}

void main(void)
{
  PM7_bit.no7 = 0;  	// P7.7 como sa�da
  LED = 1;          	// led desligado
  CMC = OSC_XT1_OSC;    // oscilador XT1 ligado no modo de baixa pot�ncia
  CSC = bMSTOP;         // libera oscilador XT1 e mant�m X1 parado
  OSMC = 0;             // configura o XT1 como fonte de clock do IT/RTC
  RTCEN = 1;            // habilita o RTC e o IT 
  // habilita o RTC, modo 24 horas, interrup��o peri�dica de 1 segundo
  RTCC0 = bRTCE | bAMPM | INT_RTC_1S;
  RTCC1 = 0;
  RTCMK = 0;            // habilita a interrup��o do RTC
  time.set = 0;
  time.update = 1;
  __enable_interrupt(); // habilita as interrup��es do RL78
  while (1)
  {
    if (time.set)
    { // se houver requisi��o de atualiza��o de hor�rio do RTC
      RWAIT = 1;        // solicita bloqueio dos contadores
    }
    if (RWST)
    { // se o indicador de bloqueio dos contadores estiver setado
      if (time.set)
      { // setar o hor�rio do rel�gio
        HOUR = time.hour;
        MIN = time.minute;
        SEC = time.second;
        YEAR = time.year;
        MONTH = time.month;
        WEEK = time.week;
        DAY = time.day;        
        time.set = 0; // apaga o indicador de atualiza��o
      } else
      { // � uma atualiza��o do mostrador
        time.hour = HOUR;
        time.minute = MIN;
        time.second = SEC;
        time.year = YEAR;
        time.month = MONTH;
        time.week = WEEK;
        time.day = DAY;        
      }
      RWAIT = 0;  // desbloqueia o clock dos contadores do RTC
      time.update = 1;  // ativa a atualiza��o do mostrador
    }
  }
}
