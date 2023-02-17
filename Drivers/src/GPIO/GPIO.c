/*
 * GPIO.c
 *
 *  Created on: Feb 14, 2023
 *      Author: samir
 */


/*********************************************** include file *******************************************/
#include "../../include/LIB/Bit_math.h"
#include "../../include/LIB/Std_types.h""

#include "../../include/MCLA/GPIO/GPIO.h"


/*********************************************** MACROS ************************************************/
#define GET_BIT_NUM(BIT)       ((BIT&-BIT)>>1)

#define PIN_CHECK_MASK    ((u32)0x10000U)
#define PIN_CLR_MASK      ((u32)0xEFFFFU)
#define IS_GPIO(PIN)      (((PIN) & (PIN_CHECK_MASK)) == PIN_CHECK_MASK)

#define IS_PORT(PORT)     (((PORT) == GPIO_PORTA) || \
                           ((PORT) == GPIO_PORTB) || \
                           ((PORT) == GPIO_PORTC))

#define MODE_CHECK_MASK   (0x80U)
#define MODE_CLR_MASK     (0x7FU)
#define IS_MODE(MODE)    (((MODE) & (MODE_CHECK_MASK)) == (MODE_CHECK_MASK))

#define IS_PULL(PULL)      (((PULL) == GPIO_PULL_NONE) || \
                           ((PULL) == GPIO_PULL_UP)    || \
                           ((PULL) == GPIO_PULL_DOWN))

#define IS_SPEED(SPEED)   (((SPEED) ==  GPIO_SPEED_LOW  )     || \
                           ((SPEED) == GPIO_SPEED_MEDIUM)     || \
                           ((SPEED) == GPIO_SPEED_HIGH))

#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF0_RTC_50Hz)   || ((AF) == GPIO_AF9_TIM14)      || ((AF) == GPIO_AF_0) ||\
                          ((AF) == GPIO_AF0_MCO)        || ((AF) == GPIO_AF0_TAMPER)     || ((AF) == GPIO_AF_1) ||\
                          ((AF) == GPIO_AF0_SWJ)        || ((AF) == GPIO_AF0_TRACE)      || ((AF) == GPIO_AF_2) ||\
                          ((AF) == GPIO_AF1_TIM1)       || ((AF) == GPIO_AF1_TIM2)       || ((AF) == GPIO_AF_3) ||\
                          ((AF) == GPIO_AF2_TIM3)       || ((AF) == GPIO_AF2_TIM4)       || ((AF) == GPIO_AF_4) ||\
                          ((AF) == GPIO_AF2_TIM5)       || ((AF) == GPIO_AF4_I2C1)       || ((AF) == GPIO_AF_5) ||\
                          ((AF) == GPIO_AF4_I2C2)       || ((AF) == GPIO_AF4_I2C3)       || ((AF) == GPIO_AF_6) ||\
                          ((AF) == GPIO_AF5_SPI1)       || ((AF) == GPIO_AF5_SPI2)       || ((AF) == GPIO_AF_7) ||\
                          ((AF) == GPIO_AF6_SPI3)       || ((AF) == GPIO_AF5_SPI4)       || ((AF) == GPIO_AF_8) ||\
                          ((AF) == GPIO_AF7_USART1)     || ((AF) == GPIO_AF7_USART2)     || ((AF) == GPIO_AF_9) ||\
                          ((AF) == GPIO_AF8_USART6)     || ((AF) == GPIO_AF10_OTG_FS)    || ((AF) == GPIO_AF_10)||\
                          ((AF) == GPIO_AF9_I2C2)       || ((AF) == GPIO_AF9_I2C3)       || ((AF) == GPIO_AF_12)||\
                          ((AF) == GPIO_AF12_SDIO)      || ((AF) == GPIO_AF15_EVENTOUT)  || ((AF) == GPIO_AF_15))
#define IS_GPIO_VALUE(VAL)    ( (VAL == GPIO_HIGH) || (VAL == GPIO_LOW))
#define CLR_DOUBLE_BITS                 0x3
#define CLR_QUAD_BITS                   ((u32)0xFU)

#define PUPD_MASK                       0x3
#define OSPEEDR_MASK                    0xC
#define OTYPER_MASK                     0x10
#define MODER_MASK                      0x60

#define GET_PUPDR_BITS(MASK)            (MASK & PUPD_MASK)
#define GET_OSPEEDR_BITS(MASK)          ((MASK & OSPEEDR_MASK)>>2)
#define GET_OTYPER_BITS(MASK)           ((MASK & OTYPER_MASK) >>4)
#define GET_MODER_BITS(MASK)            ((MASK & MODER_MASK)  >>5)

#define MODE_COUNT(MODE,PIN)            ((u32)MODE<<PIN*2)
#define SPEED_COUNT(SPEED,PIN)          ((u32)SPEED<<PIN*2)
#define PULL_COUNT(PULL,PIN)            ((u32)PULL<<PIN*2)

#define  AF_LOW_REG                        0
#define  AF_HIGH_REG                       1

#define  GPIO_PORT_MASK                   ((u32)0xFFFFFFFFU)
#define GPIO_PORT_OP_MASK                 ((u32)0x55555555U)

#define PORT_SIZE                         16
#define BIT_MASK_QUAD_SHIFT               4
#define BIT_MASK_SHIFT                    2
#define NUMBER_OF_LOW_AF                  7

#define PORT_SET_RESET                   ((u32)0x0000FFFFU)

#define GPIO_INPUT                       0b00
#define GPIO_OUTPUT                      0b01
#define GPIO_ALTF                        0b10
#define GPIO_ANALOG                      0b11

#define GPIO_LCK_PORT_PIN                  16
/******************************************** Types definitions **********************************************/

/*********************************************    REG    ********************************************/
typedef struct{

	volatile u32 GPIO_MODER   ;
	volatile u32 GPIO_OTYPER  ;
	volatile u32 GPIO_OSPEEDR ;
	volatile u32 GPIO_PUPDR   ;
	volatile u32 GPIO_IDR     ;
	volatile u32 GPIO_ODR     ;
	volatile u32 GPIO_BSRR    ;
	volatile u32 GPIO_LCKR    ;
	volatile u32 GPIO_AFR[2]  ;
    
}GPIO;

#define GPIOA ((volatile GPIO*)0x40020000)
#define GPIOB ((volatile GPIO*)0x40020400)
#define GPIOC ((volatile GPIO*)0x40020800)

/********************************************** variables ************************************************/





/********************************************** Static Function Prototypes ********************************/
static u32 GetPinNumber(u32 mask);
static GPIO_ERROR_STATE_t Wraper_MODER (GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode);
static GPIO_ERROR_STATE_t Wraper_OTYPER(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode);
static GPIO_ERROR_STATE_t Wraper_OSPEEDR(GPIO_PORTS_t port, GPIO_PINS_t pin, GPIO_MODES_t mode);
static GPIO_ERROR_STATE_t Wraper_PUPDR(GPIO_PORTS_t port, GPIO_PINS_t pin, GPIO_MODES_t mode);
static GPIO_ERROR_STATE_t Wraper_SlectAF(u32 AF, GPIO_PINS_t pin,GPIO_PORTS_t port);

/********************************************** End Static Function ***************************************/



/********************************************** implementation of APIs *************************************/

GPIO_ERROR_STATE_t GPIO_init(GPIO_CFG_t* copy_ptcfg)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 tmp_Buffer;
    if(IS_MODE(copy_ptcfg->GPIO_Mode)   == 0 )error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_PULL(copy_ptcfg->GPIO_Pull)   == 0 )error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_PORT(copy_ptcfg->GPIO_Port)   == 0 )error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(copy_ptcfg->GPIO_Pin)    == 0 )error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_SPEED(copy_ptcfg->GPIO_Speed) == 0 )error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO_AF(copy_ptcfg->GPIO_AF)  == 0 )error_state = GPIO_ERROR_INVALID_PARAM;
    
    switch(copy_ptcfg->GPIO_Port)
    {
        case GPIO_PORTA:
            error_state = Wraper_MODER(GPIO_PORTA, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);
        
            error_state = Wraper_OTYPER(GPIO_PORTA, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);
        
            error_state = Wraper_OSPEEDR(GPIO_PORTA, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);
        
            error_state = Wraper_PUPDR(GPIO_PORTA, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);

            error_state = Wraper_SlectAF(copy_ptcfg->GPIO_AF, copy_ptcfg->GPIO_Pin,GPIO_PORTA);
            break;

        case GPIO_PORTB:

            error_state = Wraper_MODER(GPIO_PORTB, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);
            
            error_state = Wraper_OTYPER(GPIO_PORTB, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);


            error_state = Wraper_OSPEEDR(GPIO_PORTB, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);


            error_state = Wraper_PUPDR(GPIO_PORTB, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);
            
            error_state = Wraper_SlectAF(copy_ptcfg->GPIO_AF, copy_ptcfg->GPIO_Pin,GPIO_PORTB);

            
            break;

        case GPIO_PORTC:

            error_state = Wraper_MODER(GPIO_PORTC, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);


            error_state = Wraper_OTYPER(GPIO_PORTC, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);


            error_state = Wraper_OSPEEDR(GPIO_PORTC, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);

            error_state = Wraper_PUPDR(GPIO_PORTC, copy_ptcfg->GPIO_Pin,copy_ptcfg->GPIO_Mode);
            
            error_state = Wraper_SlectAF(copy_ptcfg->GPIO_AF, copy_ptcfg->GPIO_Pin,GPIO_PORTC);

            break;
       
        default :
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;

    } 


    return error_state;
}



GPIO_ERROR_STATE_t GPIO_setPinDir(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;  
    u32 LOC_tmpBuffer,LOC_pinnumber; 
    if(IS_PORT(port) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0)error_state  = GPIO_ERROR_INVALID_PARAM;
    if(IS_MODE(mode) == 0)error_state = GPIO_ERROR_INVALID_PARAM; 
    switch(port) 
    {
        case GPIO_PORTA:
            LOC_tmpBuffer = GPIOA->GPIO_MODER;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS<<(LOC_pinnumber*BIT_MASK_SHIFT));
            LOC_tmpBuffer |=  (GET_MODER_BITS(mode)<<(LOC_pinnumber*BIT_MASK_SHIFT));
            GPIOA->GPIO_MODER = LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
            LOC_tmpBuffer = GPIOB->GPIO_MODER;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS<<(LOC_pinnumber*BIT_MASK_SHIFT));
            LOC_tmpBuffer |=  (GET_MODER_BITS(mode)<<(LOC_pinnumber*BIT_MASK_SHIFT));
            GPIOB->GPIO_MODER = LOC_tmpBuffer;
            break;
        case GPIO_PORTC:
            LOC_tmpBuffer = GPIOC->GPIO_MODER;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS<<(LOC_pinnumber*BIT_MASK_SHIFT));
            LOC_tmpBuffer |=  (GET_MODER_BITS(mode)<<(LOC_pinnumber*BIT_MASK_SHIFT));
            GPIOC->GPIO_MODER = LOC_tmpBuffer;
            break;
        default: 
            error_state = GPIO_ERROR_INVALID_PARAM; 
     
    }
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_setOutMode(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;  
    u32 LOC_tmpBuffer,LOC_pinnumber; 
    if(IS_PORT(port) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0)error_state  = GPIO_ERROR_INVALID_PARAM;
    if(IS_MODE(mode) == 0)error_state = GPIO_ERROR_INVALID_PARAM; 
    switch(port) 
    {
        case GPIO_PORTA:
            LOC_tmpBuffer = GPIOA->GPIO_OTYPER;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber));
            LOC_tmpBuffer |=  (GET_OTYPER_BITS(mode)<<(LOC_pinnumber));
            GPIOA->GPIO_OTYPER = LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
            LOC_tmpBuffer = GPIOB->GPIO_OTYPER;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber));
            LOC_tmpBuffer |=  (GET_OTYPER_BITS(mode)<<(LOC_pinnumber));
            GPIOB->GPIO_OTYPER = LOC_tmpBuffer;
            break;
        case GPIO_PORTC:
            LOC_tmpBuffer = GPIOC->GPIO_OTYPER;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber));
            LOC_tmpBuffer |=  (GET_OTYPER_BITS(mode)<<(LOC_pinnumber));
            GPIOC->GPIO_OTYPER = LOC_tmpBuffer;
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
        break;

    }
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_setPinSpeed(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;  
    u32 LOC_tmpBuffer,LOC_pinnumber; 
    if(IS_PORT(port) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0)error_state  = GPIO_ERROR_INVALID_PARAM;
    if(IS_SPEED(mode) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    switch(port)
    {
        case GPIO_PORTA:
            LOC_tmpBuffer = GPIOA->GPIO_OSPEEDR;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber * BIT_MASK_SHIFT ));
            LOC_tmpBuffer |=  (GET_OSPEEDR_BITS(mode)<<(LOC_pinnumber * BIT_MASK_SHIFT));
            GPIOA->GPIO_OSPEEDR = LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
            LOC_tmpBuffer = GPIOB->GPIO_OSPEEDR;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber * BIT_MASK_SHIFT ));
            LOC_tmpBuffer |=  (GET_OSPEEDR_BITS(mode)<<(LOC_pinnumber * BIT_MASK_SHIFT));
            GPIOB->GPIO_OSPEEDR = LOC_tmpBuffer;
            break;
        case GPIO_PORTC:
            LOC_tmpBuffer = GPIOC->GPIO_OSPEEDR;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber * BIT_MASK_SHIFT ));
            LOC_tmpBuffer |=  (GET_OSPEEDR_BITS(mode)<<(LOC_pinnumber * BIT_MASK_SHIFT));
            GPIOC->GPIO_OSPEEDR = LOC_tmpBuffer;
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;
    }
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_setPinPull(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;  
    u32 LOC_tmpBuffer,LOC_pinnumber; 
    if(IS_PORT(port) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0)error_state  = GPIO_ERROR_INVALID_PARAM;
    if(IS_PULL(mode) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    switch(port)
    {
        case GPIO_PORTA:
            LOC_tmpBuffer = GPIOA->GPIO_PUPDR;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber * BIT_MASK_SHIFT ));
            LOC_tmpBuffer |=  (GET_PUPDR_BITS(mode)<<(LOC_pinnumber * BIT_MASK_SHIFT));
            GPIOA->GPIO_PUPDR = LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
            LOC_tmpBuffer = GPIOB->GPIO_PUPDR;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber * BIT_MASK_SHIFT ));
            LOC_tmpBuffer |=  (GET_PUPDR_BITS(mode)<<(LOC_pinnumber * BIT_MASK_SHIFT));
            GPIOB->GPIO_PUPDR = LOC_tmpBuffer;
            break;
        case GPIO_PORTC:
            LOC_tmpBuffer = GPIOC->GPIO_PUPDR;
            LOC_pinnumber = GetPinNumber(pin);
            LOC_tmpBuffer &= ~(CLR_DOUBLE_BITS << (LOC_pinnumber * BIT_MASK_SHIFT ));
            LOC_tmpBuffer |=  (GET_PUPDR_BITS(mode)<<(LOC_pinnumber * BIT_MASK_SHIFT));
            GPIOC->GPIO_PUPDR = LOC_tmpBuffer;
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;

    }
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_selectAF(u32  AFNumber, GPIO_PINS_t pin,GPIO_PORTS_t port)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;  
    u32 tmpBuffer,LOC_pinnumber;
    if(IS_PORT(port) == 0)error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO_AF(AFNumber) == 0)error_state  = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0)error_state  = GPIO_ERROR_INVALID_PARAM;
    switch(port)
    {
        case GPIO_PORTA:
            LOC_pinnumber = GetPinNumber(pin);
            if(LOC_pinnumber <= NUMBER_OF_LOW_AF )
            {
                tmpBuffer = GPIOA->GPIO_AFR[AF_LOW_REG];
                tmpBuffer &= ~(CLR_QUAD_BITS <<    (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                tmpBuffer |=  (AFNumber << (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                GPIOA->GPIO_AFR[AF_LOW_REG] = tmpBuffer;

            }
            else if(LOC_pinnumber > NUMBER_OF_LOW_AF)
            {
                LOC_pinnumber %= NUMBER_OF_LOW_AF + 1;
                tmpBuffer = GPIOA->GPIO_AFR[AF_HIGH_REG];
                tmpBuffer &= ~(CLR_QUAD_BITS <<    (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                tmpBuffer |=  (AFNumber << (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                GPIOA->GPIO_AFR[AF_HIGH_REG] = tmpBuffer;
            }
            else
            {
                error_state = GPIO_ERROR_INVALID_PARAM;
            }
            break;
        case GPIO_PORTB:
            LOC_pinnumber = GetPinNumber(pin);
            if(LOC_pinnumber <= NUMBER_OF_LOW_AF )
            {
                tmpBuffer = GPIOB->GPIO_AFR[AF_LOW_REG];
                tmpBuffer &= ~(CLR_QUAD_BITS <<    (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                tmpBuffer |=  (AFNumber << (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                GPIOB->GPIO_AFR[AF_LOW_REG] = tmpBuffer;
            }
            else if(LOC_pinnumber > NUMBER_OF_LOW_AF)
            {
                tmpBuffer = GPIOB->GPIO_AFR[AF_HIGH_REG];
                tmpBuffer &= ~(CLR_QUAD_BITS <<    (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                tmpBuffer |=  (AFNumber << (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                GPIOB->GPIO_AFR[AF_HIGH_REG] = tmpBuffer;
            }
            else
            {
                error_state = GPIO_ERROR_INVALID_PARAM;
            }
            break;
            
        case GPIO_PORTC:
            LOC_pinnumber = GetPinNumber(pin);
            if(LOC_pinnumber <= NUMBER_OF_LOW_AF )
            {
                tmpBuffer = GPIOC->GPIO_AFR[AF_LOW_REG];
                tmpBuffer &= ~(CLR_QUAD_BITS <<    (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                tmpBuffer |=  (AFNumber << (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                GPIOC->GPIO_AFR[AF_LOW_REG] = tmpBuffer;
            }
            else if(LOC_pinnumber > NUMBER_OF_LOW_AF)
            {
                tmpBuffer = GPIOC->GPIO_AFR[AF_HIGH_REG];
                tmpBuffer &= ~(CLR_QUAD_BITS <<    (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                tmpBuffer |=  (AFNumber << (LOC_pinnumber * BIT_MASK_QUAD_SHIFT ));
                GPIOC->GPIO_AFR[AF_HIGH_REG] = tmpBuffer;
            }
            else
            {
                error_state = GPIO_ERROR_INVALID_PARAM;
            }
            break;
            
    }

    return error_state;
}

GPIO_ERROR_STATE_t GPIO_setPinVal(GPIO_PORTS_t port, GPIO_PINS_t pin)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 tmpBuffer;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            tmpBuffer = GPIOA->GPIO_BSRR;
            pin &= PIN_CLR_MASK;
            tmpBuffer |= pin ;
            GPIOA->GPIO_BSRR = tmpBuffer;
            break;
        case GPIO_PORTB:
            tmpBuffer = GPIOB->GPIO_BSRR;
            pin &= PIN_CLR_MASK;
            tmpBuffer |= pin ;
            GPIOB->GPIO_BSRR = tmpBuffer;
            break;
        case GPIO_PORTC:
            tmpBuffer = GPIOC->GPIO_BSRR;
            pin &= PIN_CLR_MASK;
            tmpBuffer |= pin ;
            GPIOC->GPIO_BSRR = tmpBuffer;
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
    }  
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_resetPinVal(GPIO_PORTS_t port, GPIO_PINS_t pin)
{
     GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 tmpBuffer;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            tmpBuffer = GPIOA->GPIO_BSRR;
            pin &= PIN_CLR_MASK;
            tmpBuffer |= (pin<<PORT_SIZE) ;
            GPIOA->GPIO_BSRR = tmpBuffer;
            break;
        case GPIO_PORTB:
            tmpBuffer = GPIOB->GPIO_BSRR;
            pin &= PIN_CLR_MASK;
            tmpBuffer |= (pin<<PORT_SIZE) ;
            GPIOB->GPIO_BSRR = tmpBuffer;
            break;
        case GPIO_PORTC:
            tmpBuffer = GPIOC->GPIO_BSRR;
            pin &= PIN_CLR_MASK;
            tmpBuffer |= (pin<<PORT_SIZE) ;
            GPIOC->GPIO_BSRR = tmpBuffer;
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
    }  
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_readOutPin(GPIO_PORTS_t port, GPIO_PINS_t pin ,u32* value)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 pin_Number ;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_GPIO(pin) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {

        case GPIO_PORTA:
            pin_Number = GetPinNumber(pin);
            *value = GET_BIT(GPIOA->GPIO_ODR,pin_Number);
            break;
        case GPIO_PORTB:
            pin_Number = GetPinNumber(pin);
            *value = GET_BIT(GPIOB->GPIO_ODR,pin_Number);
            break;
        case GPIO_PORTC:
            pin_Number = GetPinNumber(pin);
            *value = GET_BIT(GPIOC->GPIO_ODR,pin_Number);
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
    }
    return error_state;
}

GPIO_ERROR_STATE_t GPIO_readPhysVal(GPIO_PORTS_t port, GPIO_PINS_t pin,u32* value) 
{
    u32 pin_Number ;
    GPIO_ERROR_STATE_t LOC_ERROR_STATE =GPIO_OK;
    if(IS_PORT(port) == 0) return 0;
    if(IS_GPIO(pin) == 0) return 0;
    switch (port)
    {
        case GPIO_PORTA:
            pin_Number = GetPinNumber(pin);
            *value  = GET_BIT(GPIOA->GPIO_IDR,pin_Number);
            break;
        case GPIO_PORTB:
            pin_Number = GetPinNumber(pin);
            *value  = GET_BIT(GPIOA->GPIO_IDR,pin_Number);
            break;
        case GPIO_PORTC:
            pin_Number = GetPinNumber(pin);
            *value  = GET_BIT(GPIOA->GPIO_IDR,pin_Number);
            break;
        default:
            LOC_ERROR_STATE  = GPIO_ERROR_INVALID_PARAM ;
    }
    return LOC_ERROR_STATE;
}

GPIO_ERROR_STATE_t GPIO_setPortDir(GPIO_PORTS_t port, GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 LOC_tmpBuffer;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_MODE(mode) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            LOC_tmpBuffer = GPIOA->GPIO_MODER;
            LOC_tmpBuffer &= ~(GPIO_PORT_MASK);
            if(GET_MODER_BITS(mode) == GPIO_INPUT)
            {
                LOC_tmpBuffer |= ~(GPIO_PORT_MASK);
            }
            else if (GET_MODER_BITS(mode) == GPIO_OUTPUT)
            {
                LOC_tmpBuffer |= (GPIO_PORT_OP_MASK);
            }
            else if (GET_MODER_BITS(mode) == GPIO_ALTF)
            {
                LOC_tmpBuffer |= ~(GPIO_PORT_OP_MASK);
            }
            else
            {
                LOC_tmpBuffer |= (GPIO_PORT_MASK);
            }
            GPIOA->GPIO_MODER |= LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
            LOC_tmpBuffer = GPIOB->GPIO_MODER;
            LOC_tmpBuffer &= ~(GPIO_PORT_MASK);
            if(GET_MODER_BITS(mode) == GPIO_INPUT)
            {
                LOC_tmpBuffer |= ~(GPIO_PORT_MASK);
            }
            else if (GET_MODER_BITS(mode) == GPIO_OUTPUT)
            {
                LOC_tmpBuffer |= (GPIO_PORT_OP_MASK);
            }
            else if (GET_MODER_BITS(mode) == GPIO_ALTF)
            {
                LOC_tmpBuffer |= ~(GPIO_PORT_OP_MASK);
            }
            else
            {
                LOC_tmpBuffer |= (GPIO_PORT_MASK);
            }
            GPIOB->GPIO_MODER |= LOC_tmpBuffer;              
            break;
        case GPIO_PORTC:
            LOC_tmpBuffer = GPIOC->GPIO_MODER;
            LOC_tmpBuffer &= ~(GPIO_PORT_MASK);
            if(GET_MODER_BITS(mode) == GPIO_INPUT)
            {
                LOC_tmpBuffer |= ~(GPIO_PORT_MASK);
            }
            else if (GET_MODER_BITS(mode) == GPIO_OUTPUT)
            {
                LOC_tmpBuffer |= (GPIO_PORT_OP_MASK);
            }
            else if (GET_MODER_BITS(mode) == GPIO_ALTF)
            {
                LOC_tmpBuffer |= ~(GPIO_PORT_OP_MASK);
            }
            else
            {
                LOC_tmpBuffer |= (GPIO_PORT_MASK);
            }
            GPIOC->GPIO_MODER |= LOC_tmpBuffer;
            break;
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
    }
    return error_state;
}

GPIO_ERROR_STATE_t GPIO_setPortVal(GPIO_PORTS_t port)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            GPIOA->GPIO_BSRR = PORT_SET_RESET;
            break;
        case GPIO_PORTB:
            GPIOB->GPIO_BSRR = PORT_SET_RESET;
            break;
        case GPIO_PORTC:
            GPIOC->GPIO_BSRR = PORT_SET_RESET;
            break;
        
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;
    }
    return error_state;
}


GPIO_ERROR_STATE_t GPIO_resetPortVal(GPIO_PORTS_t port)
{
       GPIO_ERROR_STATE_t error_state = GPIO_OK;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            GPIOA->GPIO_BSRR = (PORT_SET_RESET << PORT_SIZE);
            break;
        case GPIO_PORTB:
            GPIOB->GPIO_BSRR = (PORT_SET_RESET << PORT_SIZE);
            break;
        case GPIO_PORTC:
            GPIOC->GPIO_BSRR = (PORT_SET_RESET << PORT_SIZE);
            break;
        
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;
    }
    return error_state;
}


 GPIO_ERROR_STATE_t GPIO_readPortVal(GPIO_PORTS_t port,u16* portVal)
 {
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            *portVal = GPIOA->GPIO_IDR;
            break;
        case GPIO_PORTB:
            *portVal = GPIOB->GPIO_IDR;
            break;
        case GPIO_PORTC:
            *portVal = GPIOC->GPIO_IDR;
            break;
        
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;
    }
    return error_state;
 }   


GPIO_ERROR_STATE_t GPIO_lockPIN(GPIO_PINS_t pin,GPIO_PORTS_t port)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 LOC_tmpBuffer ,pinNumber;
    if(IS_PIN(pin) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            pinNumber = GetPinNumber(pin);
            LOC_tmpBuffer = (1<< pinNumber);
            GPIOA->GPIO_LCKR = LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
             pinNumber = GetPinNumber(pin);
            LOC_tmpBuffer = (1<< pinNumber);
            GPIOB->GPIO_LCKR = LOC_tmpBuffer;
            break;
        case GPIO_PORTC:
            pinNumber = GetPinNumber(pin);
            LOC_tmpBuffer = (1<< pinNumber);
            GPIOC->GPIO_LCKR = LOC_tmpBuffer;       
             break;
        
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;
    }
    return error_state;

}
 

GPIO_ERROR_STATE_t GPIO_lockPORT(GPIO_PORTS_t port)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    u32 LOC_tmpBuffer;
    if(IS_PORT(port) == 0) error_state = GPIO_ERROR_INVALID_PARAM;
    switch (port)
    {
        case GPIO_PORTA:
            LOC_tmpBuffer = (1 << GPIO_LCK_PORT_PIN);
            GPIOA->GPIO_LCKR = LOC_tmpBuffer;
            break;
        case GPIO_PORTB:
            LOC_tmpBuffer = (1 << GPIO_LCK_PORT_PIN);
            GPIOB->GPIO_LCKR = LOC_tmpBuffer;           
            break;
        case GPIO_PORTC:
            LOC_tmpBuffer = (1 << GPIO_LCK_PORT_PIN);
            GPIOC->GPIO_LCKR = LOC_tmpBuffer;       
            break;
        
        default:
            error_state = GPIO_ERROR_INVALID_PARAM;
            break;
    }
    return error_state;
}
/********************************************** End Of APIs ***********************************************/



/********************************************** Static Function implementation ****************************/


static u32 GetPinNumber(u32 mask)
{
    u32 count = 0,loopCount = 0;
    while (GET_BIT(mask,loopCount)!=1)
    {
        count++;
        loopCount++;
    }
    return count;

}


static GPIO_ERROR_STATE_t Wraper_MODER(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    error_state = GPIO_setPinDir(port, pin, mode);
    return error_state;
}


static GPIO_ERROR_STATE_t Wraper_OTYPER(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    error_state = GPIO_setOutMode(port, pin, mode);
    return error_state;
}

static GPIO_ERROR_STATE_t Wraper_OSPEEDR(GPIO_PORTS_t port, GPIO_PINS_t pin, GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    error_state = GPIO_setPinSpeed(port, pin, mode);
    return error_state;
}

static GPIO_ERROR_STATE_t Wraper_PUPDR(GPIO_PORTS_t port, GPIO_PINS_t pin, GPIO_MODES_t mode)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    error_state = GPIO_setPinPull(port, pin, mode);
    return error_state;
}

static GPIO_ERROR_STATE_t Wraper_SlectAF(u32 AF, GPIO_PINS_t pin,GPIO_PORTS_t port)
{
    GPIO_ERROR_STATE_t error_state = GPIO_OK;
    error_state = GPIO_selectAF(AF, pin, port);
    return error_state;
}

/********************************************** End Static Function implementation*************************/

