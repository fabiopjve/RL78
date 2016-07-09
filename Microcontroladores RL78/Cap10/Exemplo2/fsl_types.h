/*================================================================================================*/
/* Project      = Flash self-programming library                                                  */
/* Module       = fsl_types.h                                                                     */
/* Device:      = RL78                                                                            */
/* Version      = V2.11                                                                           */
/*================================================================================================*/
/*                                  COPYRIGHT                                                     */
/*================================================================================================*/
/* Copyright(c) 2010-2011 by Renesas Electronics Europe GmbH,a company of the Renesas Electronics */
/* Corporation. All rights reserved.                                                              */
/*================================================================================================*/
/* Purpose:     definition of types used by the library                                           */
/*================================================================================================*/
/*                                                                                                */
/* Warranty Disclaimer                                                                            */
/*                                                                                                */
/* Because the Product(s) is licensed free of charge, there is no warranty of any kind whatsoever */
/* and expressly disclaimed and excluded by Renesas, either expressed or implied, including but   */
/* not limited to those for non-infringement of intellectual property, merchantability and/or     */
/* fitness for the particular purpose.                                                            */
/* Renesas shall not have any obligation to maintain, service or provide bug fixes for the        */
/* supplied Product(s) and/or the Application.                                                    */
/*                                                                                                */
/* Each User is solely responsible for determining the appropriateness of using the Product(s)    */
/* and assumes all risks associated with its exercise of rights under this Agreement, including,  */
/* but not limited to the risks and costs of program errors, compliance with applicable laws,     */
/* damage to or loss of data, programs or equipment, and unavailability or interruption of        */
/* operations.                                                                                    */
/*                                                                                                */
/* Limitation of Liability                                                                        */
/*                                                                                                */
/* In no event shall Renesas be liable to the User for any incidental, consequential, indirect,   */
/* or punitive damage (including but not limited to lost profits) regardless of whether such      */
/* liability is based on breach of contract, tort, strict liability, breach of warranties,        */
/* failure of essential purpose or otherwise and even if advised of the possibility of such       */
/* damages. Renesas shall not be liable for any services or products provided by third party      */
/* vendors, developers or consultants identified or referred to the User by Renesas in            */
/* connection with the Product(s) and/or the Application.                                         */
/*                                                                                                */
/*================================================================================================*/
/* Environment: IAR environment for RL78                                                          */
/*================================================================================================*/



#ifndef __FSL_TYPES_H_INCLUDED
#define __FSL_TYPES_H_INCLUDED



/*==============================================================================================*/
/* unsigned type definitions                                                                    */
/*==============================================================================================*/
typedef unsigned char                       fsl_u08;
typedef unsigned int                        fsl_u16;
typedef unsigned long int                   fsl_u32;


/*==============================================================================================*/
/* global constant definitions                                                                  */
/*==============================================================================================*/


/*==============================================================================================*/
/* global type definitions                                                                      */
/*==============================================================================================*/

/* FSL descriptor type */
typedef struct {                                                  /* ---------------------------------------------- */
                  fsl_u08         fsl_flash_voltage_u08;          /* 1, setting of flash voltage mode               */
                  fsl_u08         fsl_frequency_u08;              /* 1, CPU frequency (MHz)                         */
                  fsl_u08         fsl_auto_status_check_u08;      /* 1, setting of status check mode                */
                } fsl_descriptor_t;                               /*------------------------------------------------*/
                                                                  /* 3 bytes in total                               */
                                                                  /*------------------------------------------------*/


/* FSL write type */
typedef struct {                                                  /* ---------------------------------------------- */
           __near fsl_u08*        fsl_data_buffer_p_u08;          /* 2, pointer to data buffer                      */
                  fsl_u32         fsl_destination_address_u32;    /* 4, starting address to write data              */
                  fsl_u08         fsl_word_count_u08;             /* 1, number of words to write data               */
                } fsl_write_t;                                    /*------------------------------------------------*/
                                                                  /* 7 bytes in total                               */
                                                                  /*------------------------------------------------*/


/* FSL get block end address type */
typedef struct {                                                  /* ---------------------------------------------- */
                  fsl_u32         fsl_destination_address_u32;    /* 4, destination variable for the end address    */
                  fsl_u16         fsl_block_u16;                  /* 2, block number                                */
                } fsl_getblockendaddr_t;                          /*------------------------------------------------*/
                                                                  /* 6 bytes in total                               */
                                                                  /*------------------------------------------------*/


/* FSL get and set flash shield window type */
typedef struct {                                                  /* ---------------------------------------------- */
                  fsl_u16         fsl_start_block_u16;            /* 2, start block number of FSW (destination)     */
                  fsl_u16         fsl_end_block_u16;              /* 2, end block number of FSW (destination)       */
                } fsl_fsw_t;                                      /*------------------------------------------------*/
                                                                  /* 4 bytes in total                               */
                                                                  /*------------------------------------------------*/


#endif
