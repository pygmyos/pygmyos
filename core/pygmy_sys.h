/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#pragma once

#define NULL                                (void*)0
#define TRUE                                1
#define FALSE                               0

typedef signed long long                    s64;
typedef signed long                         s32;
typedef signed short                        s16;
typedef signed char                         s8;

typedef signed long long const              sc64;
typedef signed long  const                  sc32;  
typedef signed short const                  sc16;  
typedef signed char  const                  sc8;   

typedef volatile signed long long           vs64;
typedef volatile signed long                vs32;
typedef volatile signed short               vs16;
typedef volatile signed char                vs8;

typedef volatile signed long long const     vsc64;
typedef volatile signed long  const         vsc32; 
typedef volatile signed short const         vsc16;  
typedef volatile signed char  const         vsc8;  

typedef unsigned long long                  u64;
typedef unsigned long                       u32;
typedef unsigned short                      u16;
typedef unsigned char                       u8;

typedef unsigned long long const            uc64;
typedef unsigned long  const                uc32;  
typedef unsigned short const                uc16;  
typedef unsigned char  const                uc8;   

typedef volatile unsigned long long         vu64;
typedef volatile unsigned long              vu32;
typedef volatile unsigned short             vu16;
typedef volatile unsigned char              vu8;

typedef volatile unsigned long long const   vuc64;
typedef volatile unsigned long  const       vuc32;  
typedef volatile unsigned short const       vuc16;  
typedef volatile unsigned char  const       vuc8;   

#define LITTLEENDIAN                        0
#define BIGENDIAN                           1

#define U8_MAX                              ((u8)255)
#define S8_MAX                              ((s8)127)
#define S8_MIN                              ((s8)-128)
#define U16_MAX                             ((u16)65535u)
#define S16_MAX                             ((s16)32767)
#define S16_MIN                             ((s16)-32768)
#define U32_MAX                             ((u32)4294967295uL)
#define S32_MAX                             ((s32)2147483647)
#define S32_MIN                             ((s32)2147483648uL)

#define BIT0                                0x00000001
#define BIT1                                0x00000002
#define BIT2                                0x00000004
#define BIT3                                0x00000008
#define BIT4                                0x00000010
#define BIT5                                0x00000020
#define BIT6                                0x00000040
#define BIT7                                0x00000080
#define BIT8                                0x00000100
#define BIT9                                0x00000200
#define BIT10                               0x00000400
#define BIT11                               0x00000800
#define BIT12                               0x00001000
#define BIT13                               0x00002000
#define BIT14                               0x00004000
#define BIT15                               0x00008000
#define BIT16                               0x00010000
#define BIT17                               0x00020000
#define BIT18                               0x00040000
#define BIT19                               0x00080000
#define BIT20                               0x00100000
#define BIT21                               0x00200000
#define BIT22                               0x00400000
#define BIT23                               0x00800000
#define BIT24                               0x01000000
#define BIT25                               0x02000000
#define BIT26                               0x04000000
#define BIT27                               0x08000000
#define BIT28                               0x10000000
#define BIT29                               0x20000000
#define BIT30                               0x40000000
#define BIT31                               0x80000000


#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BITNUMBER           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))
#define PLLON_BITNUMBER           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))
#define CSSON_BITNUMBER           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)
#define USBPRE_BITNUMBER          0x16
#define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BITNUMBER           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))
#define BDRST_BITNUMBER           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BITNUMBER           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

// ---------------------- RCC registers bit mask ------------------------ 
#define CR_HSEBYP_RESET           ((u32)0xFFFBFFFF)
#define CR_HSEBYP_SET             ((u32)0x00040000)
#define CR_HSEON_RESET            ((u32)0xFFFEFFFF)
#define CR_HSEON_SET              ((u32)0x00010000)
#define CR_HSITRIM_MASK           ((u32)0xFFFFFF07)

#define CFGR_PLL_MASK             ((u32)0xFFC0FFFF)
#define CFGR_PLLMull_MASK         ((u32)0x003C0000)
#define CFGR_PLLSRC_MASK          ((u32)0x00010000)
#define CFGR_PLLXTPRE_MASK        ((u32)0x00020000)
#define CFGR_SWS_MASK             ((u32)0x0000000C)
#define CFGR_SW_MASK              ((u32)0xFFFFFFFC)
#define CFGR_HPRE_RESETMASK       ((u32)0xFFFFFF0F)
#define CFGR_HPRE_SETMASK         ((u32)0x000000F0)
#define CFGR_PPRE1_RESETMASK      ((u32)0xFFFFF8FF)
#define CFGR_PPRE1_SETMASK        ((u32)0x00000700)
#define CFGR_PPRE2_RESETMASK      ((u32)0xFFFFC7FF)
#define CFGR_PPRE2_SETMASK        ((u32)0x00003800)
#define CFGR_ADCPRE_RESETMASK     ((u32)0xFFFF3FFF)
#define CFGR_ADCPRE_SETMASK       ((u32)0x0000C000)

#define CSR_RMVF_SET              ((u32)0x01000000) //// CSR register bit mask 
#define FLAG_MASK                 ((u8)0x1F) // RCC Flag Mask 
#define BDCR_BASE                 (PERIPH_BASE + BDCR_OFFSET) // BDCR register base address 

#define HSEStartUp_TimeOut        ((u8)0xFF)

//------------------------------------------------------------------------------
//-----------------------------System Specific Types----------------------------
#define PYGMY_MAXTASKS          15
#define PYGMY_MAXMESSAGES       20
#define PYGMY_AT_MAXCOMMANDS    20
#define PYGMY_MAXCOMMANDS       20
#define PYGMY_NOMESSAGE         0xFFFF
#define PYGMY_BLANKID           0x0000
#define PYGMY_SYSTEMID          0xF000
#define PYGMY_UNMARKEDID        0xF001
#define PYGMY_TEMPID            0x2000
#define PYGMY_SERVICEDID        0x1000
#define PYGMY_TASK_FAIL         0x0000
#define PYGMY_TASK_SUCCESS      0x0001
#define PYGMY_TASK_EXISTS       0x0002


// End Thread ID Defines

#define PYGMY_SECONDS_AVERAGEYEAR       31556952
#define PYGMY_SECONDS_LEAPYEAR          31622400
#define PYGMY_SECONDS_COMMONYEAR        31536000     
#define PYGMY_SECONDS_AVERAGEMONTH      2628003
#define PYGMY_SECONDS_WEEK              4492800
#define PYGMY_SECONDS_DAY               86400
#define PYGMY_SECONDS_HOUR              3600
#define PYGMY_SECONDS_MINUTE            60

#define PYGMY_CLOCK_MAIN            (u32)  24000000
#define PYGMY_FIFO_BUFFERLEN        256
#define PYGMY_SYS_MAXPIPES          7

#define PYGMY_SYS_BUSY              BIT0
//#define PYGMY_SYS_LCDWRITE          BIT1
//#define PYGMY_SYS_FILEWRITE         BIT2

#define PYGMY_COMMAND_MAXEXPIRE          30
#define PYGMY_COMMAND_QUEUED             BIT0
#define PYGMY_COMMAND_TX                 BIT1
#define PYGMY_COMMAND_RX                 BIT2
#define PYGMY_COMMAND_OK                 BIT3
#define PYGMY_COMMAND_ERROR              BIT4
#define PYGMY_COMMAND_NOCARRIER          BIT5
#define PYGMY_COMMAND_CONNECT            BIT6
#define PYGMY_COMMAND_RETRY              BIT7

#define PYGMY_AT_MAXEXPIRE          30
#define PYGMY_AT_QUEUED             BIT0
#define PYGMY_AT_TX                 BIT1
#define PYGMY_AT_RX                 BIT2
#define PYGMY_AT_OK                 BIT3
#define PYGMY_AT_ERROR              BIT4
#define PYGMY_AT_NOCARRIER          BIT5
#define PYGMY_AT_CONNECT            BIT6
#define PYGMY_AT_RETRY              BIT7
#define PYGMY_USART1_BUFFERLEN      512

//-------------------------------------------------------------------------------
//------------------------------Pygmy OS Datatypes-------------------------------

typedef u8 ( *PYGMYFUNCPTR )( void );
typedef void ( *PYGMYVOIDPTR )( void );
typedef u8 ( *PYGMYCMDPTR )( u8 *ucParams );

typedef struct {
                u16 Year;
                u8 Month;
                u8 Day;
                u8 Hour;
                u8 Minute;
                u8 Second;
                //s8 UMT;
                } PYGMYTIME;

typedef struct {
                u8 *Name;
                PYGMYCMDPTR Call;
                } PYGMYCMD; 

typedef struct {
                u8 Status;
                u8 Retry;
                u8 *Name;
                PYGMYVOIDPTR PrintHandler;
                PYGMYCMDPTR EventHandler;
                //PYGMYVOIDPTR ErrorHandler;
                u32 TimeStamp;
                u32 Expire;
                } PYGMYCOMMAND;

typedef struct {
                u8 Index;
                u8 Count;
                PYGMYCOMMAND Commands[ PYGMY_MAXCOMMANDS ];
                } PYGMYCOMMANDQUEUE;
            
typedef struct {
                u16 DestID;
                //u16 SenderID;
                u8 *DestName;
                //u8 *SenderName;
                u8 *Message;
                u32 Value;
                u32 TimeStamp;
                //u32 Expire;
                } PYGMYMESSAGE;

typedef struct {
                u8 *Name;
                u8 Busy;
                u16 ID;
                u32 Timer;
                u32 Reload;
                PYGMYFUNCPTR EventHandler;
                u32 TimeStamp;
                u32 Expire;
                } PYGMYTASK;            
            
typedef struct {
                //u8 Status;
                u16 RXBufferLen;
                u16 RXIndex;
                //u16 RXInIndex;
                u16 RXLen;
                u16 TXBufferLen;
                u16 TXIndex;
                //u16 TXInIndex;
                u16 TXLen;
                PYGMYCMDPTR Put;
                PYGMYVOIDPTR Get;
                u8 *RXBuffer;
                u8 *TXBuffer;
                } PYGMYFIFO;    

enum {  TIMER0, TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7, TIMER8, 
        TIMER9, TIMER10, TIMER11, TIMER12, TIMER13, TIMER14, TIMER15, TIMER16, TIMER17
        };
            
typedef struct {
                u32 MainClock;
                u32 XTAL;
                u32 StopWatch;
                u32 Status;
                u16 MCUID;
                u16 CursorX;
                u16 CursorY;
                u8 CursorState;
                u8 DelayTimer;
                u8 PWMTimer;
                //u8 STDIO;
                //u8 ATIndex;
                //u8 ATCount;
                
                //PYGMYATCOMMAND ATCommands[ PYGMY_AT_MAXCOMMANDS ];
                PYGMYFIFO Stream[ 16 ];
                void *File; // Reserved for use by print, PYGMYFILE type
                } PYGMYSYSTEM;
            
//------------------------------------------------------------------------------
//----------------------------Constant Data-------------------------------------
           
extern const u8 PYGSYS_BITS[];
extern const u8 PYGSYS_INVBITS[];
extern const u8 PYGMYHEXCHARS[];
extern const u8 PYGMY_MSG_NULL[];
extern const u8 PYGMY_MSG_BLANK[];
extern const u8 PYGMY_DAYSINMONTH[];


//STM32 Backup Regs
typedef struct
{
  u32 RESERVED0;
  vu16 DR1;
  u16  RESERVED1;
  vu16 DR2;
  u16  RESERVED2;
  vu16 DR3;
  u16  RESERVED3;
  vu16 DR4;
  u16  RESERVED4;
  vu16 DR5;
  u16  RESERVED5;
  vu16 DR6;
  u16  RESERVED6;
  vu16 DR7;
  u16  RESERVED7;
  vu16 DR8;
  u16  RESERVED8;
  vu16 DR9;
  u16  RESERVED9;
  vu16 DR10;
  u16  RESERVED10;
  vu16 RTCCR;
  u16  RESERVED11;
  vu16 CR;
  u16  RESERVED12;
  vu16 CSR;
  u16  RESERVED13;
} BKP_TYPEDEF;

//STM32 Power/Sleep Mode Regs
typedef struct
{
  vu32 CR;
  vu32 CSR;
} PWR_TYPEDEF;


//--------------------------------------------------------------------------------------------
//-----------------------------------------------STM32 Clocks---------------------------------
/*
typedef struct
{
  u32 SYSCLK_Frequency;
  u32 HCLK_Frequency;
  u32 PCLK1_Frequency;
  u32 PCLK2_Frequency;
  u32 ADCCLK_Frequency;
} RCC_CLOCKSTYPEDEF;
*/
typedef struct
{
  vu32 CR;
  vu32 CFGR;
  vu32 CIR;
  vu32 APB2RSTR;
  vu32 APB1RSTR;
  vu32 AHBENR;
  vu32 APB2ENR;
  vu32 APB1ENR;
  vu32 BDCR;
  vu32 CSR;
  vu32 CFGR2;
} RCC_TYPEDEF;

#define RCC_HSE_OFF                      ((u32)0x00000000)
#define RCC_HSE_ON                       ((u32)0x00010000)
#define RCC_HSE_Bypass                   ((u32)0x00040000)
#define RCC_PLLSOURCE_HSI_Div2           ((u32)0x00000000)
#define RCC_PLLSOURCE_HSE_Div1           ((u32)0x00010000)
#define RCC_PLLSOURCE_HSE_Div2           ((u32)0x00030000)

#define RCC_PLL_X2                     ((u32)0x00000000)
#define RCC_PLL_X3                     ((u32)0x00040000)
#define RCC_PLL_X4                     ((u32)0x00080000)
#define RCC_PLL_X5                     ((u32)0x000C0000)
#define RCC_PLL_X6                     ((u32)0x00100000)
#define RCC_PLL_X7                     ((u32)0x00140000)
#define RCC_PLL_X8                     ((u32)0x00180000)
#define RCC_PLL_X9                     ((u32)0x001C0000)
#define RCC_PLL_X10                    ((u32)0x00200000)
#define RCC_PLL_X11                    ((u32)0x00240000)
#define RCC_PLL_X12                    ((u32)0x00280000)
#define RCC_PLL_X13                    ((u32)0x002C0000)
#define RCC_PLL_X14                    ((u32)0x00300000)
#define RCC_PLL_X15                    ((u32)0x00340000)
#define RCC_PLL_X16                    ((u32)0x00380000)

#define RCC_SYSCLKSOURCE_HSI             ((u32)0x00000000)
#define RCC_SYSCLKSOURCE_HSE             ((u32)0x00000001)
#define RCC_SYSCLKSOURCE_PLLCLK          ((u32)0x00000002)

#define RCC_SYSCLK_DIV1                  ((u32)0x00000000)
#define RCC_SYSCLK_DIV2                  ((u32)0x00000080)
#define RCC_SYSCLK_DIV4                  ((u32)0x00000090)
#define RCC_SYSCLK_DIV8                  ((u32)0x000000A0)
#define RCC_SYSCLK_DIV16                 ((u32)0x000000B0)
#define RCC_SYSCLK_DIV64                 ((u32)0x000000C0)
#define RCC_SYSCLK_DIV128                ((u32)0x000000D0)
#define RCC_SYSCLK_DIV256                ((u32)0x000000E0)
#define RCC_SYSCLK_DIV512                ((u32)0x000000F0)

#define RCC_HCLK_DIV1                    ((u32)0x00000000)
#define RCC_HCLK_DIV2                    ((u32)0x00000400)
#define RCC_HCLK_DIV4                    ((u32)0x00000500)
#define RCC_HCLK_DIV8                    ((u32)0x00000600)
#define RCC_HCLK_DIV16                   ((u32)0x00000700)

#define RCC_IT_LSIRDY                    ((u8)0x01)
#define RCC_IT_LSERDY                    ((u8)0x02)
#define RCC_IT_HSIRDY                    ((u8)0x04)
#define RCC_IT_HSERDY                    ((u8)0x08)
#define RCC_IT_PLLRDY                    ((u8)0x10)
#define RCC_IT_CSS                       ((u8)0x80)

#define RCC_USBCLKSOURCE_PLLCLK_1DIV5    ((u8)0x00)
#define RCC_USBCLKSOURCE_PLLCLK_DIV1     ((u8)0x01)

#define RCC_PCLK2_Div2                   ((u32)0x00000000)
#define RCC_PCLK2_Div4                   ((u32)0x00004000)
#define RCC_PCLK2_Div6                   ((u32)0x00008000)
#define RCC_PCLK2_Div8                   ((u32)0x0000C000)

#define RCC_LSE_OFF                      ((u8)0x00)
#define RCC_LSE_ON                       ((u8)0x01)
#define RCC_LSE_BYPASS                   ((u8)0x04)

#define RCC_RTCCLKSOURCE_LSE             ((u32)0x00000100)
#define RCC_RTCCLKSOURCE_LSI             ((u32)0x00000200)
#define RCC_RTCCLKSOURCE_HSE_Div128      ((u32)0x00000300)

#define RCC_AHB_DMA                ((u32)0x00000001)
#define RCC_AHB_SRAM               ((u32)0x00000004)
#define RCC_AHB_FLITF              ((u32)0x00000010)

#define RCC_APB2_AFIO              ((u32)0x00000001)
#define RCC_APB2_GPIOA             ((u32)0x00000004)
#define RCC_APB2_GPIOB             ((u32)0x00000008)
#define RCC_APB2_GPIOC             ((u32)0x00000010)
#define RCC_APB2_GPIOD             ((u32)0x00000020)
#define RCC_APB2_GPIOE             ((u32)0x00000040)
#define RCC_APB2_ADC1              ((u32)0x00000200)
#define RCC_APB2_ADC2              ((u32)0x00000400)
#define RCC_APB2_TIM1              ((u32)0x00000800)
#define RCC_APB2_SPI1              ((u32)0x00001000)
#define RCC_APB2_USART1            ((u32)0x00004000)
#define RCC_APB2_ALL               ((u32)0x00005E7D)

#define RCC_APB1_TIM2              ((u32)0x00000001)
#define RCC_APB1_TIM3              ((u32)0x00000002)
#define RCC_APB1_TIM4              ((u32)0x00000004)
#define RCC_APB1_WWDG              ((u32)0x00000800)
#define RCC_APB1_SPI2              ((u32)0x00004000)
#define RCC_APB1_USART2            ((u32)0x00020000)
#define RCC_APB1_USART3            ((u32)0x00040000)
#define RCC_APB1_I2C1              ((u32)0x00200000)
#define RCC_APB1_I2C2              ((u32)0x00400000)
#define RCC_APB1_USB               ((u32)0x00800000)
#define RCC_APB1_CAN               ((u32)0x02000000)
#define RCC_APB1_BKP               ((u32)0x08000000)
#define RCC_APB1_PWR               ((u32)0x10000000)
#define RCC_APB1_ALL               ((u32)0x1AE64807)
#define RCC_MCO_NOCLOCK                  ((u8)0x00)
#define RCC_MCO_SYSCLK                   ((u8)0x04)
#define RCC_MCO_HSI                      ((u8)0x05)
#define RCC_MCO_HSE                      ((u8)0x06)
#define RCC_MCO_PLLCLK_Div2              ((u8)0x07)
#define RCC_FLAG_HSIRDY                  ((u8)0x20)
#define RCC_FLAG_HSERDY                  ((u8)0x31)
#define RCC_FLAG_PLLRDY                  ((u8)0x39)
#define RCC_FLAG_LSERDY                  ((u8)0x41)
#define RCC_FLAG_LSIRDY                  ((u8)0x61)
#define RCC_FLAG_PINRST                  ((u8)0x7A)
#define RCC_FLAG_PORRST                  ((u8)0x7B)
#define RCC_FLAG_SFTRST                  ((u8)0x7C)
#define RCC_FLAG_IWDGRST                 ((u8)0x7D)
#define RCC_FLAG_WWDGRST                 ((u8)0x7E)
#define RCC_FLAG_LPWRRST                 ((u8)0x7F)

#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BITNUMBER           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))
#define PLLON_BITNUMBER           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))
#define CSSON_BITNUMBER           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)
#define USBPRE_BITNUMBER          0x16
#define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))
#define BDRST_BITNUMBER           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BITNUMBER           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

//STM32 UART Modules
//STM32 SPI 

#define CR1_SPE_SET          ((u16)0x0040)
#define CR1_SPE_RESET        ((u16)0xFFBF)
#define CR1_CRCNEXT_SET      ((u16)0x1000)
#define CR1_CRCEN_SET        ((u16)0x2000)
#define CR1_CRCEN_RESET      ((u16)0xDFFF)
#define CR2_SSOE_SET        ((u16)0x0004)
#define CR2_SSOE_RESET      ((u16)0xFFFB)
#define CR1_CLEAR_Mask       ((u16)0x3040)

//STM32 USARTS

// Guard Time and Prescaler Regsiter
// For IrDA and Smart card, bit fields, undefined at this time

typedef struct {
    vu32 CR;
    vu32 SWTRIGR;
    vu32 DHR12R1;
    vu32 DHR12L1;
    vu32 DHR8R1;
    vu32 DHR12R2;
    vu32 DHR12L2;
    vu32 DHR8R2;
    vu32 DHR12RD;
    vu32 DHR12LD;
    vu32 DHR8RD;
    vu32 DOR1;
    vu32 DOR2;
} DAC_TYPEDEF;

// DAC Bit Defines
#define DAC_DMAUDRIE2                   BIT29
#define DAC_DMAEN2                      BIT28
#define DAC_MAMP2_CLEAR                 ( BIT27 | BIT26 | BIT25 | BIT24 )
#define DAC_MAMP2_1                     0
#define DAC_MAMP2_3                     BIT24
#define DAC_MAMP2_7                     BIT25
#define DAC_MAMP2_15                    ( BIT25 | BIT24 )
#define DAC_MAMP2_31                    BIT26
#define DAC_MAMP2_63                    ( BIT26 | BIT24 )
#define DAC_MAMP2_127                   ( BIT26 | BIT25 )
#define DAC_MAMP2_255                   ( BIT26 | BIT25 | BIT24 )
#define DAC_MAMP2_511                   BIT27
#define DAC_MAMP2_1023                  ( BIT27 | BIT24 )
#define DAC_MAMP2_2047                  ( BIT27 | BIT25 )
#define DAC_MAMP2_4095                  ( BIT27 | BIT25 | BIT24 )
#define DAC_WAVE2_CLEAR                 ( BIT23 | BIT22 )
#define DAC_WAVE2_OFF                   0
#define DAC_WAVE2_NOISE                 BIT22
#define DAC_WAVE2_TRANGLE               BIT23

#define DAC_TSEL2_CLEAR                 ( BIT21 | BIT20 | BIT19 )
#define DAC_TSEL2_TIMER6                0
#define DAC_TSEL2_TIMER3                BIT19
#define DAC_TSEL2_TIMER7                BIT20
#define DAC_TSEL2_TIMER15               ( BIT20 | BIT19 )
#define DAC_TSEL2_TIMER2                BIT21
#define DAC_TSEL2_TIMER4                ( BIT21 | BIT19 )
#define DAC_TSEL2_EXTI9                 ( BIT21 | BIT20 )
#define DAC_TSEL2_SOFTWARE              ( BIT21 | BIT20 | BIT19 )
#define DAC_TEN2                        BIT18
#define DAC_BOFF2                       BIT17
#define DAC_EN2                         BIT16          

#define DAC_DMAUDRIE1                   BIT13
#define DAC_DMAEN1                      BIT12
#define DAC_MAMP1_CLEAR                 ( BIT11 | BIT10 | BIT9 | BIT8 )
#define DAC_MAMP1_1                     0
#define DAC_MAMP1_3                     BIT8
#define DAC_MAMP1_7                     BIT9
#define DAC_MAMP1_15                    ( BIT9 | BIT8 )
#define DAC_MAMP1_31                    BIT10
#define DAC_MAMP1_63                    ( BIT10 | BIT8 )
#define DAC_MAMP1_127                   ( BIT10 | BIT9 )
#define DAC_MAMP1_255                   ( BIT10 | BIT9 | BIT8 )
#define DAC_MAMP1_511                   BIT11
#define DAC_MAMP1_1023                  ( BIT11 | BIT8 )
#define DAC_MAMP1_2047                  ( BIT11 | BIT9 )
#define DAC_MAMP1_4095                  ( BIT11 | BIT9 | BIT8 )
#define DAC_WAVE1_CLEAR                 ( BIT7 | BIT6 )
#define DAC_WAVE1_OFF                   0
#define DAC_WAVE1_NOISE                 BIT6
#define DAC_WAVE1_TRANGLE               BIT7
#define DAC_TSEL1_CLEAR                 ( BIT5 | BIT4 | BIT3 )
#define DAC_TSEL1_TIMER6                0
#define DAC_TSEL1_TIMER3                BIT3
#define DAC_TSEL1_TIMER7                BIT4
#define DAC_TSEL1_TIMER15               ( BIT4 | BIT3 )
#define DAC_TSEL1_TIMER2                BIT5
#define DAC_TSEL1_TIMER4                ( BIT5 | BIT3 )
#define DAC_TSEL1_EXTI9                 ( BIT5 | BIT4 )
#define DAC_TSEL1_SOFTWARE              ( BIT5 | BIT4 | BIT3 )
#define DAC_TEN1                        BIT2
#define DAC_BOFF1                       BIT1
#define DAC_EN1                         BIT0

// End DAC Bit Defines

//STM32  External and Internal Peripheral defines
#define SRAM_BASE             ((u32)0x20000000)
#define PERIPH_BASE           ((u32)0x40000000)
#define PERIPH_BB_BASE        ((u32)0x42000000) // Alias
#define SRAM_BB_BASE          ((u32)0x22000000) // Alias
#define FLASH_BASE            ((u32)0x40022000) // Flash Registers Starting Address
#define OB_BASE               ((u32)0x1FFFF800) // Flash Option Registers Starting Address

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

#define TIM2_BASE                   (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE                   (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE                   (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE                   (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE                   (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE                   (APB1PERIPH_BASE + 0x1400)
#define TIM8_BASE                   (APB2PERIPH_BASE + 0x3400)
#define TIM9_BASE                   (APB2PERIPH_BASE + 0x4C00)
#define TIM10_BASE                  (APB2PERIPH_BASE + 0x5000)
#define TIM11_BASE                  (APB2PERIPH_BASE + 0x5400)
#define TIM12_BASE                  (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE                  (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE                  (APB1PERIPH_BASE + 0x2000)
#define TIM15_BASE                  (APB2PERIPH_BASE + 0x4000)
#define TIM16_BASE                  (APB2PERIPH_BASE + 0x4400)
#define TIM17_BASE                  (APB2PERIPH_BASE + 0x4800)

#ifndef RTC_BASE
    #define RTC_BASE                (APB1PERIPH_BASE + 0x2800)
#endif
#ifndef WWDG_BASE
    #define WWDG_BASE               (APB1PERIPH_BASE + 0x2C00)
#endif
#ifndef IWDG_BASE
    #define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#endif

#define BKP_BASE              (APB1PERIPH_BASE + 0x6C00)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)

//#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000)
//#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)

//#define ADC1_BASE             (APB2PERIPH_BASE + 0x2400)
//#define ADC2_BASE             (APB2PERIPH_BASE + 0x2800)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00)
#define DAC_BASE                    (APB1PERIPH_BASE + 0x7400)
#define DAC                 ((DAC_TYPEDEF *) DAC_BASE)

//STM32 DMA
typedef struct
{
  vu32 CCR;
  vu32 CNDTR;
  vu32 CPAR;
  vu32 CMAR;
} DMA_CHANNEL_TYPEDEF;

typedef struct
{
  vu32 ISR;
  vu32 IFCR;
} DMA_TYPEDEF;

#define DMA_MEM2MEM             BIT14
#define DMA_PRIORITY_MASK       ( BIT13|BIT12 )
#define DMA_PRIORITY_LOW        0
#define DMA_PRIORITY_MEDIUM     BIT12
#define DMA_PRIORITY_HIGH       BIT13
#define DMA_PRIORITY_VERYHIGH   ( BIT13|BIT12 )
#define DMA_MSIZE_MASK          ( BIT11|BIT10 )
#define DMA_MSIZE8              0
#define DMA_MSIZE16             BIT10
#define DMA_MSIZE32             BIT11
#define DMA_PSIZE_MASK          ( BIT9|BIT8 )
#define DMA_PSIZE8              0
#define DMA_PSIZE16             BIT8
#define DMA_PSIZE32             BIT9
#define DMA_MINC                BIT7
#define DMA_PINC                BIT6
#define DMA_CIRC                BIT5
#define DMA_DIR                 BIT4
#define DMA_TEIE                BIT3
#define DMA_HTIE                BIT2
#define DMA_TCIE                BIT1
#define DMA_EN                  BIT0  


#define DMA1_BASE (AHBPERIPH_BASE + 0x0000)
#define DMA2_BASE (AHBPERIPH_BASE + 0x0400)
#define DMA1 ((DMA_TYPEDEF *) DMA1_BASE)
#define DMA2 ((DMA_TYPEDEF *) DMA2_BASE)
#define DMA1_CH1_BASE (AHBPERIPH_BASE + 0x0008)
#define DMA1_CH2_BASE (AHBPERIPH_BASE + 0x001C)
#define DMA1_CH3_BASE (AHBPERIPH_BASE + 0x0030)
#define DMA1_CH4_BASE (AHBPERIPH_BASE + 0x0044)
#define DMA1_CH5_BASE (AHBPERIPH_BASE + 0x0058)
#define DMA1_CH6_BASE (AHBPERIPH_BASE + 0x006C)
#define DMA1_CH7_BASE (AHBPERIPH_BASE + 0x0080)
#define DMA2_CH1_BASE (AHBPERIPH_BASE + 0x0408)
#define DMA2_CH2_BASE (AHBPERIPH_BASE + 0x041C)
#define DMA2_CH3_BASE (AHBPERIPH_BASE + 0x0430)
#define DMA2_CH4_BASE (AHBPERIPH_BASE + 0x0444)
#define DMA2_CH5_BASE (AHBPERIPH_BASE + 0x0458)

#define DMA1_CH1 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH1_BASE)
#define DMA1_CH2 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH2_BASE)
#define DMA1_CH3 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH3_BASE)
#define DMA1_CH4 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH4_BASE)
#define DMA1_CH5 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH5_BASE)
#define DMA1_CH6 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH6_BASE)
#define DMA1_CH7 ((DMA_CHANNEL_TYPEDEF *)DMA1_CH7_BASE)
#define DMA2_CH1 ((DMA_CHANNEL_TYPEDEF *)DMA2_CH1_BASE)
#define DMA2_CH2 ((DMA_CHANNEL_TYPEDEF *)DMA2_CH2_BASE)
#define DMA2_CH3 ((DMA_CHANNEL_TYPEDEF *)DMA2_CH3_BASE)
#define DMA2_CH4 ((DMA_CHANNEL_TYPEDEF *)DMA2_CH4_BASE)
#define DMA2_CH5 ((DMA_CHANNEL_TYPEDEF *)DMA2_CH5_BASE)

#define PYGMY_DMA1_CH1_DISABLE      DMA1_CH1->CCR &= ~DMA_EN;
#define PYGMY_DMA1_CH2_DISABLE      DMA1_CH2->CCR &= ~DMA_EN;
#define PYGMY_DMA1_CH3_DISABLE      DMA1_CH3->CCR &= ~DMA_EN;
#define PYGMY_DMA1_CH4_DISABLE      DMA1_CH1->CCR &= ~DMA_EN;
#define PYGMY_DMA1_CH5_DISABLE      DMA1_CH2->CCR &= ~DMA_EN;
#define PYGMY_DMA1_CH6_DISABLE      DMA1_CH3->CCR &= ~DMA_EN;
#define PYGMY_DMA1_CH7_DISABLE      DMA1_CH1->CCR &= ~DMA_EN;
#define PYGMY_DMA2_CH1_DISABLE      DMA2_CH2->CCR &= ~DMA_EN;
#define PYGMY_DMA2_CH2_DISABLE      DMA2_CH3->CCR &= ~DMA_EN;
#define PYGMY_DMA2_CH3_DISABLE      DMA2_CH1->CCR &= ~DMA_EN;
#define PYGMY_DMA2_CH4_DISABLE      DMA2_CH2->CCR &= ~DMA_EN;
#define PYGMY_DMA2_CH5_DISABLE      DMA2_CH3->CCR &= ~DMA_EN;

#define PYGMY_DMA1_CH1_ENABLE       DMA1_CH1->CCR |= DMA_EN;
#define PYGMY_DMA1_CH2_ENABLE       DMA1_CH2->CCR |= DMA_EN;
#define PYGMY_DMA1_CH3_ENABLE       DMA1_CH3->CCR |= DMA_EN;
#define PYGMY_DMA1_CH4_ENABLE       DMA1_CH1->CCR |= DMA_EN;
#define PYGMY_DMA1_CH5_ENABLE       DMA1_CH2->CCR |= DMA_EN;
#define PYGMY_DMA1_CH6_ENABLE       DMA1_CH3->CCR |= DMA_EN;
#define PYGMY_DMA1_CH7_ENABLE       DMA1_CH1->CCR |= DMA_EN;
#define PYGMY_DMA2_CH1_ENABLE       DMA2_CH2->CCR |= DMA_EN;
#define PYGMY_DMA2_CH2_ENABLE       DMA2_CH3->CCR |= DMA_EN;
#define PYGMY_DMA2_CH3_ENABLE       DMA2_CH1->CCR |= DMA_EN;
#define PYGMY_DMA2_CH4_ENABLE       DMA2_CH2->CCR |= DMA_EN;
#define PYGMY_DMA2_CH5_ENABLE       DMA2_CH3->CCR |= DMA_EN;

#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
//#define SCS_BASE              ((u32)0xE000E000)         // System Control
//#define SysTick_BASE          (SCS_BASE + 0x0010)
//#define NVIC_BASE             (SCS_BASE + 0x0100)
//#define SCB_BASE              (SCS_BASE + 0x0D00)       // System Control

// Define Pointers to registers ( access through structures for STM32 
#define TIM1                    ((TIM1_TYPEDEF *) TIM1_BASE)
#define TIM2                ((TIM_TYPEDEF *) TIM2_BASE)
#define TIM3                ((TIM_TYPEDEF *) TIM3_BASE)
#define TIM4                ((TIM_TYPEDEF *) TIM4_BASE)
#define TIM5                ((TIM_TYPEDEF *) TIM5_BASE)
#define TIM6                ((TIM_TYPEDEF *) TIM6_BASE)
#define TIM7                ((TIM_TYPEDEF *) TIM7_BASE)
#define TIM8                ((TIM_TYPEDEF *) TIM8_BASE)
#define TIM9                ((TIM_TYPEDEF *) TIM9_BASE)
#define TIM10               ((TIM_TYPEDEF *) TIM10_BASE)
#define TIM11               ((TIM_TYPEDEF *) TIM11_BASE)
#define TIM12               ((TIM_TYPEDEF *) TIM12_BASE)
#define TIM13               ((TIM_TYPEDEF *) TIM13_BASE)
#define TIM14               ((TIM_TYPEDEF *) TIM14_BASE)
#define TIM15               ((TIM_TYPEDEF *) TIM15_BASE)
#define TIM16               ((TIM_TYPEDEF *) TIM16_BASE)
#define TIM17               ((TIM_TYPEDEF *) TIM17_BASE)

/*
#ifndef RTC
    #define RTC                 ((RTC_TYPEDEF *) RTC_BASE)
#endif
#ifndef WWDG
    #define WWDG                ((WWDG_TYPEDEF *) WWDG_BASE)
#endif
#ifndef IWDG
    #define IWDG                ((IWDG_TYPEDEF *) IWDG_BASE)
#endif
*/
#ifndef BKP
    #define BKP                 ((BKP_TYPEDEF *) BKP_BASE)
#endif
#ifndef PWR
    #define PWR                 ((PWR_TYPEDEF *) PWR_BASE)
#endif

//#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
//#define ADC2                ((ADC_TypeDef *) ADC2_BASE)

#ifndef SPI1
    #define SPI1                    ((SPI_TYPEDEF *) SPI1_BASE)
#endif
#ifndef USART1
    #define USART1                  ((USART_TYPEDEF *) USART1_BASE)
#endif

//#define FLASH                   ((FLASH_TypeDef *) FLASH_BASE)
#define OB                      ((OB_TYPEDEF *) OB_BASE) 
#define RCC                     ((RCC_TYPEDEF *) RCC_BASE)
//#define SysTick             ((SysTick_TypeDef *) SysTick_BASE)
//#define NVIC                ((NVIC_TypeDef *) NVIC_BASE)
//#define SCB                 ((SCB_TypeDef *) SCB_BASE)  


//--------------------------------------------------------------------------------------------
//-------------------------------------------RCC bits-----------------------------------------
//RCC_CR
#define RCC_PLLRDY              BIT25                   
#define RCC_PLLON               BIT24                  
#define RCC_CSSON               BIT19                   
#define RCC_HSEBYP              BIT18                   
#define RCC_HSERDY              BIT17                  
#define RCC_HSEON               BIT16                   
#define RCC_HSIRDY              BIT1                  
#define RCC_HSION               BIT0                    
#define RCC_MCO_CLEAR           (BIT26|BIT25|BIT24)       
#define RCC_MCO_SYSCLOCK        BIT26                   
#define RCC_MCO_INTERNALRC      (BIT26|BIT24)             
#define RCC_MCO_EXTCLOCK        (BIT26|BIT25)             
#define RCC_MCO_PLLDIV2         (BIT26|BIT25|BIT24)       
#define RCC_USBPRE              BIT22                   
#define RCC_PLL_CLEAR           (BIT21|BIT20|BIT19|BIT18) 
#define RCC_PLL_MUL2            0                      
#define RCC_PLL_MUL3            BIT18                   
#define RCC_PLL_MUL4            BIT19                  
#define RCC_PLL_MUL5            (BIT19|BIT18)         
#define RCC_PLL_MUL6            BIT20                   
#define RCC_PLL_MUL7            (BIT20|BIT18)            
#define RCC_PLL_MUL8            (BIT20|BIT19)            
#define RCC_PLL_MUL9            (BIT20|BIT19|BIT18)     
#define RCC_PLL_MUL10           BIT21                   
#define RCC_PLL_MUL11           (BIT21|BIT18)            
#define RCC_PLL_MUL12           (BIT21|BIT19)            
#define RCC_PLL_MUL13           (BIT21|BIT19|BIT18)     
#define RCC_PLL_MUL14           (BIT21|BIT20)            
#define RCC_PLL_MUL15           (BIT21|BIT20|BIT18)       
#define RCC_PLL_MUL16           (BIT21|BIT20|BIT19)    
#define RCC_PLLXTPRE            BIT17                  
#define RCC_PLLSRC              BIT16                  
#define RCC_ADC_CLEAR           (BIT15|BIT14)           
#define RCC_ADC_PRE2            0                     
#define RCC_ADC_PRE4            BIT14                 
#define RCC_ADC_PRE6            BIT15                  
#define RCC_ADC_PRE8            (BIT15|BIT14)           
//PLL HCLK for APB
#define RCC_PRE2_CLEAR          (BIT13|BIT12|BIT11)       
#define RCC_PRE2_DIV1           0                     
#define RCC_PRE2_DIV2           BIT13                   
#define RCC_PRE2_DIV4           (BIT13|BIT11)             
#define RCC_PRE2_DIV8           (BIT13|BIT12)           
#define RCC_PRE2_DIV16          (BIT13|BIT12|BIT11)      
#define RCC_PRE1_CLEAR          (BIT10|BIT9|BIT8)        
#define RCC_PRE1_DIV1           0                     
#define RCC_PRE1_DIV2           BIT10                   
#define RCC_PRE1_DIV4           (BIT10|BIT8)             
#define RCC_PRE1_DIV8           (BIT10|BIT9)              
#define RCC_PRE1_DIV16          (BIT10|BIT9|BIT8)         
//PLL SYSCLK for AHB
#define RCC_HPRE_CLEAR          (BIT7|BIT6|BIT5|BIT4)    
#define RCC_HPRE_DIV1           0                       
#define RCC_HPRE_DIV2           BIT7                  
#define RCC_HPRE_DIV4           (BIT7|BIT4)              
#define RCC_HPRE_DIV8           (BIT7|BIT5)               
#define RCC_HPRE_DIV16          (BIT7|BIT5|BIT4)         
#define RCC_HPRE_DIV64          (BIT7|BIT6)              
#define RCC_HPRE_DIV128         (BIT7|BIT6|BIT4)        
#define RCC_HPRE_DIV256         (BIT7|BIT6|BIT5 )         
#define RCC_HPRE_DIV512         (BIT7|BIT6|BIT5|BIT4)     
//PLL System Clock Swtich Status
#define RCC_SWS                 (BIT3|BIT2)          
#define RCC_SWS_HSI             0                     
#define RCC_SWS_HSE             BIT2                  
#define RCC_SWS_PLL             BIT3                   
#define RCC_SW_CLEAR            BIT1|BIT0              
#define RCC_SW_HSI              0                     
#define RCC_SW_HSE              BIT0                   
#define RCC_SW_PLL              BIT1                   
//RCC_CIR Clock Interrupt 
#define RCC_CSSC                BIT23                  
#define RCC_PLLRDYC             BIT20                  
#define RCC_HSERDYC             BIT19                 
#define RCC_HSIRDYC             BIT18                  
#define RCC_LSERDYC             BIT17                 
#define RCC_LSIRDYC             BIT16                  
#define RCC_PLLRDYIE            BIT12                  
#define RCC_HSERDYIE            BIT11                  
#define RCC_HSIRDYIE            BIT10                  
#define RCC_LSERDYIE            BIT9                   
#define RCC_LSIRDYIE            BIT8                    
#define RCC_CSSF                BIT7                   
#define RCC_PLLRDYF             BIT4                   
#define RCC_HSERDYF             BIT3                   
#define RCC_HSIRDYF             BIT2                  
#define RCC_LSERDYF             BIT1                   
#define RCC_LSIRDYF             BIT0                  
//RCC_APB2RSTR Peripheral Reset Register
#define RCC_ADC3RST             BIT15                   
#define RCC_USART1RST           BIT14                  
#define RCC_TIM8RST             BIT13                  
#define RCC_SPI1RST             BIT12                  
#define RCC_TIM1RST             BIT11                 
#define RCC_ADC2RST             BIT10                   
#define RCC_ADC1RST             BIT9                    
#define RCC_IOPGRST             BIT8                    
#define RCC_IOPFRST             BIT7                   
#define RCC_IOPERST             BIT6                   
#define RCC_IOPDRST             BIT5                   
#define RCC_IOPCRST             BIT4                    
#define RCC_IOPBRST             BIT3                  
#define RCC_IOPARST             BIT2                   
#define RCC_AFIORST             BIT0                   
//RCC_APB1RSTR Peripheral Reset Register
#define RCC_DACRST              BIT29                   
#define RCC_PWRRST              BIT28                 
#define RCC_BKPRST              BIT27                
#define RCC_CANRST              BIT25                  
#define RCC_USBRST              BIT23                  
#define RCC_I2C2RST             BIT22                  
#define RCC_I2C1RST             BIT21                   
#define RCC_UART5RST            BIT20                 
#define RCC_UART4RST            BIT19               
#define RCC_UART3RST            BIT18                  
#define RCC_UART2RST            BIT17                  
#define RCC_SPI3RST             BIT15                  
#define RCC_SPI2RST             BIT14                  
#define RCC_WWDGRST             BIT11                
#define RCC_TIM7RST             BIT5                  
#define RCC_TIM6RST             BIT4                   
#define RCC_TIM5RST             BIT3                    
#define RCC_TIM4RST             BIT2                   
#define RCC_TIM3RST             BIT1                 
#define RCC_TIM2RST             BIT0                    
//RCC_AHBENR Peripheral Clock Enable
#define RCC_SDIOEN              BIT10                  
#define RCC_FSMCEN              BIT8                  
#define RCC_CRCEN               BIT6                    
#define RCC_FLITFEN             BIT4                 
#define RCC_SRAMEN              BIT2                  
#define RCC_DMA2EN              BIT1                    
#define RCC_DMA1EN              BIT0                   
//RCC_APB2ENR Peripheral Clock Enable
#define RCC_TIM11EN             BIT21
#define RCC_TIM10EN             BIT20
#define RCC_TIM9EN              BIT19
#define RCC_TIM17EN             BIT18
#define RCC_TIM16EN             BIT17
#define RCC_TIM15EN             BIT16
#define RCC_ADC3EN              BIT15                 
#define RCC_USART1EN            BIT14                  
#define RCC_TIM8EN              BIT13                 
#define RCC_SPI1EN              BIT12                
#define RCC_TIM1EN              BIT11                  
#define RCC_ADC2EN              BIT10                  
#define RCC_ADC1EN              BIT9                 
#define RCC_IOPGEN              BIT8                    
#define RCC_IOPFEN              BIT7                   
#define RCC_IOPEEN              BIT6                  
#define RCC_IOPDEN              BIT5                    
#define RCC_IOPCEN              BIT4                 
#define RCC_IOPBEN              BIT3                 
#define RCC_IOPAEN              BIT2                   
#define RCC_AFIOEN              BIT0                  
//RCC_APB1ENR Peripheral Clock Enable
#define RCC_DACEN               BIT29                 
#define RCC_PWREN               BIT28                  
#define RCC_BKPEN               BIT27                  
#define RCC_CANEN               BIT25                   
#define RCC_USBEN               BIT23                 
#define RCC_I2C2EN              BIT22                  
#define RCC_I2C1EN              BIT21                  
#define RCC_USART5EN            BIT20                 
#define RCC_USART4EN            BIT19                 
#define RCC_USART3EN            BIT18                  
#define RCC_USART2EN            BIT17                  
#define RCC_SPI3EN              BIT15                  
#define RCC_SPI2EN              BIT14                  
#define RCC_WWDGEN              BIT11  
#define RCC_TIM14EN             BIT8
#define RCC_TIM13EN             BIT7
#define RCC_TIM12EN             BIT6
#define RCC_TIM7EN              BIT5                  
#define RCC_TIM6EN              BIT4                  
#define RCC_TIM5EN              BIT3                 
#define RCC_TIM4EN              BIT2                  
#define RCC_TIM3EN              BIT1                 
#define RCC_TIM2EN              BIT0                  
//RCC_BDCR Backup Domain Control Register
#define RCC_BDRST               BIT16                
#define RCC_RTCEN               BIT15                 
#define RCC_RTCSEL1             BIT9                  
#define RCC_RTCSEL0             BIT8                  
#define RCC_LSEBYP              BIT2                   
#define RCC_LSERDY              BIT1                 
#define RCC_LSEON               BIT0                   
//RCC_CSR Control Status Register
#define RCC_LPWRRSTF            BIT31                
#define RCC_WWDGRSTF            BIT30              
#define RCC_IWDGRSTF            BIT29                 
#define RCC_SFTRSTF             BIT28                  
#define RCC_PORRSTF             BIT27                 
#define RCC_PINRSTF             BIT26                
#define RCC_RMVF                BIT24                  
#define RCC_LSIRDY              BIT1                  
#define RCC_LSION               BIT0               

enum { SYSCLK, HSI, HSE, PLLDIV2 };

//---------------------------------------------------------------------------------------------
//---------------------------------------------Reset-------------------------------------------
#ifndef PYGMY_RESET
    #define PYGMY_RESET                 SCB->AIRCR = AIRCR_VECTKEY_MASK | BIT2
#endif

//-------------------------------------------End Reset-----------------------------------------
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
//--------------------------------------------WatchDog-----------------------------------------
typedef struct
{
  volatile u32 KR;
  volatile u32 PR;
  volatile u32 RLR;
  volatile u32 SR;
} IWDG_TYPEDEF;

typedef struct
{
  volatile u32 CR;
  volatile u32 CFR;
  volatile u32 SR;
} WWDG_TYPEDEF;

#ifndef WWDG_BASE
    #define WWDG_BASE           (APB1PERIPH_BASE + 0x2C00)
#endif
#ifndef IWDG_BASE
    #define IWDG_BASE           (APB1PERIPH_BASE + 0x3000)
#endif
#ifndef WWDG
    #define WWDG                ((WWDG_TYPEDEF *) WWDG_BASE)
#endif
#ifndef IWDG
    #define IWDG                ((IWDG_TYPEDEF *) IWDG_BASE)
#endif

#define IWDT_PREDIV4                            0
#define IWDT_PREDIV8                            BIT0
#define IWDT_PREDIV16                           BIT1
#define IWDT_PREDIV32                           ( BIT1 | BIT0 )
#define IWDT_PREDIV64                           BIT2
#define IWDT_PREDIV128                          ( BIT2 | BIT0 )
#define IWDT_PREDIV256                          ( BIT2 | BIT1 )
#define PYGMY_WATCHDOG_UNLOCK                   IWDG->KR = 0x5555 // register access key
#define PYGMY_WATCHDOG_PRESCALER( __IWDT_PRE )  IWDG->PR = __IWDT_PRE // WD Timer is 12 bit
#define PYGMY_WATCHDOG_TIMER( __IWDT_RELOAD )   IWDG->RLR = __IWDT_RELOAD
#define PYGMY_WATCHDOG_START                    IWDG->KR = 0xCCCC // WD start key
#define PYGMY_WATCHDOG_STOP                     IWDG->KR = 0x0000
#define PYGMY_WATCHDOG_REFRESH                  IWDG->KR = 0xAAAA // WD update key
//------------------------------------------End WatchDog---------------------------------------
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
//---------------------------------------Clock Macro Defs---------------------------------------

#define PYGMY_RCC_HSE_READY                ( RCC->CR & RCC_HSERDY )
#define PYGMY_RCC_PLL_READY                ( RCC->CR & RCC_PLLRDY )
#define PYGMY_RCC_PLL_ENABLE                RCC->CR |= RCC_PLLON;
#define PYGMY_RCC_PLL_DISABLE               RCC->CR &= ~RCC_PLLON;
#define PYGMY_RCC_LSE_ENABLE                RCC->CR |= RCC_LSEON;
#define PYGMY_RCC_LSE_DISABLE               RCC->CR &= ~RCC_LSEON;
#define PYGMY_RCC_HSE_ENABLE                RCC->CR |= RCC_HSEON;
#define PYGMY_RCC_HSE_DISABLE               RCC->CR &= ~( RCC_HSEON | RCC_HSEBYP );
#define PYGMY_RCC_HSE_BYPASS                RCC->CR &= ~RCC_HSEON; RCC->CR |= ( RCC_HSEBYP|RCC_HSEON );
#define PYGMY_RCC_HSI_ENABLE                RCC->CFGR |= RCC_HSION;
#define PYGMY_RCC_HSI_DISABLE               RCC->CFGR &= ~RCC_HSION;
#define PYGMY_RCC_PWR_ENABLE                RCC->APB1ENR |= RCC_PWREN;
#define PYGMY_RCC_PWR_DISABLE               RCC->APB1ENR &= ~RCC_PWREN;
#define PYGMY_RCC_BKP_ENABLE                RCC->APB1ENR |= RCC_BKPEN;
#define PYGMY_RCC_BKP_DISABLE               RCC->APB1ENR &= ~RCC_BKPEN;
#define PYGMY_RCC_DMA1_ENABLE               RCC->AHBENR |= RCC_DMA1EN;
#define PYGMY_RCC_DMA1_DISABLE              RCC->AHBENR &= ~RCC_DMA1EN;
#define PYGMY_RCC_DMA2_ENABLE               RCC->AHBENR |= RCC_DMA2EN;
#define PYGMY_RCC_DMA2_DISABLE              RCC->AHBENR &= ~RCC_DMA2EN;
#define PYGMY_RCC_ADC1_ENABLE               RCC->APB2ENR |= RCC_ADC1EN;
#define PYGMY_RCC_ADC1_DISABLE              RCC->APB2ENR &= ~RCC_ADC1EN;
#define PYGMY_RCC_ADC2_ENABLE               RCC->APB2ENR |= RCC_ADC2EN;
#define PYGMY_RCC_ADC2_DISABLE              RCC->APB2ENR &= ~RCC_ADC2EN;
#define PYGMY_RCC_ADC3_ENABLE               RCC->APB2ENR |= RCC_ADC3EN;
#define PYGMY_RCC_ADC3_DISABLE              RCC->APB2ENR &= ~RCC_ADC3EN;
#define PYGMY_RCC_AFIO_ENABLE               RCC->APB2ENR |= RCC_AFIOEN;
#define PYGMY_RCC_AFIO_DISABLE              RCC->APB2ENR &= ~RCC_AFIOEN;
#define PYGMY_RCC_GPIOA_ENABLE              RCC->APB2ENR |= RCC_IOPAEN;
#define PYGMY_RCC_GPIOB_ENABLE              RCC->APB2ENR |= RCC_IOPBEN;
#define PYGMY_RCC_GPIOC_ENABLE              RCC->APB2ENR |= RCC_IOPCEN;
#define PYGMY_RCC_GPIOD_ENABLE              RCC->APB2ENR |= RCC_IOPDEN;
#define PYGMY_RCC_GPIOE_ENABLE              RCC->APB2ENR |= RCC_IOPEEN;
#define PYGMY_RCC_GPIOF_ENABLE              RCC->APB2ENR |= RCC_IOPFEN;
#define PYGMY_RCC_GPIOG_ENABLE              RCC->APB2ENR |= RCC_IOPGEN;
#define PYGMY_RCC_GPIOA_DISABLE             RCC->APB2ENR &= ~RCC_IOPAEN;
#define PYGMY_RCC_GPIOB_DISABLE             RCC->APB2ENR &= ~RCC_IOPBEN;
#define PYGMY_RCC_GPIOC_DISABLE             RCC->APB2ENR &= ~RCC_IOPCEN;
#define PYGMY_RCC_GPIOD_DISABLE             RCC->APB2ENR &= ~RCC_IOPDEN;
#define PYGMY_RCC_GPIOE_DISABLE             RCC->APB2ENR &= ~RCC_IOPEEN;
#define PYGMY_RCC_GPIOF_DISABLE             RCC->APB2ENR &= ~RCC_IOPFEN;
#define PYGMY_RCC_GPIOG_DISABLE             RCC->APB2ENR &= ~RCC_IOPGEN;
#define PYGMY_RCC_TIMER1_ENABLE             RCC->APB2ENR |= RCC_TIM1EN;
#define PYGMY_RCC_TIMER1_DISABLE            RCC->APB2ENR &= ~RCC_TIM1EN;
#define PYGMY_RCC_TIMER2_ENABLE             RCC->APB1ENR |= RCC_TIM2EN;
#define PYGMY_RCC_TIMER2_DISABLE            RCC->APB1ENR &= ~RCC_TIM2EN;
#define PYGMY_RCC_TIMER3_ENABLE             RCC->APB1ENR |= RCC_TIM3EN;
#define PYGMY_RCC_TIMER3_DISABLE            RCC->APB1ENR &= ~RCC_TIM3EN;
#define PYGMY_RCC_TIMER4_ENABLE             RCC->APB1ENR |= RCC_TIM4EN;
#define PYGMY_RCC_TIMER4_DISABLE            RCC->APB1ENR &= ~RCC_TIM4EN;
#define PYGMY_RCC_TIMER5_ENABLE             RCC->APB1ENR |= RCC_TIM5EN;
#define PYGMY_RCC_TIMER5_DISABLE            RCC->APB1ENR &= ~RCC_TIM5EN;
#define PYGMY_RCC_TIMER6_ENABLE             RCC->APB1ENR |= RCC_TIM6EN;
#define PYGMY_RCC_TIMER6_DISABLE            RCC->APB1ENR &= ~RCC_TIM6EN;
#define PYGMY_RCC_TIMER7_ENABLE             RCC->APB1ENR |= RCC_TIM7EN;
#define PYGMY_RCC_TIMER7_DISABLE            RCC->APB1ENR &= ~RCC_TIM7EN;
#define PYGMY_RCC_TIMER8_ENABLE             RCC->APB2ENR |= RCC_TIM8EN;
#define PYGMY_RCC_TIMER8_DISABLE            RCC->APB2ENR &= ~RCC_TIM8EN;                                            
#define PYGMY_RCC_TIMER9_ENABLE             RCC->APB2ENR |= RCC_TIM9EN;
#define PYGMY_RCC_TIMER9_DISABLE            RCC->APB2ENR &= ~RCC_TIM9EN;
#define PYGMY_RCC_TIMER10_ENABLE            RCC->APB2ENR |= RCC_TIM10EN;
#define PYGMY_RCC_TIMER10_DISABLE           RCC->APB2ENR &= ~RCC_TIM10EN;
#define PYGMY_RCC_TIMER11_ENABLE            RCC->APB2ENR |= RCC_TIM11EN;
#define PYGMY_RCC_TIMER11_DISABLE           RCC->APB2ENR &= ~RCC_TIM11EN;
#define PYGMY_RCC_TIMER12_ENABLE            RCC->APB1ENR |= RCC_TIM12EN;
#define PYGMY_RCC_TIMER12_DISABLE           RCC->APB1ENR &= ~RCC_TIM12EN;
#define PYGMY_RCC_TIMER13_ENABLE            RCC->APB1ENR |= RCC_TIM13EN;
#define PYGMY_RCC_TIMER13_DISABLE           RCC->APB1ENR &= ~RCC_TIM13EN;
#define PYGMY_RCC_TIMER14_ENABLE            RCC->APB1ENR |= RCC_TIM14EN;
#define PYGMY_RCC_TIMER14_DISABLE           RCC->APB1ENR &= ~RCC_TIM14EN;
#define PYGMY_RCC_TIMER15_ENABLE            RCC->APB2ENR |= RCC_TIM15EN;
#define PYGMY_RCC_TIMER15_DISABLE           RCC->APB2ENR &= ~RCC_TIM15EN;
#define PYGMY_RCC_TIMER16_ENABLE            RCC->APB2ENR |= RCC_TIM16EN;
#define PYGMY_RCC_TIMER16_DISABLE           RCC->APB2ENR &= ~RCC_TIM16EN;
#define PYGMY_RCC_TIMER17_ENABLE            RCC->APB2ENR |= RCC_TIM17EN;
#define PYGMY_RCC_TIMER17_DISABLE           RCC->APB2ENR &= ~RCC_TIM17EN;
#define PYGMY_RCC_SPI1_ENABLE               RCC->APB2ENR |= RCC_SPI1EN;
#define PYGMY_RCC_SPI1_DISABLE              RCC->APB2ENR &= ~RCC_SPI1EN;
#define PYGMY_RCC_DAC_ENABLE                RCC->APB1ENR |= RCC_DACEN;
#define PYGMY_RCC_DAC_DISABLE               RCC->APB1ENR &= ~RCC_DACEN;
#define PYGMY_RCC_USB_ENABLE                RCC->APB1ENR |= RCC_USBEN;
#define PYGMY_RCC_USB_DISABLE               RCC->APB1ENR &= ~RCC_USBEN;
#define PYGMY_RCC_USART5_ENABLE             RCC->APB1ENR |= RCC_USART5EN;
#define PYGMY_RCC_USART5_DISABLE            RCC->APB1ENR &= ~RCC_USART5EN;
#define PYGMY_RCC_USART4_ENABLE             RCC->APB1ENR |= RCC_USART4EN;
#define PYGMY_RCC_USART4_DISABLE            RCC->APB1ENR &= ~RCC_USART4EN;
#define PYGMY_RCC_USART3_ENABLE             RCC->APB1ENR |= RCC_USART3EN;
#define PYGMY_RCC_USART3_DISABLE            RCC->APB1ENR &= ~RCC_USART3EN;
#define PYGMY_RCC_USART2_ENABLE             RCC->APB1ENR |= RCC_USART2EN;
#define PYGMY_RCC_USART2_DISABLE            RCC->APB1ENR &= ~RCC_USART2EN;
#define PYGMY_RCC_USART1_ENABLE             RCC->APB2ENR |= RCC_USART1EN;
#define PYGMY_RCC_USART1_DISABLE            RCC->APB2ENR &= ~RCC_USART1EN;
#define PYGMY_RCC_I2C2_ENABLE               RCC->APB1ENR |= RCC_I2C2EN;
#define PYGMY_RCC_I2C2_DISABLE              RCC->APB1ENR &= ~RCC_I2C2EN;
#define PYGMY_RCC_I2C1_ENABLE               RCC->APB1ENR |= RCC_I2C1EN;
#define PYGMY_RCC_I2C1_DISABLE              RCC->APB1ENR &= ~RCC_I2C1EN;
#define PYGMY_RCC_SPI3_ENABLE               RCC->APB1ENR |= RCC_SPI3EN;
#define PYGMY_RCC_SPI3_DISABLE              RCC->APB1ENR &= ~RCC_SPI3EN;
#define PYGMY_RCC_SPI2_ENABLE               RCC->APB1ENR |= RCC_SPI2EN;
#define PYGMY_RCC_SPI2_DISABLE              RCC->APB1ENR &= ~RCC_SPI2EN;
#define PYGMY_RCC_WWDGEN_ENABLE             RCC->APB1ENR |= RCC_WWDGEN;
#define PYGMY_RCC_WWDGEN_DISABLE            RCC->APB1ENR &= ~RCC_WWDGEN;


//--------------------------------------------------------------------------------------------
//-----------------------------------------Additional GPIO------------------------------------
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((u8)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))

#define EVCR_PORTPINCONFIG_MASK     ((u16)0xFF80)
#define LSB_MASK                    ((u16)0xFFFF)
#define DBGAFR_POSITION_MASK        ((u32)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((u32)0xF8FFFFFF)
#define DBGAFR_LOCATION_MASK        ((u32)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((u32)0x00100000)

//---------------------------------------------------------------------------------------------
//-------------------------------Pygmy Data Integtrity Algorithm Defs--------------------------
#define PDIA_NEW   1
#define PDIA_ADD   2
#define PDIA_END   4

//---------------------------------------------------------------------------------------------
//-------------------------------------------Constants-----------------------------------------
extern const u32 PYGMY_BITMASKS[];
extern const u32 PYGMY_INVBITMASKS[];

//---------------------------------------------------------------------------------------------
//--------------------------------------------Globals------------------------------------------
extern PYGMYSYSTEM pygmyGlobalData;

//---------------------------------------------------------------------------------------------
//------------------------------------------Function Defs--------------------------------------

void print( u8 ucStream, u8 *ucBuffer, ... );
//void setPrintFile( PYGMYFILE *pygmyFile );
u8 putsUSART1FIFO( u8 *ucBuffer );
u8 putsUSART2FIFO( u8 *ucBuffer );
u8 putsUSART3FIFO( u8 *ucBuffer );
u8 putsUSART4FIFO( u8 *ucBuffer );
u8 putsUSART5FIFO( u8 *ucBuffer );
u8 putsFILE( u8 *ucBuffer );
u8 putsLCD( u8 *ucBuffer );

void clearRTCFlags( void );

u8 sysInit( void );
u16 taskInit( void );
u16 msgInit( void );

void streamInit( void );
u8 streamReset( u8 ucStream );
void streamResetRX( u8 ucStream );
void streamResetTX( u8 ucStream );
void streamTXChar( u8 ucStream, void *pygmyUSART );
u8 streamGetChar( u8 ucStream );
u8 streamPutChar( u8 ucStream, u8 ucChar );
u8 streamPopChar( u8 ucStream );
u8 streamPeekChar( u8 ucStream );
void streamPushChar( u8 ucStream, u8 ucChar );
u8 streamSetPut( u8 ucStream, void *ptrFunc );
u8 streamSetGet( u8 ucStream, void *ptrFunc );
u8 streamSetRXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen );
u8 streamSetTXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen );
void streamSetPrintFile( void *pygmyFile );


void cmdProcess( PYGMYCOMMANDQUEUE *pygmyCmdQueue );
void cmdDelete( PYGMYCOMMANDQUEUE *pygmyCmdQueue, u16 uiIndex );
void cmdPrint( PYGMYCOMMANDQUEUE *pygmyCmdQueue );
u8 cmdIsQueued( PYGMYCOMMANDQUEUE *pygmyCmdQueue, u8 *ucName  );
u8 cmdQueue( PYGMYCOMMANDQUEUE *pygmyCmdQueue, PYGMYCOMMAND *pygmyCmd );
void cmdInitQueue( PYGMYCOMMANDQUEUE *pygmyCmdQueue );
void cmdReplace( PYGMYCOMMANDQUEUE *pygmyCmdQueue, PYGMYCOMMAND *pygmyCmd );

u16 msgGetIndex( u8 *ucName, u16 uiID );
u8 msgSend( u8 *ucName, u16 uiID, u8 *ucMessage, u16 uiValue );
u8 msgGet( u8 *ucName, u16 uiID, PYGMYMESSAGE *pygmyMsg );
u8 msgIs( u8 *ucName, u16 uiID );
u8 msgDelete( u8 *ucName, u16 uiID );
u16 msgInit( void );
void msgList( PYGMYMESSAGE *pygmyMsg, u16 uiMsg );

u16 taskNew( u8 *ucName, u16 uiID, u32 ulTimer, u32 ulReload, u32 ulExpire, PYGMYFUNCPTR EventHandler );
u16 taskNewSimple( u8 *ucName, u32 ulTimer, PYGMYFUNCPTR EventHandler );
u8 taskDelete( u8 *ucName, u16 uiID );
void taskList( PYGMYTASK *pygmyTask, u16 uiTask );
u8 taskGet( u8 *ucName, u16 uiID, PYGMYTASK *pygmyTask );
u8 taskIsRunning( u8 *ucName, u16 uiID );

void TaskException_Handler( void );

u8 sysCRC8( u8 *ucBuffer, u16 uiLen );
u16 sysCRC16( u8 *ucBuffer, u16 uiLen );
u8 pdiaEncode( u8 ucByte, u8 ucMode, u32 *ulSum );
void pdiaPrintInteger( u8 ucMode, u32 *ulSum, u8 ucStream, u8 *ucFormat, u32 ulData );
void pdiaPrintString( u8 ucMode, u32 *ulSum, u8 ucStream, u8 *ucBuffer );

void DriverThread_RestoreThreads( void );
void DriverThread_KillThreads( void );

u32 descriptorGetMainClock( void );
u32 descriptorGetXTAL( void );
u32 descriptorGetID( void );

u8 *getIDString( void );
u8 *getID( void );
u32 getIDCode( void );
u32 getIDRevision( void );

/*void setTime( u32 ulTime );
u32 getTime( void );
u32 convertSystemTimeToSeconds( PYGMYTIME *pygmyTime );
void convertSecondsToSystemTime( u32 ulSeconds, PYGMYTIME *pygmyTime );
*/

u8 executeCmd( u8 *ucBuffer, PYGMYCMD *pygmyCmds );

void enableXTAL( void );
void delay( u32 ulDelay );
void mcoEnable( u8 ucSource );
void mcoDisable( void );
void stopwatchStart( void );
u32 stopwatchGet( void );


//--------------------------------------------------------------------------------------
//--------------------------------------Deprecated--------------------------------------


