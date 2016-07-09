/*******************************************************************************
  Livro RL78: Guia Básico
  Autor: Fábio Pereira
  2013

  Indicador de temperatura com o sensor interno do RL78/G13
  Placa RSK RL78/G13

  Mostra no display da placa a tensão de alimentação do chip e a temperatura
  ambiente.
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

volatile __saddr struct
{
  unsigned char display_update  : 1;
  unsigned char adc_channel     : 1;
} bits;

unsigned int acc_vref, vref, acc_vtemp, vtemp, vdd;
int toff;
unsigned long long_data;

#pragma vector = INTAD_vect
__interrupt void trata_ADC(void)
{
  unsigned int result;
  result = ADCR >> 6;   // lê o resultado da conversão
  // o bit adc_channel controla qual o canal do ADC sendo convertido
  if (bits.adc_channel)
  { // conversão da referência interna de tensão
    acc_vref += result - vref;  // acumula o novo valor, subtrai a média
    vref = acc_vref >> 6;       // calcula a média dos 64 últimos valores
    ADS = ADC_CH_TEMP;          // seleciona o sensor de temperatura
    bits.adc_channel = 0;
  } else
  {
    acc_vtemp += result - vtemp;  // acumula o novo valor, subtrai a média
    vtemp = acc_vtemp >> 6;       // calcula a média dos 64 últimos valores
    ADS = ADC_CH_REF;             // seleciona a referência interna
    bits.adc_channel = 1;
  }
}

#pragma vector = INTTM00_vect
__interrupt void trata_TAU0_canal0(void)
{
  bits.display_update = 1;  // atualiza o display a cada 500ms
}

void init(void)
{
  ADPC = 0;         // nenhum pino em modo analógico
  ADCEN = 1;        // habilita o ADC
  // Configura o ADC (conversões múltiplas, um canal, disparo por software)
  ADM0 = ADCLK_DIV64 | ADC_LV0 | bADCE;
  ADM1 = ADC_TRIG_SOFT;
  ADS = ADC_CH_REF; // seleciona a referência interna como entrada do ADC
  ADMK = 0;         // habilita interrupção do ADC
  ADCS = 1;         // inicia uma conversão
  TAU0EN = 1;       // habilita a TAU0
  TPS0 = TAU_CK0_DIV1024; // CK0=31250Hz, CK1=32MHz, CK2=16MHz e CK3=125kHz
  // configura o canal 0 da TAU0 no modo temporizador
  TMR00 = TAU_CK0 | TAU_TRIG_SOFT | TAU_MD_TIMER;
  TDR00 = 15624;    // uma interrupção a cada 500ms no canal 0
  TMMK00 = 0;       // habilita a interrupção do canal 0 da TAU0
  TS0L = TAU_CH0;   // Dispara o canal 0 
  LCD_init(DISPLAY_8x5|_2LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_FIXED);
  acc_vref = vref = 0;
  acc_vtemp = vtemp = 0;
  bits.adc_channel = 1;   // ADC convertendo tensão de referência
  __enable_interrupt(); // habilita as interrupções do RL78
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

void main(void)
{
  init();  
  LCD_write_char('\f');         // apaga o display
  toff = -500;                  // offset de temperatura (-5 graus)
  while(1)
  {
    // caso exista uma solicitação de atualização do display
    if (bits.display_update)
    {
      bits.display_update = 0;    // apaga o bit de atualização de display
      LCD_pos_xy(0,0);            // cursor na linha 0, coluna 0
      LCD_write_string("V=");     
      long_data = 148480 / vref;  // calcula o VDD
      vdd = long_data;
      LCD_write_frac_int(vdd);    // imprime o VDD (5V = 5.00)
      LCD_write_string("   ");    // apaga caracteres remanescentes
      LCD_pos_xy(0,1);            // cursor na linha 1, coluna 0
      LCD_write_string("T=");     
      // calcula a temperatura atual
      long_data = 2500 - ((long)vtemp*(long)vdd*100 - 10752000)/368 + toff;
      LCD_write_frac_int(long_data);  // imprime a temperatura
      LCD_write_string("   ");    // apaga caracteres remanescentes
    }    
  }
}

