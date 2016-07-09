/*******************************************************************************

  Rotinas de manipulação da flash e FSL

*******************************************************************************/
#include "fsl_types.h"
#include "fsl.h"

#pragma location = "FLASH_DATA"
__no_init __far  unsigned char flash_data[1024];

static fsl_u08 my_fsl_status;
static fsl_descriptor_t fsl_descr;

// Escreve uma palavra de 32 bits num endereço de 32 bits da flash
void flash_write32(unsigned long data, unsigned long int address)
{
  unsigned long int array[1];
  fsl_write_t my_fsl_write_str;
  array[0] = data;
  my_fsl_write_str.fsl_data_buffer_p_u08 = (fsl_u08 __near*)array;
  my_fsl_write_str.fsl_word_count_u08 = 0x01;
  my_fsl_write_str.fsl_destination_address_u32 = address;
  my_fsl_status = FSL_Write((fsl_write_t __near*)&my_fsl_write_str);
}

unsigned char flash_read8(unsigned long addr)
{
  return(*(__far char *)addr);  // lê um byte da flash
}

void flash_erase(void)
{
  FSL_Erase(63);  // apaga o bloco 63 (0xFC00 a 0xFFFF)
}

void flash_init(void)
{
  // Inicialização da FSL
  fsl_descr.fsl_flash_voltage_u08 = 0;  // full speed mode
  fsl_descr.fsl_frequency_u08 = 32;     // 32MHz
  // Modo da FSL = SCI - checagem de status interna
  fsl_descr.fsl_auto_status_check_u08 = 0x01;
  // Inicializa a FSL
  FSL_Init((__far fsl_descriptor_t*)&fsl_descr);
  FSL_Open(); // abre uma sessão FSL
  FSL_PrepareFunctions(); // inicializa funções básicas FSL
}