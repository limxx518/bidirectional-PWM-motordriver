#define MAIN 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_tim2.h"
#include "stm8s_uart1.h"
#include "stm8s_iwdg.h"
#include "stm8s_clk.h"
#include "stm8s_rst.h"
#include "stm8s_tim1.h"
#include "stm8s_it.h"
#include "var.h"

          
 /*******************************************************************************************************
*	NAME:			main()
*
*	DESCRIPTION:	This function handles user entry of parameter inputs. Function coded using state machine approach
*                   and several infinite loops. Each infinite loop corresponds to a unique state, as indicated by the
*                   value of process_state variable. Infinite loop is used to wait for user input indefinitely. 
*                   There is only one way a loop is exited and that's when input is valid. User inputs are processed 
*                   using processCommand function that processes accordingly to each unique state. At any point in time
*                   when program is running, user can input x/X to halt execution and reset program.
*			
*	INPUTS:		    
*
*	OUTPUTS:	    
*
*	Notes:          
*
*	CHANGES:
*	REF NO		DATE		WHO		DETAIL
*
****************************************************************************************/

void main(void)
{
    init();

    LOOP1:printOptions();
    while(1)
    {
        IWDG->KR = 0xAA;
        if(input_flag == INPUT_RECEIVED) 
            processCommand();
        else if(input_flag == INPUT_INVALID) 
        {
            printf("Error:Invalid Input\r");
            printf("\r");
            printOptions();
            input_flag = IDLE;                         //Idle state is needed to avoid repeatedly printing options
        }
        else if(input_flag == INPUT_VALID) 
        {
            input_flag = IDLE;
            printf("Enter frequency, duty and cycles\r");
            break;
        }
    }

    LOOP2: while(1)
    {
        IWDG->KR = 0xAA;
        if(input_flag == INPUT_RECEIVED) 
            processCommand();
        else if(input_flag == INPUT_INVALID)
        {
            printf("Error:Invalid Input\r");
            printf("\r");
            while(delay--);
            delay = DELAY_TIME;
            printf("Enter frequency, duty and cycles\r");
            input_flag = IDLE;
        }
        else if(input_flag == CANCEL_STATE) 
        {
            input_flag = IDLE;
            process_state = INPUT_STATE;
            init();
            goto LOOP1;
        }
        else if(input_flag == INPUT_VALID) 
        {
            init_PWM();
            
            //This block of code for debugging purposes
            printf("PRE: %u\r", prescaler);
            while(delay--);                     //delay needed to make sure whole msg printed without buffer overrun
            delay = DELAY_TIME;
            printf("ARR: %u\r", ARR);
            printf("Duty: %u\r", pulseWidth);
            while(delay--);
            delay = DELAY_TIME;
            printf("\r");
        
            input_flag = IDLE;
            printf("Enter cycle duration(ms), neutral time(ms) and direction(1(CW)/2(CCW)/3(Both))\r");
            break;
        }
    }  

    LOOP3: while(1)
    {
        IWDG->KR = 0xAA;
        if(input_flag == INPUT_RECEIVED)
            processCommand();
        else if(input_flag == CANCEL_STATE)
        {
            input_flag = IDLE;
            process_state = INPUT_STATE;
            init();
            goto LOOP1;
        }
        else if(input_flag == INPUT_INVALID)
        {
            input_flag = IDLE;
            printf("Error:Invalid Input\r");
            printf("\r");
            while(delay--);
            delay = DELAY_TIME;
            printf("Enter cycle duration(ms), neutral time(ms) and direction(1(CW)/2(CCW)/3(Both))\r");
        }
        else if(input_flag == INPUT_VALID)
        {
            input_flag = IDLE;
            printf("Enter s/S to start\r");
        break;
        }
    }
            
    LOOP4: while(1)
    {
        IWDG->KR = 0xAA;
        if(input_flag == INPUT_RECEIVED) 
            processCommand();
        else if(input_flag == CANCEL_STATE) 
        {
            input_flag = IDLE;
            process_state = INPUT_STATE;
            init();
            goto LOOP1;
        }
        else if(input_flag == INPUT_INVALID)
        {
            printf("Error:Invalid Input\r");
            printf("\r");
            printf("Hit s/S to start\r");
            input_flag = IDLE;
        }
        else if(input_flag == INPUT_VALID)
        {
            input_flag = IDLE;
            TIM1_Cmd(ENABLE);
            TIM2_Cmd(ENABLE);
            break;
        }
    }
    
    LOOP5: while(1)
    {
        IWDG->KR = 0xAA;
        if(input_flag == INPUT_RECEIVED) processCommand();
        else if(input_flag == CANCEL_STATE) 
        {
            input_flag = IDLE;
            process_state = INPUT_STATE;
            init();
            goto LOOP1;
        }
        else if(print_flag)
        {
            print_flag = 0;
            printf("Cycle count: %lu\r", cycle_count);
            printf("Direction: %s\r",  half_cycle_flag ? "CCW":"CW");            
            printf("\r");  
            if(cycle_count == PWM_ints[COUNT])
            {
                if(PWM_ints[DIR]==3 && half_cycle_flag) while(delay--);
                else if(PWM_ints[DIR] == 1 || PWM_ints[DIR] == 2) while(delay--);
            }
        }
    }
}

/*******************************************************************************************************
*	NAME:			void printOptions(void)
*
*	DESCRIPTION:	This function prints the first block of input choices upon program startup or reset
*			
*	INPUTS:		    
*
*	OUTPUTS:	    
*
*	Notes:          
*
*	CHANGES:
*	REF NO		DATE		WHO		DETAIL
*
****************************************************************************************/
void printOptions(void)
{
    printf("Select test mode(enter number of selection):\r");
    printf("1: Edge-aligned PWM\r");
    printf("2: Center-aligned PWM\r");
    //printf("3: One-pulse mode\r");
}


/*******************************************************************************************************
*	NAME:			void processCommand(void)
*
*	DESCRIPTION:	This function is called from each infinite loop in main() when input is received. processComand
*                   sets the value of input_flag to be valid, invalid or a cancel state for the loop to 
*                   process subsequently
*			
*	INPUTS:		    
*
*	OUTPUTS:	    
*
*	Notes:          
*
*	CHANGES:
*	REF NO		DATE		WHO		DETAIL
*
****************************************************************************************/
void processCommand(void)
{
    //First block processes Options command
    if(process_state == INPUT_STATE)
    {
        if(strncmp("1",rx_buff, 1) == 0) input_mode = 0x01;
        else if (strncmp("2",rx_buff, 1) == 0) input_mode = 0x02;
        else if (strncmp("3",rx_buff, 1) == 0) input_mode = 0x03;
        else 
            input_flag = INPUT_INVALID;
        if(input_flag != INPUT_INVALID)
        {
            input_flag = INPUT_VALID;
            process_state = PARAM_STATE_1;
        }
    }
    
    //Second block processes 1st block of parameters
    else if(process_state == PARAM_STATE_1)
    {
        uint8_t ind;
        if(strncmp("X", rx_buff, 1) == 0) 
        {
            input_flag = CANCEL_STATE; 
            return;
        }
        ind = parseString(rx_buff);
        if(ind)
        {
            input_flag = INPUT_VALID;
            process_state = PARAM_STATE_2;
        }            
        else 
        {
            input_flag = INPUT_INVALID;
            PWM_params_index = 0;
        }
    }
    
    //Third block processes 2nd block of parameters
    else if(process_state == PARAM_STATE_2)
    {
        uint8_t ind;
        if(strncmp("X", rx_buff, 1) == 0) 
        {
            input_flag = CANCEL_STATE; 
            return;
        }
        ind = parseString(rx_buff);
        if(ind)
        {
            input_flag = INPUT_VALID;
            process_state = WAIT_FOR_START;
        }            
        else 
        {
            input_flag = INPUT_INVALID;
            PWM_params_index = 3;
        }
    }

    //Fourth block processes Start command
    else if(process_state == WAIT_FOR_START)
    {
        if(strncmp("S", rx_buff, 1) == 0)
        {
            input_flag = INPUT_VALID;
            process_state = RUNNING_STATE;
        }
        else if(strncmp("X", rx_buff, 1) == 0)  input_flag = CANCEL_STATE;    
        else 
            input_flag = INPUT_INVALID;
    } 
    
    else if(process_state == RUNNING_STATE)
    {
        if(strncmp("X", rx_buff, 1) == 0) input_flag = CANCEL_STATE;     
        else 
            input_flag = IDLE;
    }
}


/*******************************************************************************************************
*	NAME:			uint8_t parseString(char *str)
*
*	DESCRIPTION:	This function is called from the processCommand function to process parameter input strings
*                   entered by user. This function also does parameter checking and returns 0 if there's an error, 
*                   1 otherwise. Those return values will be subsequently processed by the processCommand function.
*			
*	INPUTS:	        (char*) str	    
*
*	OUTPUTS:	    uint8_t, 1 or 0
*
*	Notes:          
*
*	CHANGES:
*	REF NO		DATE		WHO		DETAIL
*
****************************************************************************************/		
uint8_t parseString(char *str)
{
    unsigned long test;
    uint8_t *ptr;
    uint8_t i,j;

    //This block separates the string to separate integer strings
    uint8_t *pch;
    //uint8_t *copy = (uint8_t *)malloc((strlen(str) + 1) * sizeof(uint8_t));
    uint8_t copy[25] = {0x00};
    //For some reason, malloc pointer wasn't successful, yet to determine why
    //~ i = (uint8_t)strlen(str);
    //~ copy = (uint8_t*)malloc((i+1) * sizeof(uint8_t));
    //~ printf("String length is %u\r", strlen(str));
    //~ printf("copy address is %x\r", &copy);
    //~ printf("copy is pointing to %s\r", copy);
    strcpy(copy, str);
    pch = strtok(copy, ",");
    while(pch != NULL && (PWM_params_index < 6))
    {
        strcpy(PWM_params[PWM_params_index++], pch);
        //printf("Copied string: %s\r", PWM_params[PWM_params_index++]);
        pch = strtok(NULL, ",");
    }
    
    //free(copy);

    //These two statements are to check if user input all required parameters
    if(process_state == PARAM_STATE_1 && PWM_params_index != 3) return 0x00;
    else if(process_state == PARAM_STATE_2 && PWM_params_index != 6) return 0x00;
    
    //This block converts the integer strings to integers and performs validations
    i = (process_state == PARAM_STATE_1) ? 0x00: 0x03;
    j = i;
    while( i < (j+3) )
    {
        test = strtoul(PWM_params[i], &ptr, 10);
        if(test) 
        {
            switch(i){
            case 0: if(test >= 10 && test <= 10000) PWM_ints[i] = (uint32_t)test;
                    else return 0x00;
                    break;
            case 1: if(test <= 100) PWM_ints[i] = (uint32_t)test;
                    else return 0x00;
                    break;
            case 2: if(!test) return 0x00;
                    else PWM_ints[i] = (uint32_t)test;
                    break;
            case 3: if(!test) return 0x00;
                    else PWM_ints[i] = (uint32_t)test;
                    break;
            case 5: if(test >= 1 && test <= 3) PWM_ints[i] = (uint32_t)test;
                    else return 0x00;
                    break;
            default: PWM_ints[i] = (uint32_t)test;
            }
        }
        else if(i == 1) PWM_ints[i] = 0;     //0 duty cycle is possible
        else if(i == 4) PWM_ints[i] = 0;     //0 neutral time is possible
        else return 0x00;      //fail
        i++;
    }
    return 0x01;            //success
}


/********************************************************************************************
* Name:				int putchar(const char c)
*
* Description:	 	This code section is called from printf		
*
* Inputs			a character to place on the xmit buffer
*
* Outputs			updated xmit_buff
*		
* Process			- We can never stall the system waiting for transmit
*					- We load data into the xmit buffer and index the head counter
*					- The isr increments the tail counter when running
*					- This section increments the tail counter when we start the isr for a new xmit
*					- If the head and tail are = we are not xmitting anything and ISR is off - ISR does this
*					- We have set the xmit_buff to a length of TX_BUFF_SIZE
*						* if we use multiple prints that overrun the buffer we wrap over our xmit buffer
*							We do not stall and wait for the xmit to catch up 
*					
* Note:			    - The STM8S MCU doesn't have UART0, so we can't disable printf using UART0 in project properties.
*                   - In order to force printf to use io_putchar, we have to follow the declaration of putchar function in stdio.h
*                   - putchar() must return 1, otherwise printf() will stop after first character
*                   - The information is taken from Raisonance ST7/STM8 Compiler manual, document version 01/25/2011
*
* Changes:			Date			Who			Detail
*					
********************************************************************************************/
int putchar(const char c)
{
    // move this character to the buffer -- we need to halt the isr interrupt to ensure we do not reindex in process
	_sim_();												// disable isr - ensure index is changed as load character to buffer

    // load the latest char
    xmit_buff[xmit_buff_head++] = (char)c;					// load the newest character
     
    // when the buffer has indexed to the end wrap to the beginning
	if (xmit_buff_head >= TX_BUFF_SIZE)				    	// we can only dump to end of buffer and then wrap back to 0
		xmit_buff_head = 0;									// loop back to 0
	_rim_();												// enable isr


	// If we are not sending rs232 then start the sending by initialing isr.
	if (xmit_buff_sending_flag == RESET)					// not sending anything so start sending
	{
		xmit_buff_sending_flag = SET;						// turn on the sending flag
		UART1_SendData8(xmit_buff[xmit_buff_tail++]);       // just send a normal character like ISR would have sent it
		UART1->CR2 = 0b10101100;			                // TIEN = 1 (bit 7), enable TX interrupt
		if(xmit_buff_tail >= TX_BUFF_SIZE)				    // increment to next char to send
			xmit_buff_tail = 0;								// loop back to 0
	}

    return(1);
}   