/*
 * GPIO.h
 *
 *  Created on: Feb 14, 2023
 *      Author: samir
 */

#ifndef GPIO_H_
#define GPIO_H_

/*******************************************Includes*******************************************************/
#include "../../include/LIB/Bit_math.h"
#include "../../include/LIB/Std_types.h"

/*******************************************************************************************************/
/******************************************* Macros ****************************************************/
/******************************************************************************************************/

/***********************************************AF*****************************************************/
/* AF 0 selection */ 
#define GPIO_AF0_RTC_50Hz      (0b0000U)  
#define GPIO_AF0_MCO           (0b0000U)  
#define GPIO_AF0_TAMPER        (0b0000U)  
#define GPIO_AF0_SWJ           (0b0000U)  
#define GPIO_AF0_TRACE         (0b0000U)  

/* AF 1 selection */
#define GPIO_AF1_TIM1          (0b0001U)  
#define GPIO_AF1_TIM2          (0b0001U)  

/* AF 2 selection */
#define GPIO_AF2_TIM3          (0b0010U)  
#define GPIO_AF2_TIM4          (0b0010U)  
#define GPIO_AF2_TIM5          (0b0010U)  

/* AF 3 selection */
#define GPIO_AF3_TIM9          (0b0011U)  
#define GPIO_AF3_TIM10         (0b0011U)  
#define GPIO_AF3_TIM11         (0b0011U)  

/* AF 4 selection */
#define GPIO_AF4_I2C1          (0b0100U) 
#define GPIO_AF4_I2C2          (0b0100U) 
#define GPIO_AF4_I2C3          (0b0100U) 

/* AF 5 selection */
#define GPIO_AF5_SPI1           (0b0101U) 
#define GPIO_AF5_SPI2           (0b0101U) 
#define GPIO_AF5_SPI4           (0b0101U) 
#define GPIO_AF5_I2S3ext        (0b0101U) 

/* AF 6 selection */
#define GPIO_AF6_SPI3          (0b0110U)  
#define GPIO_AF6_I2S2ext       (0b0110U)  

/* AF 7 selection */
#define GPIO_AF7_USART1        (0b0111U)   
#define GPIO_AF7_USART2        (0b0111U)   
#define GPIO_AF7_I2S3ext       (0b0111U)   

/* AF 8 selection */
#define GPIO_AF8_USART6        (0b1000U)   

/* AF 9 selection */
#define GPIO_AF9_TIM14         (0b1001U)   
#define GPIO_AF9_I2C2          (0b1001U)   
#define GPIO_AF9_I2C3          (0b1001U)   

/* AF 10 selection */
#define GPIO_AF10_OTG_FS        (0b1010U)  

/* AF 12 selection */
#define GPIO_AF12_SDIO          (0b1100U) 

/* AF 15 selection */
#define GPIO_AF15_EVENTOUT      (0b1111U)


/***************************************************** VALUE ********************************************/
#define GPIO_HIGH   1
#define GPIO_LOW    2


/**********************************************************************************************************/
/***************************************************END****************************************************/
/**********************************************************************************************************/


/******************************************************************************************************/
/******************************************* Types ****************************************************/
/******************************************************************************************************/


/*********************************************    ENUMS    ********************************************/
/**                                                                                                  **/
/********************************************* ErrorStatus ********************************************/

typedef enum
{
    GPIO_OK,
    GPIO_ERROR,
    GPIO_ERROR_INVALID_PARAM,
    GPIO_ERROR_NOT_SUPPORTED
    
}GPIO_ERROR_STATE_t;

/*********************************************    PINS    ********************************************/

typedef enum
{
    GPIO_PIN0     = 0x10001,   
    GPIO_PIN1     = 0x10002,   
    GPIO_PIN2     = 0x10004,   
    GPIO_PIN3     = 0x10008,   
    GPIO_PIN4     = 0x10010,   
    GPIO_PIN5     = 0x10020,  
    GPIO_PIN6     = 0x10040,
    GPIO_PIN7     = 0x10080, 
    GPIO_PIN8     = 0x10100, 
    GPIO_PIN9     = 0x10200,
    GPIO_PIN10    = 0x10400,
    GPIO_PIN11    = 0x10800,
    GPIO_PIN12    = 0x11000,
    GPIO_PIN13    = 0x12000,
    GPIO_PIN14    = 0x14000,
    GPIO_PIN15    = 0x18000,
    GPIO_PIN_All  = 0xFFFF
}GPIO_PINS_t;

/*********************************************    PORTS    ********************************************/
typedef enum
{
    GPIO_PORTA,
    GPIO_PORTB,
    GPIO_PORTC
}GPIO_PORTS_t;


/*********************************************    MODES    ********************************************/

typedef enum {                /*  M:M/OT/S:S/P:P */
    /*****************intput*******************/
    GPIO_MODE_INPUT_FLOATING    = 0b10000000,
    GPIO_MODE_INPUT_PULLUP      = 0b10000001,
    GPIO_MODE_INPUT_PULLDOWN    = 0b10000010,

    /*****************output*******************/
       /**********LOW SPEED*************/
    GPIO_MODE_OUTPUT_PP_LS      = 0b10100000,
    GPIO_MODE_OUTPUT_PP_PU_LS   = 0b10100001, 
    GPIO_MODE_OUTPUT_PP_PD_LS   = 0b10100010,
    GPIO_MODE_OUTPUT_OD_LS      = 0b10110000,
    GPIO_MODE_OUTPUT_OD_PU_LS   = 0b10110001,
    GPIO_MODE_OUTPUT_OD_PD_LS   = 0b10110010,
       /**********MED SPEED*************/
    GPIO_MODE_OUTPUT_PP_MS      = 0b10100100,
    GPIO_MODE_OUTPUT_PP_PU_MS   = 0b10100101, 
    GPIO_MODE_OUTPUT_PP_PD_MS   = 0b10100110,
    GPIO_MODE_OUTPUT_OD_MS      = 0b10110100,
    GPIO_MODE_OUTPUT_OD_PU_MS   = 0b10110101,
    GPIO_MODE_OUTPUT_OD_PD_MS   = 0b10110110,
       /**********HIGH SPEED*************/
    GPIO_MODE_OUTPUT_PP_HS      = 0b10101100,
    GPIO_MODE_OUTPUT_PP_PU_HS   = 0b10101101, 
    GPIO_MODE_OUTPUT_PP_PD_HS   = 0b10101110,
    GPIO_MODE_OUTPUT_OD_HS      = 0b10111100,
    GPIO_MODE_OUTPUT_OD_PU_HS   = 0b10111101,
    GPIO_MODE_OUTPUT_OD_PD_HS   = 0b10111110,       
   
    /*****************ALT FUNC*******************/
        /**********LOW SPEED*************/

    GPIO_MODE_AF_PP_LS          = 0b11000000,
    GPIO_MODE_AF_PP_PU_LS       = 0b11000001,
    GPIO_MODE_AF_PP_PD_LS       = 0b11000010,  
    GPIO_MODE_AF_OD_LS          = 0b11010000,
    GPIO_MODE_AF_OD_PU_LS       = 0b11010001,
    GPIO_MODE_AF_OD_PD_LS       = 0b11010010,
        /**********MED SPEED*************/
    GPIO_MODE_AF_PP_MS          = 0b11000100,
    GPIO_MODE_AF_PP_PU_MS       = 0b11000101,
    GPIO_MODE_AF_PP_PD_MS       = 0b11000110,  
    GPIO_MODE_AF_OD_MS          = 0b11010100,
    GPIO_MODE_AF_OD_PU_MS       = 0b11010101,
    GPIO_MODE_AF_OD_PD_MS       = 0b11010110,
       
       /**********HIGH SPEED*************/
    GPIO_MODE_AF_PP_HS          = 0b11001100,
    GPIO_MODE_AF_PP_PU_HS       = 0b11001101,
    GPIO_MODE_AF_PP_PD_HS       = 0b11001110,  
    GPIO_MODE_AF_OD_HS          = 0b11011100,
    GPIO_MODE_AF_OD_PU_HS       = 0b11011101,
    GPIO_MODE_AF_OD_PD_HS       = 0b11011110,

    /*****************Analog*******************/
    GPIO_MODE_IO_ANALOG         = 0b11100000
}GPIO_MODES_t;

/*********************************************    Speeds    ********************************************/

typedef enum {
    GPIO_SPEED_LOW    = 0b00,
    GPIO_SPEED_MEDIUM = 0b01,
    GPIO_SPEED_HIGH   = 0b10
}GPIO_SPEEDS_t;

/********************************************** PULL ************************************************/

typedef enum {
    GPIO_PULL_NONE = 0b00,
    GPIO_PULL_UP   = 0b01,
    GPIO_PULL_DOWN = 0b10
}GPIO_PULL_t;

/********************************************** AF ****************************************************/

typedef enum {
    GPIO_AF_0 = 0b0000,
    GPIO_AF_1 ,
    GPIO_AF_2 ,
    GPIO_AF_3 ,
    GPIO_AF_4 ,
    GPIO_AF_5 ,
    GPIO_AF_6 ,
    GPIO_AF_7,
    GPIO_AF_8,
    GPIO_AF_9,
    GPIO_AF_10,
    GPIO_AF_11,
    GPIO_AF_12,
    GPIO_AF_13,
    GPIO_AF_14,
    GPIO_AF_15
}GPIO_AF_t;

/*********************************************   CFG Struct    ***************************************/

typedef struct
{
    GPIO_PORTS_t  GPIO_Port;
    GPIO_PINS_t   GPIO_Pin;
    GPIO_MODES_t  GPIO_Mode;
    GPIO_SPEEDS_t GPIO_Speed;
    GPIO_PULL_t   GPIO_Pull;
    GPIO_AF_t     GPIO_AF;
}GPIO_CFG_t;

/**********************************************************************************************************/
/***************************************************END****************************************************/
/**********************************************************************************************************/



/************************************************ APIs ***************************************************/
/*                                                                                                       */

/***************************************************************************************/
/*                                GPIO_init     function                               */
/***************************************************************************************/
/*  Description: used to initialize GPIO PIN                                           */
/* 	input:       GPIO_PIN_t                                                            */
/* 	return:      GPIO_ERROR_STATE_t( GPIO_OK ,GPIO_ERRRO, GPIO_ERROR_INVALID_PARAM)    */
/***************************************************************************************/

GPIO_ERROR_STATE_t GPIO_init(GPIO_CFG_t* copy_ptcfg);


/***************************************************************************************/
/*                                GPIO_setPinDir     function                          */
/***************************************************************************************/
/*  Description: used to set  GPIO PIN   direction                                     */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t ,       GPIO_MODES_t                    */
/* 	return:      GPIO_ERROR_STATE_t                                                     */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setPinDir(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode);

/***************************************************************************************/
/*                                GPIO_setPinMode     function                         */
/***************************************************************************************/
/*  Description: used to set  GPIO PIN   Mode                                          */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t ,       GPIO_MODES_t                    */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setOutMode(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode);

/***************************************************************************************/
/*                                GPIO_setPinSpeed     function                        */
/***************************************************************************************/
/*  Description: used to set  GPIO PIN   Speed                                         */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t ,       GPIO_SPEEDS_t                   */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setPinSpeed(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode);

/***************************************************************************************/
/*                                GPIO_setPinPull     function                         */
/***************************************************************************************/
/*  Description: used to set  GPIO PIN   Speed                                         */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t ,       GPIO_PULL_t                     */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setPinPull(GPIO_PORTS_t port, GPIO_PINS_t pin,GPIO_MODES_t mode);


/***************************************************************************************/
/*                                GPIO_setPinVal     function                          */
/***************************************************************************************/
/*  Description: used to set  GPIO PIN   Value                                         */
/* 	input:       value(GPIO_LOW,GPIO_HIGH), GPIO_PINS_t ,       GPIO_PORTS_t           */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setPinVal(GPIO_PORTS_t port, GPIO_PINS_t pin);

/***************************************************************************************/
/*                                GPIO_resetPinVal     function                        */
/***************************************************************************************/
/*  Description: used to reset  GPIO PIN   Value                                       */
/* 	input:       value(GPIO_LOW,GPIO_HIGH), GPIO_PINS_t ,       GPIO_PORTS_t           */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_resetPinVal(GPIO_PORTS_t port, GPIO_PINS_t pin);

/***************************************************************************************/
/*                                GPIO_readPinVal     function                          */
/***************************************************************************************/
/*  Description: used to read  GPIO PIN   Value                                        */
/* 	input:        GPIO_PINS_t ,       GPIO_PORTS_t                                     */
/* 	return:      (u32)bit value                                                        */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_readOutPin(GPIO_PORTS_t port, GPIO_PINS_t pin ,u32* value);

/***************************************************************************************/
/*                                GPIO_readPhysVal   function                          */
/***************************************************************************************/
/*  Description: used to read  GPIO PIN   Value                                        */
/* 	input:        GPIO_PINS_t ,       GPIO_PORTS_t                                     */
/* 	return:      (u32)bit value                                                        */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_readPhysVal(GPIO_PORTS_t port, GPIO_PINS_t pin,u32* value);

/***************************************************************************************/
/*                                GPIO_setPortDir     function                          */
/***************************************************************************************/
/*  Description: used to Set  GPIO PORT   Direction                                    */
/* 	input:         GPIO_PORTS_t,       GPIO_MODES_t                                     */
/* 	return:      (u32)bit value                                                        */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setPortDir(GPIO_PORTS_t port, GPIO_MODES_t mode);

/***************************************************************************************/
/*                                GPIO_setPortVal     function                         */
/***************************************************************************************/
/*  Description: used to set  GPIO PIN   Value                                         */
/* 	input:       value(GPIO_LOW,GPIO_HIGH),        GPIO_PORTS_t                        */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_setPortVal(GPIO_PORTS_t port);

/***************************************************************************************/
/*                                GPIO_resetPortVal     function                       */
/***************************************************************************************/
/*  Description: used to reset  GPIO PIN   Value                                       */
/* 	input:       value(GPIO_LOW,GPIO_HIGH),        GPIO_PORTS_t                        */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_resetPortVal(GPIO_PORTS_t port);
/***************************************************************************************/
/*                                GPIO_readPinVal     function                          */
/***************************************************************************************/
/*  Description: used to read  GPIO port   Value                                        */
/* 	input:        GPIO_PORTS_t                                                         */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
 GPIO_ERROR_STATE_t GPIO_readPortVal(GPIO_PORTS_t port,u16* portVal);

/***************************************************************************************/
/*                                GPIO_selectAF     function                          */
/***************************************************************************************/
/*  Description: used to select   alternate function for Pin                           */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t ,       GPIO_MODES_t  ,AFNumber(MACROS) */
/* 	return:      GPIO_ERROR_STATE_t                                                     */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_selectAF(u32  AFNumber, GPIO_PINS_t pin,GPIO_PORTS_t port);


/***************************************************************************************/
/*                                GPIO_lockPIN     function                            */
/***************************************************************************************/
/*  Description: used to LOck  GPIO PIN                                                */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t                                         */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_lockPIN(GPIO_PINS_t pin,GPIO_PORTS_t port);

/***************************************************************************************/
/*                                GPIO_lockPORT     function                           */
/***************************************************************************************/
/*  Description: used to LOck  GPIO PORT                                               */
/* 	input:       GPIO_PORTS_t,     GPIO_PINS_t                                         */
/* 	return:      GPIO_ERROR_STATE_t                                                    */
/***************************************************************************************/
GPIO_ERROR_STATE_t GPIO_lockPORT(GPIO_PORTS_t port);




/**********************************************************************************************************/
/***************************************************END****************************************************/
/**********************************************************************************************************/



#endif /* GPIO_H_ */
