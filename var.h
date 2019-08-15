#include "700-1342.h"

#ifdef MAIN
    #define EXT  
#else
    #define EXT extern
#endif

//UART variables
EXT volatile uint8_t    xmit_buff[TX_BUFF_SIZE];
EXT volatile uint8_t    xmit_buff_head;
EXT volatile uint8_t    xmit_buff_tail;
EXT volatile uint8_t    xmit_buff_sending_flag;
EXT volatile uint8_t    rx_buff[RX_BUFF_SIZE];      // receive buffer
EXT volatile uint8_t    rx_index_buff;

//Command processing variables
EXT volatile uint8_t    input_flag;               // indicates input state
EXT volatile uint8_t    process_state;

//Command variables
EXT volatile uint8_t    input_mode;
EXT volatile uint8_t    print_flag;

//Parameter parsing array
EXT uint8_t                PWM_params[6][10];              //Array of C strings for PWM parameters
EXT uint8_t                PWM_params_index;
EXT uint32_t               PWM_ints[6];                    //32-bit to prevent overflow
EXT uint8_t                PWM_ints_index;

//Neutral lapse flag
EXT uint8_t                 neutral_flag;

//Cycle Count
EXT uint32_t                cycle_count;             //32-bit to prevent overflow

//Neutral Count
EXT uint32_t                neutral_count;

//ms Count
EXT uint32_t                ms_count;            //32-bit to prevent overflow

//Half Cycle Flag
EXT uint8_t                 half_cycle_flag;         //for function to cycle motor left and right (0 for CW, 1 for CCW)

//UART Delay
EXT uint16_t                delay;                   //To ensure full transmit

//Variables for PWM
EXT uint16_t prescaler;
EXT uint16_t ARR;
EXT uint16_t pulseWidth;