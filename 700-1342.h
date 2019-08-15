//********* UART *********//
#define RX_BUFF_SIZE            25
#define TX_BUFF_SIZE            100
#define BACK_SPACE              0x08
#define LINE_FEED               0x0A
#define ASCII_NULL              0x00
#define EXTENDED_ASCII          0x80
#define ASCII_CR                0x0D
#define DELAY_TIME              65535

//Clock frequency
#define CLK_FREQ                0xF42400       //16M 

//Input Processing States for input_flag
#define IDLE                    0x00            //default initial state
#define INPUT_RECEIVED          0x01
#define INPUT_VALID             0x02
#define INPUT_INVALID           0x03

//Processing States for process_state
#define INPUT_STATE             0x04
#define PARAM_STATE_1           0x05
#define PARAM_STATE_2           0x06
#define WAIT_FOR_START          0x07
#define RUNNING_STATE           0x08
#define CANCEL_STATE            0x09

//Parameter parsing indices
#define FREQ                    0x00
#define DUTY                    0x01
#define COUNT                   0x02
#define TIME                    0x03
#define NEUTRAL                 0x04
#define DIR                     0x05


//Pin definitions
#define CW_PIN                  GPIOC, GPIO_PIN_6
#define CCW_PIN                 GPIOC, GPIO_PIN_5

//Function Definitions
void init(void);
void deinit(void);              // deinitialize all peripherals - in case watchdog timer resets the MCU
void init_TIM2(void);           // configure TIM2
void init_PWM(void);            // configure 2 PWM channels of TIM2
void init_GPIO(void);           // configure GPIO
void init_variable(void);       // initialize all global variables and flags
void init_UART(void);           // configure UART peripheral
void init_TIM1(void);           // configure TIM1 peripheral
void init_IWDG(void);           // configure independent watchdog timer

void printOptions(void);
void enterParameters(void);
void processCommand(void);
uint8_t parseString(char *str);