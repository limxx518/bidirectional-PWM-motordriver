#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_tim2.h"
#include "stm8s_tim4.h"
#include "stm8s_tim1.h"
#include "stm8s_tim2.h"
#include "stm8s_uart1.h"
#include "stm8s_iwdg.h"
#include "stm8s_clk.h"
#include "stm8s_rst.h"
#include "var.h"

/********************************************************************************************
* NAME:			void init()
*
* DESCRIPTION:		Initialize hardware (e.g. microcontroller peripherals)
*					and software variables (e.g. load default values)
*
* INPUTS:
*       PARAMETERS:	None
*       GLOBALS:	
*				All globals that need initialization are affected by this routine.
* OUTPUTS:
*
* PROCESS:
*			Refer to inline comments for more details.
*               [1] Configure clock(s) on STM8S
*               [2] Deinitialize all peripherals
*               [3] Configure GPIO
*               [4] Configure TIM1_CH4 for PWM
*               [5] Configure UART
*               [6] Configure TIM2, overflow every ms
*               [7] Configure independent watchdog timer, reset MCU every 512ms
* NOTES:
* CHANGES:
* REF NO       DATE        WHO     DETAIL
*
********************************************************************************************/

void init(void)
{
    // Configure system clock using HSI clock 16MHz, CPU divider is 1, therefore fCPU is also 16MHz
    CLK_LSICmd(ENABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

    // Deinitialize all peripherals - in case the watchdog timer resets the MCU
    deinit();

    init_variable();

    init_TIM2();
    //init_PWM();
    init_GPIO();
    init_UART();
    init_IWDG();

}


/********************************************************************************************
* Name:				void deinit(void)
*
* Description:      This function deinitializes all peripherals - in case the watchdog timer resets the MCU
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
void deinit(void)
{
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    TIM2_DeInit();
    TIM1_DeInit();
    UART1_DeInit();
}

/********************************************************************************************
* Name:				void init_TIM2(void)
*
* Description:      This function initializes TIM2 to overflow and set the overflow flag every 1ms
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
void init_TIM2(void)
{
    // Configure TIM2, overflow interrupt every 1ms
    TIM2_TimeBaseInit(TIM2_PRESCALER_1, 15999);     
    TIM2_ClearFlag(TIM2_IT_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    //Do I need to use TIM2_PrescalerConfig to initialize PSCR register? Add bkpoint here and watch pscr reg
    //Ans: No need
}

/********************************************************************************************
* Name:				void init_variable(void)
*
* Description:      This function initializes program variables before program execution
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process          
*					
* Note:				Certain variables are set to 1 due to sequencing issues, see comments below 
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
void init_variable(void)
{
    uint8_t localu8;

    xmit_buff_head = 0x00;
    xmit_buff_tail = 0x00;
    xmit_buff_sending_flag = RESET;
    rx_index_buff = 0x00;

    for(localu8 = 0; localu8 < TX_BUFF_SIZE; localu8++)
        xmit_buff[localu8] = 0x00;
    for(localu8 = 0; localu8 < RX_BUFF_SIZE; localu8++)
        rx_buff[localu8] = 0x00;

    input_flag = IDLE;
    process_state = INPUT_STATE;
    input_mode = 0x00;

    for(localu8 = 0; localu8 < 6; localu8++)
    {
        //PWM_params[localu8] = "0x00";
        strcpy(PWM_params[localu8], "");
        PWM_ints[localu8] = 0x0000;
    }
    PWM_params_index = 0x00;
    PWM_ints_index = 0x00;
    cycle_count = 1;                //due to sequencing issues, we regard first cycle as the first count
    ms_count = 0;
    half_cycle_flag = 0;
    print_flag = 1;                 //also due to sequencing issues, 1 to force print first iteration
    prescaler = 0;
    ARR = 0;
    pulseWidth = 0;
    delay = DELAY_TIME;
    neutral_flag = 0;
    neutral_count = 0;
}

/********************************************************************************************
* Name:				void init_UART(void)
*
* Description:      This function initializes UART peripheral to a baud rate of 115200 bps
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
void init_UART(void)
{
    UART1->CR1 = 0;
    UART1->CR2 = 0b00101100;        // enable TX RX, enable RX interrupt
    UART1->CR3 = 0;
    UART1->BRR2 = 0x0B;
    UART1->BRR1 = 0x08;
    //Note: Writing of BRR2 must precede BRR1 because writing of BRR1 will update the baud counter.
}

/********************************************************************************************
* Name:				void init_GPIO(void)
*
* Description:      This function sets up the relevant GPIO pins for the program:
*                   [1]CW PIN->PC6
*                   [2]CCW PIN->PC5
*                   [3]PWM PIN->PC4
*                   [4]UART RX->PD6
*                   [5]UART->TX->PD5
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
void init_GPIO(void)
{
    //CW and CCW signal
    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

    //PWM pin
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);

    //UART pins
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);    // TX pin
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);        // RX pin
}

/********************************************************************************************
* Name:				void init_PWM(void)
*
* Description:      This function sets up the PWM peripheral depending on user inputs
*
* Inputs			N/A
*
* Outputs			N/A
*		
* Process          Depending on user inputs of PWM frequency, prescaler that gives the best timer resolution is
*                  determined. Auto-reload value(ARR) and pulseWidth is then determined accordingly. Depending on
*                  type of PWM center/edge-aligned, TIM1 is set up accordingly PWM1 mode is used, OC polarity 
*                  active high
*					
* Note:				
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
void init_PWM(void)
{
    uint32_t temp;
    prescaler = (((CLK_FREQ / PWM_ints[FREQ])) / 65536) + 1;
    ARR = ((CLK_FREQ / PWM_ints[FREQ])) / prescaler;
    temp = PWM_ints[DUTY] * (uint32_t)ARR;
    pulseWidth = (uint16_t)((temp) / 100);

    switch(input_mode){
    case 0x01: TIM1_TimeBaseInit(prescaler, TIM1_COUNTERMODE_UP, ARR, 0);
               break;
    case 0x02: TIM1_TimeBaseInit(prescaler, TIM1_COUNTERMODE_CENTERALIGNED3, ARR, 0);   //set flags for both up-down counting
               break;
    }

    TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, pulseWidth,
                  TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET);
    
    TIM1_OC4PreloadConfig(ENABLE);
    TIM1_ARRPreloadConfig(ENABLE);

    //enableInterrupts();
    TIM1->IER = 0x11;
    TIM1->CCER1 = 0x00;
    TIM1->CCER2 = 0x10;
    TIM1->BKR = 0x80;
}

/********************************************************************************************
* Name:				void init_IWDG(void)
*
* Description:      This function initializes independent watchdog timer
*                   - 128kHz LSI internal RC clock source
*                   - prescaler 256, the watchdog timer counts from 0xff to 0x00 in 1.02s
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
void init_IWDG(void)
{
    RST_ClearFlag(RST_FLAG_IWDGF);

    IWDG->KR = 0xCC;        // enable IWDG using software, not needed if option byte is already modified to enable IWDG
    
    IWDG->KR = 0x55;        // KEY_ACCESS code
    IWDG->PR = 0x06;        // prescaler = 256
    IWDG->RLR = 0xFF;       // reload counter = 0xFF

    IWDG->KR = 0xAA;        // KEY_REFRESH code
    IWDG->KR = 0xCC;        // start IWDG timer
}