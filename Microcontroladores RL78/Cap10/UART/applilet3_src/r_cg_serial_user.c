/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2010, 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : Applilet3 for RL78/G13 V1.03.01 [11 Oct 2011]
* Device(s)    : R5F100LE
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for Serial module.
* Creation Date: 4/4/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t * gp_uart2_tx_address;         /* uart2 send buffer address */
extern uint16_t  g_uart2_tx_count;            /* uart2 send data number */
extern uint8_t * gp_uart2_rx_address;         /* uart2 receive buffer address */
extern uint16_t  g_uart2_rx_count;            /* uart2 receive data number */
extern uint16_t  g_uart2_rx_length;           /* uart2 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart2_interrupt_receive
* Description  : This function is INTSR2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTSR2_vect
__interrupt static void r_uart2_interrupt_receive(void)
{
    uint8_t rx_data;

    rx_data = RXD2;

    if (g_uart2_rx_length > g_uart2_rx_count)
    {
        *gp_uart2_rx_address = rx_data;
        gp_uart2_rx_address++;
        g_uart2_rx_count++;

        if (g_uart2_rx_length == g_uart2_rx_count)
        {
            r_uart2_callback_receiveend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_uart2_interrupt_error
* Description  : This function is INTSRE2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTSRE2_vect
__interrupt static void r_uart2_interrupt_error(void)
{
    uint8_t err_type;

    *gp_uart2_rx_address = RXD2;
    r_uart2_callback_error(err_type);
}

/***********************************************************************************************************************
* Function Name: r_uart2_interrupt_send
* Description  : This function is INTST2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTST2_vect
__interrupt static void r_uart2_interrupt_send(void)
{
    if (g_uart2_tx_count > 0U)
    {
        TXD2 = *gp_uart2_tx_address;
        gp_uart2_tx_address++;
        g_uart2_tx_count--;
    }
    else
    {
        r_uart2_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_receiveend
* Description  : This function is a callback function when UART2 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_sendend
* Description  : This function is a callback function when UART2 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_error
* Description  : This function is a callback function when UART2 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
