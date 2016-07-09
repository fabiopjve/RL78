/************************************************************************/
/*  LCD.C - Biblioteca de manipulação de módulo LCD no RL78             */
/*                                                                      */
/*  Autor: Fábio Pereira                                                */
/*                                                                      */
/************************************************************************/

// As definições a seguir são utilizadas para acesso aos pinos do display
#ifndef PIN_LCD_EN
#define PIN_LCD_EN          P5_bit.no4  // pino enable do LCD
#define PIN_LCD_EN_DIR	    PM5_bit.no4 // direção do pino enable
#define PIN_LCD_RS          P5_bit.no5  // pino rs do LCD
#define PIN_LCD_RS_DIR      PM5_bit.no5 // direção do pino rs
#define PORT_LCD_DATA4      P7
#define PORT_LCD_DATA4_DIR  PM7
#endif

#define CPU_CLOCK           32000

// Endereço da segunda linha na RAM do LCD
#define LCD_LIN2            0x40        

// Definições utilizadas para configuração do display
#define CURSOR_ON           2
#define CURSOR_OFF          0
#define CURSOR_BLINK        1
#define CURSOR_FIXED        0
#define DISPLAY_ON          4
#define DISPLAY_OFF         0
#define DISPLAY_8x5         0
#define DISPLAY_10x5        4
#define _2LINES             8
#define _1LINE              0

void LCD_delay_ms(unsigned char time)
{
  unsigned int temp;
  for (;time;time--) for (temp=CPU_CLOCK/9;temp;temp--) __no_operation();
}

//**************************************************************************
//* Função de envio de um nibble para o display
//**************************************************************************
//* Argumentos de chamada:
//* char dado : dado a ser enviado para o display (somente o nibble inferior)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send4(char data)
{
  // coloca os quatro bits nas saidas
  PORT_LCD_DATA4 = data & 0x0F;
  // pulsa a linha enable
  PIN_LCD_EN = 1;
  PIN_LCD_EN = 0;
}
//**************************************************************************
//* Função de escrita de 1 byte no display
//**************************************************************************
//* Argumentos de chamada:
//* char endereco : 0 se instrução, 1 se dado
//* char dado : dado ou comando a ser escrito
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send8(char addr, char data )
{
  // configura a linha rs dependendo do modo selecionado
  PIN_LCD_RS = addr;
  LCD_delay_ms(1);
  // envia a primeira parte do byte
  LCD_send4(data >> 4);
  // envia a segunda parte do byte
  LCD_send4(data & 0x0f);
}

//**************************************************************************
//* Função de inicialização do display
//**************************************************************************
//* Argumentos de chamada:
//* char modo1 : modo do display (número de linhas e tamanho dos caracteres
//* char modo2 : modo do display (estado do cursor e do display)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_init(char md1, char md2 )

{
  unsigned char temp;
  // configura os pinos como saídas
  PIN_LCD_EN_DIR = 0;
  PIN_LCD_RS_DIR = 0;
  PORT_LCD_DATA4_DIR = 0xF0;
  // coloca os pinos em nível lógico 0
  PORT_LCD_DATA4 = 0;
  PIN_LCD_RS = 0;
  PIN_LCD_EN = 0;
  LCD_delay_ms(15);
  // envia uma seqüência de 3 vezes 0x03 e depois 0x02 para configurar o módulo
  // no modo de 4 bits
  for(temp=3;temp;temp--)
  {
    LCD_send4(3);
    LCD_delay_ms(5);
  }
  LCD_send4(2);
  // envia códigos de inicialização do display
  LCD_send8(0,0x20 | md1);
  LCD_send8(0,0x08 | md2);
  LCD_send8(0,1);
  LCD_send8(0,6);
}

//**************************************************************************
//* Função de posicionamento do cursor do display
//**************************************************************************
//* Argumentos de chamada:
//* char x : coluna a ser posicionado o cursor (iniciando em 0)
//* char y : linha a ser posicionado o cursor (0 ou 1)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_pos_xy(char x, char y)
{
  unsigned char addr;
  addr = y * 64 + x;
  LCD_send8(0,0x80|addr);
}

//**************************************************************************
//* Função de escrita de um caractere no display
//**************************************************************************
//* Argumentos de chamada :
//* char c : caractere a ser escrito
//**************************************************************************
//* Retorno : nada
//**************************************************************************
//* Observações :
//*	\f apaga o conteúdo do display
//* \n e \r retornam o cursor para a primeira coluna da segunda linha
//**************************************************************************
void LCD_write_char(char data)
// envia um caractere para o display
{
   switch (data)
  {
    case '\f' 	:
      LCD_send8(0,1);
      LCD_delay_ms(5);
      break;
    case '\n'	:
    case '\r' 	:	
      LCD_pos_xy(0,1);
      break;
    default	:	
      LCD_send8(1,data);
  }
}

//**************************************************************************
//* Função de escrita de uma string no display
//**************************************************************************
//* Argumentos de chamada:
//* char *c : um ponteiro para um caractere
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_write_string (char *data)
{
  while (*data)
  {
    LCD_write_char(*data);
    data++;
  }
}
