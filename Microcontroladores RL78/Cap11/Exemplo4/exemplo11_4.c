/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Medidor de distância com módulo ultrasônico HC-SR04
  Placa RSK RL78/G13

  Gera periodicamente um pulso de disparo no pino TRIG do módulo e aguarda
  por um pulso de eco na entrada ECHO conectada a entrada do canal 4 da TAU0
  A aplicação utiliza o canal 0 para geração dos pulsos de disparo e o canal 4
  (no modo de captura) para medição do pulso de saída do módulo.
*******************************************************************************/

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"
#include "lcd_8x2.c"

// Configura watchdog
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF;
// Configura detector de baixa tensão
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

#define TRIGGER       P4_bit.no1

volatile unsigned int interval;
unsigned long distance;

volatile __saddr struct
{
  unsigned char display_update  : 1;
  unsigned char echo_received   : 1;
  unsigned char trig_mode       : 1;
  unsigned char aux             : 1;
} bits;

#pragma vector = INTTM00_vect
__interrupt void trata_TAU0_canal0(void)
{
  if (bits.trig_mode)
  {
    TRIGGER = 1;
    TDR00 = 9; // intervalo de 10us para o disparo
    TS0L = TAU_CH0;
    bits.echo_received = 0;
  } else
  {
    TRIGGER = 0;
    TDR00 = 0xFFFF;
    TS0L = TAU_CH0;
    if (!bits.echo_received) interval=0xFFFF;
  }
  bits.trig_mode = !bits.trig_mode;
}

#pragma vector = INTTM04_vect
__interrupt void trata_TAU0_canal4(void)
{
  interval = TDR04;         // salva o valor capturado
  bits.echo_received = 1;   // sinaliza que recebeu um pulso de eco
  bits.aux = !bits.aux;
  if (bits.aux) bits.display_update = 1;  // atualiza o display
}

/*
  Esta função escreve um valor inteiro no LCD. Ela utiliza uma formatação
  especial para representar um valor fracionário através de inteiros.
  Os dígitos de dezenas de milhares, milhares e centenas são impressos,
  seguidos do ponto decimal e dos dígitos de dezenas e unidades.
  Assim, o valor 1001 é apresentado como 10.01.
*/
void LCD_write_frac_int(unsigned int data)
{
  unsigned char aux, space;
  unsigned int sub = 10000;
  aux = 0;
  space = 1;
  while (sub)
  {
    aux = 0;
    while (data>=sub)
    {
      data -= sub;
      aux++;
      space = 0;
    }
    if (!space) LCD_write_char(aux+'0');
    sub /= 10;
    if (sub==10)
    {
      if (space) LCD_write_char('0');
      LCD_write_char('.');
      space = 0;
    }
  }
  if (space) LCD_write_char('0');
}


void TAU_init(void)
{
  TAU0EN = 1;         // habilita a TAU0
  TPS0 = TAU_CK0_DIV32; // CK0=1MHz, CK1=32MHz, CK2=16MHz e CK3=125kHz
  // configura o canal 0 da TAU0 no modo temporizador
  TMR00 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_ONECOUNT;
  TDR00 = 9;          // intervalo inicial de 10us
  // configura o canal 4 da TAU0 no modo de captura de ciclo ativo
  TMR04 = TAU_CK0 | TAU_EDGE_RISE_FALL | TAU_TRIG_BOTH_EDGE | TAU_MD_CAPTURE_LEVEL;
  TS0L = TAU_CH4 | TAU_CH0; // Arma os canais 0 e 4
  TMMK00 = 0;         // habilita a interrupção do canal 0 da TAU0
  TMMK04 = 0;         // habilita a interrupção do canal 4 da TAU0
}

void SYS_init(void)
{
  PM4_bit.no1 = 0;        // P41 como saída (TRIGGER)
  TAU_init();             // inicializa a TAU0
  LCD_init(DISPLAY_8x5|_2LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_FIXED);
  LCD_write_char('\f');   // apaga o display
  __enable_interrupt();   // habilita as interrupções do RL78
}

void main(void)
{
  SYS_init();
  LCD_pos_xy(0,0);              // posiciona o cursor na coluna 0 da linha 0
  LCD_write_string("Distanc");
  // inicia um pulso de disparo para o módulo de ultrassom
  TRIGGER = 1;
  bits.trig_mode = 0;
  TS0L = TAU_CH0;               // Dispara o canal 0
  while(1)
  {
    if (bits.display_update)
    {
      bits.display_update = 0;  // apaga o bit de atualização
      LCD_pos_xy(0,1);          // posiciona o cursor na coluna 0 da linha 1
      // calcula a distância em cm
      distance = ((long)interval*100)/58;
      if (distance<=30000)      // se a distância for menor que 300cm
      {
        LCD_write_frac_int(distance);
        LCD_write_string("cm   ");
      } else LCD_write_string("---.--cm");
    }
  }
}



