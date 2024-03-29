/**
  ******************************************************************************
  * @file     stm8s_it.c
  * @author   MCD Application Team
  * @version  V2.0.1
  * @date     18-November-2011
  * @brief    Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "stm8s_gpio.h"
#include "stm8s_tim4.h"
#include "stm8s_tim1.h"
#include "stm8s_uart1.h"
#include "var.h"
#include "stm8s_tim2.h"
#include "stm8s_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/** @addtogroup TIM4_TimeBase
  * @{
  */
#ifdef _COSMIC_
/**
  * @brief  Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    while(1);
}
#endif /*_COSMIC_*/

/**
  * @brief  TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    while(1);
}
/**
  * @brief  Top Level Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    while(1);
}

/**
  * @brief  Auto Wake Up Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Clock Controller Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTA Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTB Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}


/**
  * @brief  External Interrupt PORTD Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
    
}

/**
  * @brief  External Interrupt PORTD Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTE Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#ifdef STM8S903
/**
  * @brief  External Interrupt PORTF Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  CAN TX Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief  SPI Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}


/********************************************************************************************
* Name:				TIM1 Overflow Handler
*
* Description:      ISR for TIM1 overflow
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process          Sets the CW and CCW pin high or low according to direction input 
*					
* Note:				
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
    if(PWM_ints[DIR] == 1 && neutral_flag == 0) 
    {
        GPIO_WriteHigh(CW_PIN);
        GPIO_WriteLow(CCW_PIN);
        half_cycle_flag = 0;
    }
    else if(PWM_ints[DIR] == 2 && neutral_flag == 0) 
    {
        GPIO_WriteHigh(CCW_PIN);
        GPIO_WriteLow(CW_PIN);
        half_cycle_flag = 1;
    }
    else if(PWM_ints[DIR] == 3 && neutral_flag == 0)
    {
        if(!half_cycle_flag) 
        {
            GPIO_WriteLow(CCW_PIN);
            GPIO_WriteHigh(CW_PIN);
        }
        else
        {
            GPIO_WriteLow(CW_PIN);
            GPIO_WriteHigh(CCW_PIN);
        }
    }
    else
    {
        GPIO_WriteLow(CW_PIN);
        GPIO_WriteLow(CCW_PIN);
    }
}


/********************************************************************************************
		
********************************************************************************************/
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
   TIM1_ClearFlag(TIM1_FLAG_CC4);   
}

#ifdef STM8S903
/**
  * @brief  Timer5 Update/Overflow/Break/Trigger Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Timer5 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/********************************************************************************************
* Name:				TIM2 Overflow Handler
*
* Description:      ISR for TIM2 overflow
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process          Keeps track of cycle duration and neutral time. Updates cycle_count as well.
*					
* Note:				
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    if(cycle_count == (PWM_ints[COUNT]+1))          //due to how we regard counting first cycle is 1. thus this offset
    {
        TIM1->BKR = 0x00;
        TIM1_Cmd(DISABLE);
        input_flag = CANCEL_STATE;
        print_flag = 0;
        return;
    }
    ms_count++;
    if(neutral_flag)
    {        
        neutral_count++;
        if(neutral_count == PWM_ints[NEUTRAL])
        {
            neutral_flag = 0;
            ms_count = 0;
            print_flag = 1;
            return;
        }
    }
    if(ms_count == PWM_ints[TIME] && neutral_flag == 0)
    {
        ms_count = 0;
        if(PWM_ints[NEUTRAL] != 0) neutral_flag = 1;
        neutral_count = 0;
        if(PWM_ints[DIR] == 3)
        {
            cycle_count += (uint32_t)half_cycle_flag;
            half_cycle_flag ^= 1;
            if(PWM_ints[NEUTRAL] == 0 && cycle_count <= PWM_ints[COUNT]) print_flag = 1;
        }
        else 
        {
            cycle_count++;        
            if(PWM_ints[NEUTRAL] == 0 && cycle_count <= PWM_ints[COUNT]) print_flag = 1;
        }
    }   
    else print_flag = 0;
}

/**
  * @brief  Timer2 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer3 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)


/********************************************************************************************
* Name:				INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
*
* Description:      - UART1 TX Interrupt routine
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process           - Continue to send all characters in the xmit_buff      
*					
* Note:            
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
    if (UART1_GetFlagStatus(UART1_FLAG_TXE) != RESET)
    {
        UART1_ClearFlag(UART1_FLAG_TXE);
    
        if (xmit_buff_tail == xmit_buff_head)				    // are we thru with xmit 
        {
            xmit_buff_sending_flag = RESET;	    			    // clear the sending flag
            UART1->CR2 = 0b00101100;      		                // TIEN = 0 (bit 7), enable TX interrupt
        }
        else
        {
            UART1_SendData8(xmit_buff[xmit_buff_tail++]);       // just send a normal character
            if (xmit_buff_tail >= TX_BUFF_SIZE)			        // we can only dump to end of buffer and then wrap back to 0
                xmit_buff_tail = 0;								// loop back to 0 if needed
        }
    }
}


/********************************************************************************************
* Name:				INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
*
* Description:      - UART1 RX Interrupt routine
*                   - Put all received characters in a UART, and notify main loop to check the command by setting command_flag
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process    
*					
* Note:            
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
    static uint8_t receive_char;

    UART1_ClearFlag(UART1_FLAG_RXNE);

    receive_char = UART1_ReceiveData8();
    
    if (receive_char == ASCII_CR)           //carriage return (ENTER key), end of command
    {
        rx_buff[rx_index_buff++] = ASCII_NULL;
        rx_index_buff = 0;
        input_flag = INPUT_RECEIVED;
        //command_flag = SET;
    }
    else if ((receive_char == BACK_SPACE) && (rx_index_buff > 0))
        rx_index_buff--;
    else if ((receive_char == LINE_FEED) || (receive_char >= EXTENDED_ASCII))
        ;   //do nothing
    else if ((receive_char <= 'z') && (receive_char >= 'a'))       // receive lower-case char
    {
        receive_char -= 32;     // change to upper-case char
        rx_buff[rx_index_buff++] = receive_char;
    }
    else
        rx_buff[rx_index_buff++] = receive_char;
}
#endif /*STM8S105*/

/**
  * @brief  I2C Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief  UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /* STM8S105*/

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
{

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    return;

}
#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */


/********************************************************************************************
* Name:				INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
*
* Description:      - ADC1 interrupt routine.
*                   - sample done flag is SET, so the main loop can take ADC sample values
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process     
*					
* Note:            
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
    //~ ADC1_ClearFlag(ADC1_FLAG_EOC);
    //~ ADC_sample_done_flag = SET;
    //~ return;
}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
/**
  * @brief  Timer6 Update/Overflow/Trigger Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */


/********************************************************************************************
* Name:				INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
*
* Description:      - Timer4 Update/Overflow Interrupt routine
*                   - TIM4 overflows every 1 ms
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process     
*					
* Note:            
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{

}
#endif /*STM8S903*/

/**
  * @brief  Eeprom EEC Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
