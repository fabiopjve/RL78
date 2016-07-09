/*******************************************************************************
  Livro RL78: Guia B�sico
  Autor: F�bio Pereira
  2013

  Mostrador de barra anal�gica com utiliza��o do DMA para leitura do ADC
  Placa RSK RL78/G13

  Mostra um gr�fico de barra proporcional a posi��o do trimpot RV1
*******************************************************************************/

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"
#include "lcd_8x2.c"

// Configura watchdog
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF;
// Configura detector de baixa tens�o
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

volatile __saddr struct
{
  unsigned char display_update  : 1;
} bits;

unsigned int samples[16], result;
const char custom_char[] = 
{
  0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // uma coluna
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, // duas colunas
  0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, // tr�s colunas
  0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, // quatro colunas
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F  // cinco colunas  
};

#pragma vector = INTDMA0_vect
__interrupt void trata_DMA_ADC(void)
{
  unsigned char aux;
  DRC0 = bDEN;                // habilita o DMA0
  DBC0 = 16;                  // n�mero de palavras a transferir
  DSA0 = (char)&ADCR;         // endere�o do registrador fonte
  DRA0 = (int)samples;        // endere�o do buffer para as amostras
  // configura o canal de DMA: 16 bits, do SFR para a RAM, disparo pelo ADC
  DMC0 = bDS | DMA_TRIG_ADC;  
  DRC0 = bDEN | bDST;         // seta DST para iniciar a transfer�ncia
  result = 0;
  // soma todas as amostras (deslocando-as 6 bits � esquerda)
  for (aux=0; aux<16; aux++)
  {
    result += samples[aux]>>6;
  }
  result >>= 4;               // divide o resultado por 16
}

#pragma vector = INTTM00_vect
__interrupt void trata_TAU0_canal0(void)
{
  bits.display_update = 1;  // atualiza o display a cada 125ms
}

/*
  Desenha um gr�fico de barra no display LCD alfanum�rico utilizando
  os caracteres especiais criados: | || ||| |||| e |||||
  O par�metro pix � o n�mero de pixels a desenhar
*/
void LCD_bargraph(unsigned char pix)
{
  char cnt = 8;       // n�mero m�ximo de colunas
  while (pix)
  {
    if (pix>=5)       // se restar mais de 5 pixels a desenhar
    {
      LCD_write_char(4);  // desenha um bloco preto
      cnt--;              // uma coluna a menos
      pix -= 5;           // cinco pixels a menos
    } else                // se restar menos de 5 colunas a desenhar
    {
      LCD_write_char(pix-1);  // desenha um bloco parcial
      pix = 0;            // n�o h� mais pixels restantes
      cnt--;              // uma coluna a menos
    }    
  }
  // preenche as colunas restantes com espa�os
  while (cnt--) LCD_write_char(' ');
}


void init(void)
{
  unsigned char aux;
  ADPC = 8;         // pinos P20, P21 e P22 no modo anal�gico
  ADCEN = 1;        // habilita o ADC
  // Configura o ADC (convers�es m�ltiplas, um canal, disparo por software)
  ADM0 = ADCLK_DIV16 | ADC_LV0 | bADCE;
  ADM1 = ADC_TRIG_SOFT;
  ADS = ADC_CH6;    // seleciona o canal 6 como entrada do ADC
  ADCS = 1;         // inicia uma convers�o
  DRC0 = bDEN;                // habilita o DMA0
  DBC0 = 16;                  // n�mero de palavras a transferir
  DSA0 = (char)&ADCR;         // endere�o do registrador fonte
  DRA0 = (int)samples;        // endere�o do buffer para as amostras
  // configura o canal de DMA: 16 bits, do SFR para a RAM, disparo pelo ADC
  DMC0 = bDS | DMA_TRIG_ADC;
  DRC0 = bDEN | bDST;         // seta DST para iniciar a transfer�ncia
  TAU0EN = 1;       // habilita a TAU0
  TPS0 = TAU_CK0_DIV1024; // CK0=31250Hz, CK1=32MHz, CK2=16MHz e CK3=125kHz
  // configura o canal 0 da TAU0 no modo temporizador
  TMR00 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_TIMER;
  TDR00 = 3905;     // uma interrup��o a cada 125ms no canal 0
  TS0L = TAU_CH0;   // Dispara o canal 0   
  LCD_init(DISPLAY_8x5|_2LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_FIXED);
  // configura os caracteres especiais na CGRAM
  LCD_send8(0,0x40);          // aponta para a �rea da CGRAM
  for (aux=0; aux<sizeof(custom_char); aux++)
  {
    LCD_send8(1,custom_char[aux]);  // envia os dados para o LCD
  }
  DMAMK0 = 0;               // habilita a interrup��o do DMA0
  TMMK00 = 0;               // habilita a interrup��o do canal 0 da TAU0
  __enable_interrupt();     // habilita as interrup��es do RL78
}

void main(void)
{
  init();  
  LCD_write_char('\f');         // apaga o display
  LCD_pos_xy(0,0);              // posiciona o cursor na coluna 0 da linha 0
  LCD_write_string("RL78/G13");
  while(1)
  {
    if (bits.display_update)
    {
      bits.display_update = 0;  // apaga o indicador de atualiza��o
      LCD_pos_xy(0,1);          // posiciona o cursor na coluna 0 da linha 1
      LCD_bargraph(result/25);  // desenha o gr�fico de barras
    }
  }
}

