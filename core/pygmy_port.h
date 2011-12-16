/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

    This file is part of PygmyOS.

    PygmyOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PygmyOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PygmyOS.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/


#pragma once
    
#include "pygmy_sys.h" 

//STM32 Timers
// TIMX CR1
#define TIM_CKD_CLEAR               (BIT9|BIT8)                      
#define TIM_CKD_M0DE0               0                       
#define TIM_CKD_MODE1               BIT8                      
#define TIM_CKD_MODE2               BIT9                        
#define TIM_ARPE                    BIT7                      
#define TIM_CMS_CLEAR               (BIT6|BIT5)                
#define TIM_CMS_MODE0               0                      
#define TIM_CMS_MODE1               BIT5                     
#define TIM_CMS_MODE2               BIT6                     
#define TIM_CMS_MODE3               (BIT6|BIT5)             
#define TIM_DIR                     BIT4                        
#define TIM_OPM                     BIT3                        
#define TIM_URS                     BIT2                          
#define TIM_UDIS                    BIT1                         
#define TIM_CEN                     BIT0                        
// TIMX CR2
#define TIM_TI1S                    BIT7                      
#define TIM_MMS_CLEAR               (BIT6|BIT5|BIT4)               
#define TIM_MMS_MODE0               0                            
#define TIM_MMS_MODE1               BIT4                       
#define TIM_MMS_MODE2               BIT5                         
#define TIM_MMS_MODE3               (BIT5|BIT4)              
#define TIM_MMS_MODE4               BIT6                    
#define TIM_MMS_MODE5               (BIT6|BIT4)                
#define TIM_MMS_MODE6               (BIT6|BIT5)               
#define TIM_MMS_MODE7               (BIT6|BIT5|BIT4)        
#define TIM_CCDS                    BIT3                     
// TIMX_SMCR Slave Mode Control       
#define TIM_ETP                     BIT15                      
#define TIM_ECE                     BIT14                    
#define TIM_ETPS_CLEAR              (BIT13|BIT12)            
#define TIM_ETPS_DIV1               0                             
#define TIM_ETPS_DIV2               BIT12                         
#define TIM_ETPS_DIV4               BIT13                          
#define TIM_ETPS_DIV8               (BIT13|BIT12)                 
#define TIM_ETF_CLEAR               (BIT11|BIT10|BIT9|BIT8)     
#define TIM_ETF_MODE0               0                               
#define TIM_ETF_MODE1               BIT8                           
#define TIM_ETF_MODE2               BIT9                          
#define TIM_ETF_MODE3               (BIT9|BIT8)                  
#define TIM_ETF_MODE4               BIT10                      
#define TIM_ETF_MODE5               (BIT10|BIT8)                 
#define TIM_ETF_MODE6               (BIT10|BIT9)                
#define TIM_ETF_MODE7               (BIT10|BIT9|BIT8)           
#define TIM_ETF_MODE8               BIT11                      
#define TIM_ETF_MODE9               (BIT11|BIT8)               
#define TIM_ETF_MODE10              (BIT11|BIT9)               
#define TIM_ETF_MODE11              (BIT11|BIT9|BIT8)          
#define TIM_ETF_MODE12              (BIT11|BIT10)                
#define TIM_ETF_MODE13              (BIT11|BIT10|BIT8)          
#define TIM_ETF_MODE14              (BIT11|BIT10|BIT9)           
#define TIM_ETF_MODE15              (BIT11|BIT10|BIT9|BIT8)    
#define TIM_MSM                     BIT7                        
#define TIM_TS_CLEAR                (BIT6|BIT5|BIT4)              
#define TIM_TS_MODE0                0                            
#define TIM_TS_MODE1                BIT4                         
#define TIM_TS_MODE2                BIT5                        
#define TIM_TS_MODE3                (BIT5|BIT4)                  
#define TIM_TS_MODE4                BIT6                           
#define TIM_TS_MODE5                (BIT6|BIT4)                 
#define TIM_TS_MODE6                (BIT6|BIT5)                   
#define TIM_TS_MODE7                (BIT6|BIT5|BIT4)              
#define TIM_SMS_CLEAR               (BIT2|BIT1|BIT0)                
#define TIM_SMS_MODE0               0                               
#define TIM_SMS_MODE1               BIT0                          
#define TIM_SMS_MODE2               BIT1                           
#define TIM_SMS_MODE3               (BIT1|BIT0)                    
#define TIM_SMS_MODE4               BIT2                          
#define TIM_SMS_MODE5               (BIT2|BIT0)                 
#define TIM_SMS_MODE6               (BIT2|BIT1)                    
#define TIM_SMS_MODE7               (BIT2|BIT1|BIT0)                
// TIMX_DIER DMA Interrupt Enable Register
#define TIM_TDE                     BIT14                         
#define TIM_CC4DE                   BIT12                       
#define TIM_CC3DE                   BIT11                          
#define TIM_CC2DE                   BIT10                          
#define TIM_CC1DE                   BIT9                          
#define TIM_UDE                     BIT8                         
#define TIM_TIE                     BIT6                           
#define TIM_CC4IE                   BIT4                         
#define TIM_CC3IE                   BIT3                           
#define TIM_CC2IE                   BIT2                           
#define TIM_CC1IE                   BIT1                            
#define TIM_UIE                     BIT0                          
// TIMX_SR Status Register
#define TIM_CC4OF                   BIT12                      
#define TIM_CC3OF                   BIT11                        
#define TIM_CC2OF                   BIT10                         
#define TIM_CC1OF                   BIT9                         
#define TIM_TIF                     BIT6                          
#define TIM_CC4IF                   BIT4                        
#define TIM_CC3IF                   BIT3                         
#define TIM_CC2IF                   BIT2                           
#define TIM_CC1IF                   BIT1                           
#define TIM_UIF                     BIT0                           
// TIMX_EGR Event Generation Register
#define TIM_TG                      BIT6                          
#define TIM_CC4G                    BIT4                            
#define TIM_CC3G                    BIT3                           
#define TIM_CC2G                    BIT2                           
#define TIM_CC1G                    BIT1                           
#define TIM_UG                      BIT0                           
// TIMX_CCMR1 Capture Compare Mode Register 1
#define TIM_OC2CE                   BIT15                         
#define TIM_OC2M_CLEAR              (BIT14|BIT13|BIT12)
#define TIM_OC2M_MODE0              0
#define TIM_OC2M_MODE1              BIT12
#define TIM_OC2M_MODE2              BIT13
#define TIM_OC2M_MODE3              (BIT13|BIT12)
#define TIM_OC2M_MODE4              BIT14
#define TIM_OC2M_MODE5              (BIT14|BIT12)
#define TIM_OC2M_MODE6              (BIT14|BIT13)
#define TIM_OC2M_MODE7              (BIT14|BIT13|BIT12)
#define TIM_OC2M_PWM1               TIM_OC2M_MODE6
#define TIM_OC2M_PWM2               TIM_OC2M_MODE7
#define TIM_OC2PE                   BIT11                          
#define TIM_OC2FE                   BIT10                           
#define TIM_CC2S_CLEAR              (BIT9|BIT8)                     
#define TIM_CC2S_MODE0              0                                
#define TIM_CC2S_MODE1              BIT8                           
#define TIM_CC2S_MODE2              BIT9                            
#define TIM_CC2S_MODE3              (BIT9|BIT8)                      
#define TIM_OC1CE                   BIT7                            
#define TIM_OC1M_CLEAR              (BIT6|BIT5|BIT4)               
#define TIM_OC1M_MODE0              0                             
#define TIM_OC1M_MODE1              BIT4                            
#define TIM_OC1M_MODE2              BIT5                            
#define TIM_OC1M_MODE3              (BIT5|BIT4)                   
#define TIM_OC1M_MODE4              BIT6                             
#define TIM_OC1M_MODE5              (BIT6|BIT4)                     
#define TIM_OC1M_MODE6              (BIT6|BIT5)                     
#define TIM_OC1M_MODE7              (BIT6|BIT5|BIT4)
#define TIM_OC1M_PWM1               TIM_OC1M_MODE6
#define TIM_OC1M_PWM2               TIM_OC1M_MODE7
#define TIM_OC1PE                   BIT3                           
#define TIM_OC1FE                   BIT2                            
#define TIM_CC1S_CLEAR              (BIT1|BIT0)                     
#define TIM_CC1S_MODE0              0                               
#define TIM_CC1S_MODE1              BIT0                           
#define TIM_CC1S_MODE2              BIT1                             
#define TIM_CC1S_MODE3              BIT2                            
// This register has dual purpose bits, the following are the alternate defs 
#define TIM_IC2F_CLEAR              (BIT15|BIT14|BIT13|BIT12)         
#define TIM_IC2PSC_CLEAR            (BIT11|BIT10)                    
#define TIM_IC1F_CLEAR              (BIT7|BIT6|BIT5|BIT4)            
#define TIM_IC1F_MODE0              0                                 
#define TIM_IC1F_MODE1              BIT4                            
#define TIM_IC1F_MODE2              BIT5                             
#define TIM_IC1F_MODE3              (BIT5|BIT4)                       
#define TIM_IC1F_MODE4              BIT6                             
#define TIM_IC1F_MODE5              (BIT6|BIT4)                       
#define TIM_IC1F_MODE6              (BIT6|BIT5)                       
#define TIM_IC1F_MODE7              (BIT6|BIT5|BIT4)                 
#define TIM_IC1F_MODE8              BIT7                              
#define TIM_IC1F_MODE9              (BIT7|BIT4)                       
#define TIM_IC1F_MODE10             (BIT7|BIT5)                       
#define TIM_IC1F_MODE11             (BIT7|BIT5|BIT4)                  
#define TIM_IC1F_MODE12             (BIT7|BIT6)                     
#define TIM_IC1F_MODE13             (BIT7|BIT6|BIT4)                  
#define TIM_IC1F_MODE14             (BIT7|BIT6|BIT5)                  
#define TIM_IC1F_MODE15             (BIT7|BIT6|BIT5|BIT4)             
#define TIM_IC1PSC_CLEAR            (BIT3|BIT2)                      
#define TIM_IC1PSC_MODE0            0                                
#define TIM_IC1PSC_MODE1            BIT2                             
#define TIM_IC1PSC_MODE2            BIT3                             
#define TIM_IC1PSC_MODE3            (BIT3|BIT2)                       
// TIMX_CCMR2
#define TIM_OC4CE                   BIT15                             
#define TIM_OC4M_CLEAR              (BIT14|BIT13|BIT12)  
#define TIM_OC4M_MODE0              0
#define TIM_OC4M_MODE1              BIT12
#define TIM_OC4M_MODE2              BIT13
#define TIM_OC4M_MODE3              (BIT13|BIT12)
#define TIM_OC4M_MODE4              BIT14
#define TIM_OC4M_MODE5              (BIT14|BIT12)
#define TIM_OC4M_MODE6              (BIT14|BIT13)
#define TIM_OC4M_MODE7              (BIT14|BIT13|BIT12)
#define TIM_OC4M_PWM1               TIM_OC4M_MODE6
#define TIM_OC4M_PWM2               TIM_OC4M_MODE7
#define TIM_OC4PE                   BIT11                             
#define TIM_OC4FE                   BIT10                             
#define TIM_CC4S_CLEAR              (BIT9|BIT8)                      
#define TIM_CC4S_MODE0              0                                
#define TIM_CC4S_MODE1              BIT8                            
#define TIM_CC4S_MODE2              BIT9                           
#define TIM_CC4S_MODE3              (BIT9|BIT8)                       
#define TIM_OC3CE                   BIT7                              
#define TIM_OC3M_CLEAR              (BIT6|BIT5|BIT4)   
#define TIM_OC3M_MODE0              0
#define TIM_OC3M_MODE1              BIT4
#define TIM_OC3M_MODE2              BIT5
#define TIM_OC3M_MODE3              (BIT5|BIT4)
#define TIM_OC3M_MODE4              BIT6
#define TIM_OC3M_MODE5              (BIT6|BIT4)
#define TIM_OC3M_MODE6              (BIT6|BIT5)
#define TIM_OC3M_MODE7              (BIT6|BIT5|BIT4)
#define TIM_OC3M_PWM1               TIM_OC3M_MODE6
#define TIM_OC3M_PWM2               TIM_OC3M_MODE7
#define TIM_OC3PE                   BIT3                            
#define TIM_OC3FE                   BIT2                            
#define TIM_CC3S_CLEAR              (BIT1|BIT0)                    
#define TIM_CC3S_MODE0              0                                 
#define TIM_CC3S_MODE1              BIT0                             
#define TIM_CC3S_MODE2              BIT1                             
#define TIM_CC3S_MODE3              (BIT1|BIT0)                     
// This register has dual purpose bits, the following are the alternate defs for Input Capture
#define TIM_IC4F_CLEAR              (BIT15|BIT14|BIT13|BIT12)         
#define TIM_IC4PSC_CLEAR            (BIT11|BIT10)                     
#define TIM_IC3F_CLEAR              (BIT7|BIT6|BIT5|BIT4)             
#define TIM_IC3PSC_CLEAR            (BIT3|BIT2)                       
// TIMX_CCER Capture Compare Enable Register
#define TIM_CC4P                    BIT13                          
#define TIM_CC4E                    BIT12                          
#define TIM_CC3P                    BIT9                            
#define TIM_CC3E                    BIT8                              
#define TIM_CC2P                    BIT5                              
#define TIM_CC2E                    BIT4                             
#define TIM_CC1P                    BIT1                             
#define TIM_CC1E                    BIT0                          
// TIMX_DCR DMA Control Register
#define TIM_DBL_CLEAR               (BIT12|BIT11|BIT10|BIT9|BIT8)     
#define TIM_DBL_0                   0                                
#define TIM_DBL_1                   0                                 
#define TIM_DBL_2                   BIT8                              
#define TIM_DBL_3                   BIT9                              
#define TIM_DBL_4                   (BIT9|BIT8)                       
#define TIM_DBL_5                   BIT10                           
#define TIM_DBL_6                   (BIT10|BIT8)                     
#define TIM_DBL_7                   (BIT10|BIT9)                     
#define TIM_DBL_8                   (BIT10|BIT9|BIT8)                
#define TIM_DBL_9                   BIT11                             
#define TIM_DBL_10                  (BIT11|BIT8)                      
#define TIM_DBL_11                  (BIT11|BIT9)                      
#define TIM_DBL_12                  (BIT11|BIT9|BIT8)                 
#define TIM_DBL_13                  (BIT11|BIT10)                     
#define TIM_DBL_14                  (BIT11|BIT10|BIT8)                
#define TIM_DBL_15                  (BIT11|BIT10|BIT9)                
#define TIM_DBL_16                  (BIT11|BIT10|BIT9|BIT8)           
#define TIM_DBL_17                  BIT12                             
#define TIM_DBL_18                  (BIT12|BIT8)                      
#define TIM_DBA_CLEAR               (BIT4|BIT3|BIT2|BIT1|BIT0)        
#define TIM_DBA_0                   0 // Default is TIM_CR1, all values are bytes offset from TIM_CR1
#define TIM_DBA_1                   BIT0                            
#define TIM_DBA_2                   BIT1                             
#define TIM_DBA_3                   (BIT1|BIT0)                       
#define TIM_DBA_4                   BIT2                            
#define TIM_DBA_5                   (BIT2|BIT0)                       
#define TIM_DBA_6                   (BIT2|BIT1)                       
#define TIM_DBA_7                   (BIT2|BIT1|BIT0)                  
#define TIM_DBA_8                   BIT3                             
#define TIM_DBA_9                   (BIT3|BIT0)                      
#define TIM_DBA_10                  (BIT3|BIT1)                       
#define TIM_DBA_11                  (BIT3|BIT1|BIT0)                  
#define TIM_DBA_12                  (BIT3|BIT2)                      
#define TIM_DBA_13                  (BIT3|BIT2|BIT0)                 
#define TIM_DBA_14                  (BIT3|BIT2|BIT1)                 
#define TIM_DBA_15                  (BIT3|BIT2|BIT1|BIT0)            
#define TIM_DBA_16                  BIT4                            
#define TIM_DBA_17                  (BIT4|BIT0)                       
#define TIM_DBA_18                  (BIT4|BIT1)                       
#define TIM_DBA_19                  (BIT4|BIT1|BIT0)                
#define TIM_DBA_20                  (BIT4|BIT2)                       
#define TIM_DBA_21                  (BIT4|BIT2|BIT0)                 
#define TIM_DBA_22                  (BIT4|BIT2|BIT1)                 
#define TIM_DBA_23                  (BIT4|BIT2|BIT1|BIT0)             
#define TIM_DBA_24                  (BIT4|BIT3)                      
#define TIM_DBA_25                  (BIT4|BIT3|BIT0)                 
#define TIM_DBA_26                  (BIT4|BIT3|BIT1)                 
#define TIM_DBA_27                  (BIT4|BIT3|BIT1|BIT0)            
#define TIM_DBA_28                  (BIT4|BIT3|BIT2)                 
#define TIM_DBA_29                  (BIT4|BIT3|BIT2|BIT0)             
#define TIM_DBA_30                  (BIT4|BIT3|BIT2|BIT1)             
#define TIM_DBA_31                  (BIT4|BIT3|BIT2|BIT1|BIT0)        
// Timer1 BDTR Register
#define TIM_MOE                     BIT15 // !!! Master Output Enable !!!
#define TIM_AOE                     BIT14 // !!! Automatic Update Enable !!!
#define TIM_BKP                     BIT13 // Break Polarity
#define TIM_BKE                     BIT12 // Break Enable
#define TIM_OSSR                    BIT11 // Off state selection, run
#define TIM_OSSI                    BIT10 // Off state selection, idle
#define TIM_LOCK_CLEAR              (BIT9|BIT8)
#define TIM_LOCK_OFF                0

typedef struct {
                vu32 POWER;
                vu32 CLKCR;
                vu32 ARG;
                vu32 CMD;
                vuc32 RESPCMD;
                vuc32 RESP1;
                vuc32 RESP2;
                vuc32 RESP3;
                vuc32 RESP4;
                vu32 DTIMER;
                vu32 DLEN;
                vu32 DCTRL;
                vuc32 DCOUNT;
                vuc32 STA;
                vu32 ICR;
                vu32 MASK;
                u32  RESERVED0[2];
                vuc32 FIFOCNT;
                u32  RESERVED1[13];
                vu32 FIFO;
                } SDIO_TYPEDEF;

typedef struct {
                vu16 CR1;
                u16 RESERVED0;
                vu16 CR2;
                u16 RESERVED1;
                vu16 SMCR;
                u16 RESERVED2;
                vu16 DIER;
                u16 RESERVED3;
                vu16 SR;
                u16 RESERVED4;
                vu16 EGR;
                u16 RESERVED5;
                vu16 CCMR1;
                u16 RESERVED6;
                vu16 CCMR2;
                u16 RESERVED7;
                vu16 CCER;
                u16 RESERVED8;
                vu16 CNT;
                u16 RESERVED9;
                vu16 PSC;
                u16 RESERVED10;
                vu16 ARR;
                u16 RESERVED11;
                vu16 RCR;
                u16 RESERVED12;
                vu16 CCR1;
                u16 RESERVED13;
                vu16 CCR2;
                u16 RESERVED14;
                vu16 CCR3;
                u16 RESERVED15;
                vu16 CCR4;
                u16 RESERVED16;
                vu16 BDTR;
                u16 RESERVED17;
                vu16 DCR;
                u16 RESERVED18;
                vu16 DMAR;
                u16 RESERVED19;
                } TIM1_TYPEDEF;

typedef struct {
                vu16 CR1;
                u16 RESERVED0;
                vu16 CR2;
                u16 RESERVED1;
                vu16 SMCR;
                u16 RESERVED2;
                vu16 DIER;
                u16 RESERVED3;
                vu16 SR;
                u16 RESERVED4;
                vu16 EGR;
                u16 RESERVED5;
                vu16 CCMR1;
                u16 RESERVED6;
                vu16 CCMR2;
                u16 RESERVED7;
                vu16 CCER;
                u16 RESERVED8;
                vu16 CNT;
                u16 RESERVED9;
                vu16 PSC;
                u16 RESERVED10;
                vu16 ARR;
                u16 RESERVED11[3];
                vu16 CCR1;
                u16 RESERVED12;
                vu16 CCR2;
                u16 RESERVED13;
                vu16 CCR3;
                u16 RESERVED14;
                vu16 CCR4;
                u16 RESERVED15[3];
                vu16 DCR;
                u16 RESERVED16;
                vu16 DMAR;
                u16 RESERVED17;
                } TIM_TYPEDEF;

#define TIMER   TIM_TYPEDEF
#define TIMER1  TIM1_TYPEDEF
            
typedef struct {
                vu32 CRL;
                vu32 CRH;
                vu32 IDR;
                vu32 ODR;
                vu32 BSRR;
                vu32 BRR;
                vu32 LCKR;
                } GPIO;

#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)

#define GPIOA               ((GPIO *) GPIOA_BASE)
#define GPIOB               ((GPIO *) GPIOB_BASE)
#define GPIOC               ((GPIO *) GPIOC_BASE)
#define GPIOD               ((GPIO *) GPIOD_BASE)
#define GPIOE               ((GPIO *) GPIOE_BASE)
    
#define ALLPINS_CLEAR                       0xFFFFFFFF                // Only use before setting bits individually
#define ALLPINS_IN_ANALOG                   0x00000000
#define ALLPINS_IN_FLOAT                    0x44444444
#define ALLPINS_IN_PULL                     0x88888888
#define ALLPINS_OUT10_GPPUSHPULL            0x11111111
#define ALLPINS_OUT10_OPENDRAIN             0x55555555
#define ALLPINS_OUT10_ALTPUSHPULL           0x99999999
#define ALLPINS_OUT2_GPPUSHPULL             0x22222222
#define ALLPINS_OUT2_GPOPENDRAIN            0x66666666
#define ALLPINS_OUT2_ALTPUSHPULL            0xAAAAAAAA
#define ALLPINS_OUT2_ALTOPENDRAIN           0xEEEEEEEE
#define ALLPINS_OUT50_GPPUSHPULL            0x33333333
#define ALLPINS_OUT50_GPOPENDRAIN           0x77777777
#define ALLPINS_OUT50_ALTPUSHPULL           0xBBBBBBBB
#define ALLPINS_OUT50_ALTOPENDRAIN          0xFFFFFFFF
#define PIN0_CLEAR                          (BIT3|BIT2|BIT1|BIT0)     // Clear before setting
#define PIN0_IN_ANALOG                      0                         // Clear for Analog
#define PIN0_IN_FLOAT                       (BIT2)
#define PIN0_IN_PULL                        (BIT3)
#define PIN0_OUT10_GPPUSHPULL               (BIT0)                    // Mode bit
#define PIN0_OUT10_GPOPENDRAIN              (BIT2|BIT0)
#define PIN0_OUT10_ALTPUSHPULL              (BIT3|BIT0)
#define PIN0_OUT10_ALTOPENDRAIN             (BIT3|BIT2|BIT0)
#define PIN0_OUT2_GPPUSHPULL                (BIT1)                    // Mode bit
#define PIN0_OUT2_GPOPENDRAIN               (BIT2|BIT1)
#define PIN0_OUT2_ALTPUSHPULL               (BIT3|BIT1)
#define PIN0_OUT2_ALTOPENDRAIN              (BIT3|BIT2|BIT1)
#define PIN0_OUT50_GPPUSHPULL               (BIT1|BIT0)               // Mode Bits
#define PIN0_OUT50_GPOPENDRAIN              (BIT2|BIT1|BIT0)
#define PIN0_OUT50_ALTPUSHPULL              (BIT3|BIT1|BIT0)
#define PIN0_OUT50_ALTOPENDRAIN             (BIT3|BIT2|BIT1|BIT0)
// Pin1
#define PIN1_CLEAR                          (BIT7|BIT6|BIT5|BIT4)     // Clear before setting
#define PIN1_IN_ANALOG                      0                         // Clear for Analog
#define PIN1_IN_FLOAT                       (BIT6)
#define PIN1_IN_PULL                        (BIT7)
#define PIN1_OUT10_GPPUSHPULL               (BIT4)                    // Mode bit
#define PIN1_OUT10_GPOPENDRAIN              (BIT6|BIT4)
#define PIN1_OUT10_ALTPUSHPULL              (BIT7|BIT4)
#define PIN1_OUT10_ALTOPENDRAIN             (BIT7|BIT6|BIT4)
#define PIN1_OUT2_GPPUSHPULL                (BIT5)                    // Mode bit
#define PIN1_OUT2_GPOPENDRAIN               (BIT6|BIT5)
#define PIN1_OUT2_ALTPUSHPULL               (BIT7|BIT5)
#define PIN1_OUT2_ALTOPENDRAIN              (BIT7|BIT6|BIT5)
#define PIN1_OUT50_GPPUSHPULL               (BIT5|BIT4)               // Mode Bits
#define PIN1_OUT50_GPOPENDRAIN              (BIT6|BIT5|BIT4)
#define PIN1_OUT50_ALTPUSHPULL              (BIT7|BIT5|BIT4)
#define PIN1_OUT50_ALTOPENDRAIN             (BIT7|BIT6|BIT5|BIT4)
// Pin2                                   
#define PIN2_CLEAR                          (BIT11|BIT10|BIT9|BIT8)      // Clear before setting
#define PIN2_IN_ANALOG                      0                       // Clear for Analog
#define PIN2_IN_FLOAT                       (BIT10)
#define PIN2_IN_PULL                        (BIT11)
#define PIN2_OUT10_GPPUSHPULL               (BIT8)                    // Mode bit
#define PIN2_OUT10_GPOPENDRAIN              (BIT10|BIT8)
#define PIN2_OUT10_ALTPUSHPULL              (BIT11|BIT8)
#define PIN2_OUT10_ALTOPENDRAIN             (BIT11|BIT10|BIT8)
#define PIN2_OUT2_GPPUSHPULL                (BIT9)                    // Mode bit
#define PIN2_OUT2_GPOPENDRAIN               (BIT10|BIT9)
#define PIN2_OUT2_ALTPUSHPULL               (BIT11|BIT9)
#define PIN2_OUT2_ALTOPENDRAIN              (BIT11|BIT10|BIT9)
#define PIN2_OUT50_GPPUSHPULL               (BIT9|BIT8)               // Mode Bits
#define PIN2_OUT50_GPOPENDRAIN              (BIT10|BIT9|BIT8)
#define PIN2_OUT50_ALTPUSHPULL              (BIT11|BIT9|BIT8)
#define PIN2_OUT50_ALTOPENDRAIN             (BIT11|BIT10|BIT9|BIT8)
// Pin3                                   
#define PIN3_CLEAR                          (BIT15|BIT14|BIT13|BIT12)      // Clear before setting
#define PIN3_IN_ANALOG                      0                       // Clear for Analog
#define PIN3_IN_FLOAT                       (BIT14)
#define PIN3_IN_PULL                        (BIT15)
#define PIN3_OUT10_GPPUSHPULL               (BIT12)                    // Mode bit
#define PIN3_OUT10_GPOPENDRAIN              (BIT14|BIT12)
#define PIN3_OUT10_ALTPUSHPULL              (BIT15|BIT12)
#define PIN3_OUT10_ALTOPENDRAIN             (BIT15|BIT14|BIT12)
#define PIN3_OUT2_GPPUSHPULL                (BIT13)                    // Mode bit
#define PIN3_OUT2_GPOPENDRAIN               (BIT14|BIT13)
#define PIN3_OUT2_ALTPUSHPULL               (BIT15|BIT13)
#define PIN3_OUT2_ALTOPENDRAIN              (BIT15|BIT14|BIT13)
#define PIN3_OUT50_GPPUSHPULL               (BIT13|BIT12)               // Mode Bits
#define PIN3_OUT50_GPOPENDRAIN              (BIT14|BIT13|BIT12)
#define PIN3_OUT50_ALTPUSHPULL              (BIT15|BIT13|BIT12)
#define PIN3_OUT50_ALTOPENDRAIN             (BIT15|BIT14|BIT1|BIT12)
// Pin4                                   
#define PIN4_CLEAR                          (BIT19|BIT18|BIT17|BIT16)      // Clear before setting
#define PIN4_IN_ANALOG                      0                       // Clear for Analog
#define PIN4_IN_FLOAT                       (BIT18)
#define PIN4_IN_PULL                        (BIT19)
#define PIN4_OUT10_GPPUSHPULL               (BIT16)                    // Mode bit
#define PIN4_OUT10_GPOPENDRAIN              (BIT18|BIT16)
#define PIN4_OUT10_ALTPUSHPULL              (BIT19|BIT16)
#define PIN4_OUT10_ALTOPENDRAIN             (BIT19|BIT18|BIT16)
#define PIN4_OUT2_GPPUSHPULL                (BIT17)                    // Mode bit
#define PIN4_OUT2_GPOPENDRAIN               (BIT18|BIT17)
#define PIN4_OUT2_ALTPUSHPULL               (BIT19|BIT17)
#define PIN4_OUT2_ALTOPENDRAIN              (BIT19|BIT18|BIT17)
#define PIN4_OUT50_GPPUSHPULL               (BIT17|BIT16)               // Mode Bits
#define PIN4_OUT50_GPOPENDRAIN              (BIT18|BIT17|BIT16)
#define PIN4_OUT50_ALTPUSHPULL              (BIT19|BIT17|BIT16)
#define PIN4_OUT50_ALTOPENDRAIN             (BIT19|BIT18|BIT17|BIT16)
// Pin5                                  
#define PIN5_CLEAR                          (BIT23|BIT22|BIT21|BIT20)      // Clear before setting
#define PIN5_IN_ANALOG                      0                       // Clear for Analog
#define PIN5_IN_FLOAT                       (BIT22)
#define PIN5_IN_PULL                        (BIT23)
#define PIN5_OUT10_GPPUSHPULL               (BIT20)                    // Mode bit
#define PIN5_OUT10_GPOPENDRAIN              (BIT22|BIT20)
#define PIN5_OUT10_ALTPUSHPULL              (BIT23|BIT20)
#define PIN5_OUT10_ALTOPENDRAIN             (BIT23|BIT2|BIT20)
#define PIN5_OUT2_GPPUSHPULL                (BIT21)                    // Mode bit
#define PIN5_OUT2_GPOPENDRAIN               (BIT22|BIT21)
#define PIN5_OUT2_ALTPUSHPULL               (BIT23|BIT21)
#define PIN5_OUT2_ALTOPENDRAIN              (BIT23|BIT22|BIT1)
#define PIN5_OUT50_GPPUSHPULL               (BIT21|BIT20)               // Mode Bits
#define PIN5_OUT50_GPOPENDRAIN              (BIT22|BIT21|BIT20)
#define PIN5_OUT50_ALTPUSHPULL              (BIT23|BIT21|BIT20)
#define PIN5_OUT50_ALTOPENDRAIN             (BIT23|BIT22|BIT21|BIT20)
// Pin6                                   
#define PIN6_CLEAR                          (BIT27|BIT26|BIT25|BIT24)      // Clear before setting
#define PIN6_IN_ANALOG                      0                       // Clear for Analog
#define PIN6_IN_FLOAT                       (BIT26)
#define PIN6_IN_PULL                        (BIT27)
#define PIN6_OUT10_GPPUSHPULL               (BIT24)                    // Mode bit
#define PIN6_OUT10_GPOPENDRAIN              (BIT26|BIT24)
#define PIN6_OUT10_ALTPUSHPULL              (BIT27|BIT24)
#define PIN6_OUT10_ALTOPENDRAIN             (BIT27|BIT26|BIT24)
#define PIN6_OUT2_GPPUSHPULL                (BIT25)                    // Mode bit
#define PIN6_OUT2_GPOPENDRAIN               (BIT26|BIT25)
#define PIN6_OUT2_ALTPUSHPULL               (BIT27|BIT25)
#define PIN6_OUT2_ALTOPENDRAIN              (BIT27|BIT26|BIT25)
#define PIN6_OUT50_GPPUSHPULL               (BIT25|BIT24)               // Mode Bits
#define PIN6_OUT50_GPOPENDRAIN              (BIT26|BIT25|BIT24)
#define PIN6_OUT50_ALTPUSHPULL              (BIT27|BIT25|BIT24)
#define PIN6_OUT50_ALTOPENDRAIN             (BIT27|BIT26|BIT25|BIT24)
// Pin7                                   
#define PIN7_CLEAR                          (BIT31|BIT30|BIT29|BIT28)      // Clear before setting
#define PIN7_IN_ANALOG                      0                       // Clear for Analog
#define PIN7_IN_FLOAT                       (BIT30)
#define PIN7_IN_PULL                        (BIT31)
#define PIN7_OUT10_GPPUSHPULL               (BIT28)                    // Mode bit
#define PIN7_OUT10_GPOPENDRAIN              (BIT30|BIT28)
#define PIN7_OUT10_ALTPUSHPULL              (BIT31|BIT28)
#define PIN7_OUT10_ALTOPENDRAIN             (BIT31|BIT30|BIT28)
#define PIN7_OUT2_GPPUSHPULL                (BIT29)                    // Mode bit
#define PIN7_OUT2_GPOPENDRAIN               (BIT30|BIT29)
#define PIN7_OUT2_ALTPUSHPULL               (BIT31|BIT29)
#define PIN7_OUT2_ALTOPENDRAIN              (BIT31|BIT30|BIT29)
#define PIN7_OUT50_GPPUSHPULL               (BIT29|BIT28)               // Mode Bits
#define PIN7_OUT50_GPOPENDRAIN              (BIT30|BIT29|BIT28)
#define PIN7_OUT50_ALTPUSHPULL              (BIT31|BIT29|BIT28)
#define PIN7_OUT50_ALTOPENDRAIN             (BIT31|BIT30|BIT29|BIT28)
// Pin8
#define PIN8_CLEAR                          PIN0_CLEAR
#define PIN8_IN_ANALOG                      PIN0_IN_ANALOG
#define PIN8_IN_FLOAT                       PIN0_IN_FLOAT
#define PIN8_IN_PULL                        PIN0_IN_PULL
#define PIN8_OUT10_GPPUSHPULL               PIN0_OUT10_GPPUSHPULL
#define PIN8_OUT10_GPOPENDRAIN              PIN0_OUT10_GPOPENDRAIN
#define PIN8_OUT10_ALTPUSHPULL              PIN0_OUT10_ALTPUSHPULL
#define PIN8_OUT10_ALTOPENDRAIN             PIN0_OUT10_ALTOPENDRAIN
#define PIN8_OUT2_GPPUSHPULL                PIN0_OUT2_GPPUSHPULL
#define PIN8_OUT2_GPOPENDRAIN               PIN0_OUT2_GPOPENDRAIN
#define PIN8_OUT2_ALTPUSHPULL               PIN0_OUT2_ALTPUSHPULL
#define PIN8_OUT2_ALTOPENDRAIN              PIN0_OUT2_ALTOPENDRAIN
#define PIN8_OUT50_GPPUSHPULL               PIN0_OUT50_GPPUSHPULL
#define PIN8_OUT50_GPOPENDRAIN              PIN0_OUT50_GPOPENDRAIN
#define PIN8_OUT50_ALTPUSHPULL              PIN0_OUT50_ALTPUSHPULL
#define PIN8_OUT50_ALTOPENDRAIN             PIN0_OUT50_ALTOPENDRAIN
// Pin9
#define PIN9_CLEAR                          PIN1_CLEAR
#define PIN9_IN_ANALOG                      PIN1_IN_ANALOG
#define PIN9_IN_FLOAT                       PIN1_IN_FLOAT
#define PIN9_IN_PULL                        PIN1_IN_PULL
#define PIN9_OUT10_GPPUSHPULL               PIN1_OUT10_GPPUSHPULL
#define PIN9_OUT10_GPOPENDRAIN              PIN1_OUT10_GPOPENDRAIN
#define PIN9_OUT10_ALTPUSHPULL              PIN1_OUT10_ALTPUSHPULL
#define PIN9_OUT10_ALTOPENDRAIN             PIN1_OUT10_ALTOPENDRAIN
#define PIN9_OUT2_GPPUSHPULL                PIN1_OUT2_GPPUSHPULL
#define PIN9_OUT2_GPOPENDRAIN               PIN1_OUT2_GPOPENDRAIN
#define PIN9_OUT2_ALTPUSHPULL               PIN1_OUT2_ALTPUSHPULL
#define PIN9_OUT2_ALTOPENDRAIN              PIN1_OUT2_ALTOPENDRAIN
#define PIN9_OUT50_GPPUSHPULL               PIN1_OUT50_GPPUSHPULL
#define PIN9_OUT50_GPOPENDRAIN              PIN1_OUT50_GPOPENDRAIN
#define PIN9_OUT50_ALTPUSHPULL              PIN1_OUT50_ALTPUSHPULL
#define PIN9_OUT50_ALTOPENDRAIN             PIN1_OUT50_ALTOPENDRAIN
// Pin10
#define PIN10_CLEAR                         PIN2_CLEAR
#define PIN10_IN_ANALOG                     PIN2_IN_ANALOG
#define PIN10_IN_FLOAT                      PIN2_IN_FLOAT
#define PIN10_IN_PULL                       PIN2_IN_PULL
#define PIN10_OUT10_GPPUSHPULL              PIN2_OUT10_GPPUSHPULL
#define PIN10_OUT10_GPOPENDRAIN             PIN2_OUT10_GPOPENDRAIN
#define PIN10_OUT10_ALTPUSHPULL             PIN2_OUT10_ALTPUSHPULL
#define PIN10_OUT10_ALTOPENDRAIN            PIN2_OUT10_ALTOPENDRAIN
#define PIN10_OUT2_GPPUSHPULL               PIN2_OUT2_GPPUSHPULL
#define PIN10_OUT2_GPOPENDRAIN              PIN2_OUT2_GPOPENDRAIN
#define PIN10_OUT2_ALTPUSHPULL              PIN2_OUT2_ALTPUSHPULL
#define PIN10_OUT2_ALTOPENDRAIN             PIN2_OUT2_ALTOPENDRAIN
#define PIN10_OUT50_GPPUSHPULL              PIN2_OUT50_GPPUSHPULL
#define PIN10_OUT50_GPOPENDRAIN             PIN2_OUT50_GPOPENDRAIN
#define PIN10_OUT50_ALTPUSHPULL             PIN2_OUT50_ALTPUSHPULL
#define PIN10_OUT50_ALTOPENDRAIN            PIN2_OUT50_ALTOPENDRAIN
// Pin11
#define PIN11_CLEAR                         PIN3_CLEAR
#define PIN11_IN_ANALOG                     PIN3_IN_ANALOG
#define PIN11_IN_FLOAT                      PIN3_IN_FLOAT
#define PIN11_IN_PULL                       PIN3_IN_PULL
#define PIN11_OUT10_GPPUSHPULL              PIN3_OUT10_GPPUSHPULL
#define PIN11_OUT10_GPOPENDRAIN             PIN3_OUT10_GPOPENDRAIN
#define PIN11_OUT10_ALTPUSHPULL             PIN3_OUT10_ALTPUSHPULL
#define PIN11_OUT10_ALTOPENDRAIN            PIN3_OUT10_ALTOPENDRAIN
#define PIN11_OUT2_GPPUSHPULL               PIN3_OUT2_GPPUSHPULL
#define PIN11_OUT2_GPOPENDRAIN              PIN3_OUT2_GPOPENDRAIN
#define PIN11_OUT2_ALTPUSHPULL              PIN3_OUT2_ALTPUSHPULL
#define PIN11_OUT2_ALTOPENDRAIN             PIN3_OUT2_ALTOPENDRAIN
#define PIN11_OUT50_GPPUSHPULL              PIN3_OUT50_GPPUSHPULL
#define PIN11_OUT50_GPOPENDRAIN             PIN3_OUT50_GPOPENDRAIN
#define PIN11_OUT50_ALTPUSHPULL             PIN3_OUT50_ALTPUSHPULL
#define PIN11_OUT50_ALTOPENDRAIN            PIN3_OUT50_ALTOPENDRAIN
// Pin12
#define PIN12_CLEAR                         PIN4_CLEAR
#define PIN12_IN_ANALOG                     PIN4_IN_ANALOG
#define PIN12_IN_FLOAT                      PIN4_IN_FLOAT
#define PIN12_IN_PULL                       PIN4_IN_PULL
#define PIN12_OUT10_GPPUSHPULL              PIN4_OUT10_GPPUSHPULL
#define PIN12_OUT10_GPOPENDRAIN             PIN4_OUT10_GPOPENDRAIN
#define PIN12_OUT10_ALTPUSHPULL             PIN4_OUT10_ALTPUSHPULL
#define PIN12_OUT10_ALTOPENDRAIN            PIN4_OUT10_ALTOPENDRAIN
#define PIN12_OUT2_GPPUSHPULL               PIN4_OUT2_GPPUSHPULL
#define PIN12_OUT2_GPOPENDRAIN              PIN4_OUT2_GPOPENDRAIN
#define PIN12_OUT2_ALTPUSHPULL              PIN4_OUT2_ALTPUSHPULL
#define PIN12_OUT2_ALTOPENDRAIN             PIN4_OUT2_ALTOPENDRAIN
#define PIN12_OUT50_GPPUSHPULL              PIN4_OUT50_GPPUSHPULL
#define PIN12_OUT50_GPOPENDRAIN             PIN4_OUT50_GPOPENDRAIN
#define PIN12_OUT50_ALTPUSHPULL             PIN4_OUT50_ALTPUSHPULL
#define PIN12_OUT50_ALTOPENDRAIN            PIN4_OUT50_ALTOPENDRAIN
// Pin13
#define PIN13_CLEAR                         PIN5_CLEAR
#define PIN13_IN_ANALOG                     PIN5_IN_ANALOG
#define PIN13_IN_FLOAT                      PIN5_IN_FLOAT
#define PIN13_IN_PULL                       PIN5_IN_PULL
#define PIN13_OUT10_GPPUSHPULL              PIN5_OUT10_GPPUSHPULL
#define PIN13_OUT10_GPOPENDRAIN             PIN5_OUT10_GPOPENDRAIN
#define PIN13_OUT10_ALTPUSHPULL             PIN5_OUT10_ALTPUSHPULL
#define PIN13_OUT10_ALTOPENDRAIN            PIN5_OUT10_ALTOPENDRAIN
#define PIN13_OUT2_GPPUSHPULL               PIN5_OUT2_GPPUSHPULL
#define PIN13_OUT2_GPOPENDRAIN              PIN5_OUT2_GPOPENDRAIN
#define PIN13_OUT2_ALTPUSHPULL              PIN5_OUT2_ALTPUSHPULL
#define PIN13_OUT2_ALTOPENDRAIN             PIN5_OUT2_ALTOPENDRAIN
#define PIN13_OUT50_GPPUSHPULL              PIN5_OUT50_GPPUSHPULL
#define PIN13_OUT50_GPOPENDRAIN             PIN5_OUT50_GPOPENDRAIN
#define PIN13_OUT50_ALTPUSHPULL             PIN5_OUT50_ALTPUSHPULL
#define PIN13_OUT50_ALTOPENDRAIN            PIN5_OUT50_ALTOPENDRAIN
// Pin14
#define PIN14_CLEAR                         PIN6_CLEAR
#define PIN14_IN_ANALOG                     PIN6_IN_ANALOG
#define PIN14_IN_FLOAT                      PIN6_IN_FLOAT
#define PIN14_IN_PULL                       PIN6_IN_PULL
#define PIN14_OUT10_GPPUSHPULL              PIN6_OUT10_GPPUSHPULL
#define PIN14_OUT10_GPOPENDRAIN             PIN6_OUT10_GPOPENDRAIN
#define PIN14_OUT10_ALTPUSHPULL             PIN6_OUT10_ALTPUSHPULL
#define PIN14_OUT10_ALTOPENDRAIN            PIN6_OUT10_ALTOPENDRAIN
#define PIN14_OUT2_GPPUSHPULL               PIN6_OUT2_GPPUSHPULL
#define PIN14_OUT2_GPOPENDRAIN              PIN6_OUT2_GPOPENDRAIN
#define PIN14_OUT2_ALTPUSHPULL              PIN6_OUT2_ALTPUSHPULL
#define PIN14_OUT2_ALTOPENDRAIN             PIN6_OUT2_ALTOPENDRAIN
#define PIN14_OUT50_GPPUSHPULL              PIN6_OUT50_GPPUSHPULL
#define PIN14_OUT50_GPOPENDRAIN             PIN6_OUT50_GPOPENDRAIN
#define PIN14_OUT50_ALTPUSHPULL             PIN6_OUT50_ALTPUSHPULL
#define PIN14_OUT50_ALTOPENDRAIN            PIN6_OUT50_ALTOPENDRAIN
// Pin15
#define PIN15_CLEAR                         PIN7_CLEAR
#define PIN15_IN_ANALOG                     PIN7_IN_ANALOG
#define PIN15_IN_FLOAT                      PIN7_IN_FLOAT
#define PIN15_IN_PULL                       PIN7_IN_PULL
#define PIN15_OUT10_GPPUSHPULL              PIN7_OUT10_GPPUSHPULL
#define PIN15_OUT10_GPOPENDRAIN             PIN7_OUT10_GPOPENDRAIN
#define PIN15_OUT10_ALTPUSHPULL             PIN7_OUT10_ALTPUSHPULL
#define PIN15_OUT10_ALTOPENDRAIN            PIN7_OUT10_ALTOPENDRAIN
#define PIN15_OUT2_GPPUSHPULL               PIN7_OUT2_GPPUSHPULL
#define PIN15_OUT2_GPOPENDRAIN              PIN7_OUT2_GPOPENDRAIN
#define PIN15_OUT2_ALTPUSHPULL              PIN7_OUT2_ALTPUSHPULL
#define PIN15_OUT2_ALTOPENDRAIN             PIN7_OUT2_ALTOPENDRAIN
#define PIN15_OUT50_GPPUSHPULL              PIN7_OUT50_GPPUSHPULL
#define PIN15_OUT50_GPOPENDRAIN             PIN7_OUT50_GPOPENDRAIN
#define PIN15_OUT50_ALTPUSHPULL             PIN7_OUT50_ALTPUSHPULL
#define PIN15_OUT50_ALTOPENDRAIN            PIN7_OUT50_ALTOPENDRAIN    
    
#define PIN_CLEAR                            (BIT3|BIT2|BIT1|BIT0)     // Clear before setting
#define PIN_IN_ANALOG                        0                         // Clear for Analog
#define PIN_IN_FLOAT                         (BIT2)
#define PIN_IN_PULL                          (BIT3)
#define PIN_OUT10_GPPUSHPULL                 (BIT0)                    // Mode bit
#define PIN_OUT10_GPOPENDRAIN                (BIT2|BIT0)
#define PIN_OUT10_ALTPUSHPULL                (BIT3|BIT0)
#define PIN_OUT10_ALTOPENDRAIN               (BIT3|BIT2|BIT0)
#define PIN_OUT2_GPPUSHPULL                  (BIT1)                    // Mode bit
#define PIN_OUT2_GPOPENDRAIN                 (BIT2|BIT1)
#define PIN_OUT2_ALTPUSHPULL                 (BIT3|BIT1)
#define PIN_OUT2_ALTOPENDRAIN                (BIT3|BIT2|BIT1)
#define PIN_OUT50_GPPUSHPULL                 (BIT1|BIT0)               // Mode Bits
#define PIN_OUT50_GPOPENDRAIN                (BIT2|BIT1|BIT0)
#define PIN_OUT50_ALTPUSHPULL                (BIT3|BIT1|BIT0)
#define PIN_OUT50_ALTOPENDRAIN               (BIT3|BIT2|BIT1|BIT0)

// GPIOA Base Pin Macros
#define PA0_ALT             GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_OUT50_ALTPUSHPULL;
#define PA0_OUT             GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_OUT50_GPPUSHPULL;
#define PA0_FLOAT           GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_IN_FLOAT;
#define PA0_PULLUP          GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_IN_PULL; GPIOA->BSRR = BIT0;
#define PA0_PULLDOWN        GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_IN_PULL; GPIOA->BRR = BIT0;
#define PA0_ANALOG          GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_IN_ANALOG;
#define PA0_GET             ( GPIOA->IDR & BIT0 )
#define PA0_STATE           PA0_GET
#define PA0_HIGH            GPIOA->BSRR = BIT0;
#define PA0_LOW             GPIOA->BRR = BIT0;

#define PA1_ALT             GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_OUT50_ALTPUSHPULL;
#define PA1_OUT             GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_OUT50_GPPUSHPULL;
#define PA1_FLOAT           GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_IN_FLOAT;
#define PA1_PULLUP          GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_IN_PULL; GPIOA->BSRR = BIT1;
#define PA1_PULLDOWN        GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_IN_PULL; GPIOA->BRR = BIT1;
#define PA1_ANALOG          GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_IN_ANALOG;
#define PA1_GET             ( GPIOA->IDR & BIT1 )
#define PA1_STATE           PA1_GET
#define PA1_HIGH            GPIOA->BSRR = BIT1;
#define PA1_LOW             GPIOA->BRR = BIT1;

#define PA2_ALT             GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_OUT50_ALTPUSHPULL;
#define PA2_OUT             GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_OUT50_GPPUSHPULL;
#define PA2_FLOAT           GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_IN_FLOAT;
#define PA2_PULLUP          GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_IN_PULL; GPIOA->BSRR = BIT2;
#define PA2_PULLDOWN        GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_IN_PULL; GPIOA->BRR = BIT2;
#define PA2_ANALOG          GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_IN_ANALOG;
#define PA2_GET             ( GPIOA->IDR & BIT2 )
#define PA2_STATE           PA2_GET
#define PA2_HIGH            GPIOA->BSRR = BIT2;
#define PA2_LOW             GPIOA->BRR = BIT2;

#define PA3_ALT             GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_OUT50_ALTPUSHPULL;
#define PA3_OUT             GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_OUT50_GPPUSHPULL;
#define PA3_FLOAT           GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_IN_FLOAT;
#define PA3_PULLUP          GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_IN_PULL; GPIOA->BSRR = BIT3;
#define PA3_PULLDOWN        GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_IN_PULL; GPIOA->BRR = BIT3;
#define PA3_ANALOG          GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_IN_ANALOG;
#define PA3_GET             ( GPIOA->IDR & BIT3 )
#define PA3_STATE           PA3_GET
#define PA3_HIGH            GPIOA->BSRR = BIT3;
#define PA3_LOW             GPIOA->BRR = BIT3;

#define PA4_ALT             GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_OUT50_ALTPUSHPULL;
#define PA4_OUT             GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_OUT50_GPPUSHPULL;
#define PA4_FLOAT           GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_IN_FLOAT;
#define PA4_PULLUP          GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_IN_PULL; GPIOA->BSRR = BIT4;
#define PA4_PULLDOWN        GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_IN_PULL; GPIOA->BRR = BIT4;
#define PA4_ANALOG          GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_IN_ANALOG;
#define PA4_GET             ( GPIOA->IDR & BIT4 )
#define PA4_STATE           PA4_GET
#define PA4_HIGH            GPIOA->BSRR = BIT4;
#define PA4_LOW             GPIOA->BRR = BIT4;

#define PA5_ALT             GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_OUT50_ALTPUSHPULL;
#define PA5_OUT             GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_OUT50_GPPUSHPULL;
#define PA5_FLOAT           GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_IN_FLOAT;
#define PA5_PULLUP          GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_IN_PULL; GPIOA->BSRR = BIT5;
#define PA5_PULLDOWN        GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_IN_PULL; GPIOA->BRR = BIT5;
#define PA5_ANALOG          GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_IN_ANALOG;
#define PA5_GET             ( GPIOA->IDR & BIT5 )
#define PA5_STATE           PA5_GET
#define PA5_HIGH            GPIOA->BSRR = BIT5;
#define PA5_LOW             GPIOA->BRR = BIT5;

#define PA6_ALT             GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_OUT50_ALTPUSHPULL;
#define PA6_OUT             GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_OUT50_GPPUSHPULL;
#define PA6_FLOAT           GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_FLOAT;
#define PA6_PULLUP          GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_PULL; GPIOA->BSRR = BIT6;
#define PA6_PULLDOWN        GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_PULL; GPIOA->BRR = BIT6;
#define PA6_ANALOG          GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_ANALOG;
#define PA6_GET             ( GPIOA->IDR & BIT6 )
#define PA6_STATE           PA6_GET
#define PA6_HIGH            GPIOA->BSRR = BIT6;
#define PA6_LOW             GPIOA->BRR = BIT6;

#define PA7_ALT             GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_OUT50_ALTPUSHPULL;
#define PA7_OUT             GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_OUT50_GPPUSHPULL;
#define PA7_FLOAT           GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_FLOAT;
#define PA7_PULLUP          GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_PULL; GPIOA->BSRR = BIT7;
#define PA7_PULLDOWN        GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_PULL; GPIOA->BRR = BIT7;
#define PA7_ANALOG          GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_ANALOG;
#define PA7_GET             ( GPIOA->IDR & BIT7 )
#define PA7_STATE           PA7_GET
#define PA7_HIGH            GPIOA->BSRR = BIT7;
#define PA7_LOW             GPIOA->BRR = BIT7;

#define PA8_ALT             GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_OUT50_ALTPUSHPULL;
#define PA8_OUT             GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_OUT50_GPPUSHPULL;
#define PA8_FLOAT           GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_IN_FLOAT;
#define PA8_PULLUP          GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_IN_PULL; GPIOA->BSRR = BIT8;
#define PA8_PULLDOWN        GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_IN_PULL; GPIOA->BRR = BIT8;
#define PA8_ANALOG          GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_IN_ANALOG;
#define PA8_GET             ( GPIOA->IDR & BIT8 )
#define PA8_STATE           PA8_GET
#define PA8_HIGH            GPIOA->BSRR = BIT8;
#define PA8_LOW             GPIOA->BRR = BIT8;

#define PA9_ALT             GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_OUT50_ALTPUSHPULL;
#define PA9_OUT             GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_OUT50_GPPUSHPULL;
#define PA9_FLOAT           GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_IN_FLOAT;
#define PA9_PULLUP          GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_IN_PULL; GPIOA->BSRR = BIT9;
#define PA9_PULLDOWN        GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_IN_PULL; GPIOA->BRR = BIT9;
#define PA9_ANALOG          GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_IN_ANALOG;
#define PA9_GET             ( GPIOA->IDR & BIT9 )
#define PA9_STATE           PA9_GET
#define PA9_HIGH            GPIOA->BSRR = BIT9;
#define PA9_LOW             GPIOA->BRR = BIT9;

#define PA10_ALT             GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_OUT50_ALTPUSHPULL;
#define PA10_OUT             GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_OUT50_GPPUSHPULL;
#define PA10_FLOAT           GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_IN_FLOAT;
#define PA10_PULLUP          GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_IN_PULL; GPIOA->BSRR = BIT10;
#define PA10_PULLDOWN        GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_IN_PULL; GPIOA->BRR = BIT10;
#define PA10_ANALOG          GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_IN_ANALOG;
#define PA10_GET             ( GPIOA->IDR & BIT10 )
#define PA10_STATE           PA10_GET
#define PA10_HIGH            GPIOA->BSRR = BIT10;
#define PA10_LOW             GPIOA->BRR = BIT10;

#define PA11_ALT             GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_OUT50_ALTPUSHPULL;
#define PA11_OUT             GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_OUT50_GPPUSHPULL;
#define PA11_FLOAT           GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_IN_FLOAT;
#define PA11_PULLUP          GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_IN_PULL; GPIOA->BSRR = BIT11;
#define PA11_PULLDOWN        GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_IN_PULL; GPIOA->BRR = BIT11;
#define PA11_ANALOG          GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_IN_ANALOG;
#define PA11_GET             ( GPIOA->IDR & BIT11 )
#define PA11_STATE           PA11_GET
#define PA11_HIGH            GPIOA->BSRR = BIT11;
#define PA11_LOW             GPIOA->BRR = BIT11;

#define PA12_ALT             GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_OUT50_ALTPUSHPULL;
#define PA12_OUT             GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_OUT50_GPPUSHPULL;
#define PA12_FLOAT           GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_IN_FLOAT;
#define PA12_PULLUP          GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_IN_PULL; GPIOA->BSRR = BIT12;
#define PA12_PULLDOWN        GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_IN_PULL; GPIOA->BRR = BIT12;
#define PA12_ANALOG          GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_IN_ANALOG;
#define PA12_GET             ( GPIOA->IDR & BIT12 )
#define PA12_STATE           PA12_GET
#define PA12_HIGH            GPIOA->BSRR = BIT12;
#define PA12_LOW             GPIOA->BRR = BIT12;

#define PA13_ALT             GPIOA->CRH &= ~PIN13_CLEAR; GPIOA->CRH |= PIN13_OUT50_ALTPUSHPULL;
#define PA13_OUT             GPIOA->CRH &= ~PIN13_CLEAR; GPIOA->CRH |= PIN13_OUT50_GPPUSHPULL;
#define PA13_FLOAT           GPIOA->CRH &= ~PIN13_CLEAR; GPIOA->CRH |= PIN13_IN_FLOAT;
#define PA13_PULLUP          GPIOA->CRH &= ~PIN13_CLEAR; GPIOA->CRH |= PIN13_IN_PULL; GPIOA->BSRR = BIT13;
#define PA13_PULLDOWN        GPIOA->CRH &= ~PIN13_CLEAR; GPIOA->CRH |= PIN13_IN_PULL; GPIOA->BRR = BIT13;
#define PA13_ANALOG          GPIOA->CRH &= ~PIN13_CLEAR; GPIOA->CRH |= PIN13_IN_ANALOG;
#define PA13_GET             ( GPIOA->IDR & BIT13 )
#define PA13_STATE           PA13_GET
#define PA13_HIGH            GPIOA->BSRR = BIT13;
#define PA13_LOW             GPIOA->BRR = BIT13;

#define PA14_ALT             GPIOA->CRH &= ~PIN14_CLEAR; GPIOA->CRH |= PIN14_OUT50_ALTPUSHPULL;
#define PA14_OUT             GPIOA->CRH &= ~PIN14_CLEAR; GPIOA->CRH |= PIN14_OUT50_GPPUSHPULL;
#define PA14_FLOAT           GPIOA->CRH &= ~PIN14_CLEAR; GPIOA->CRH |= PIN14_IN_FLOAT;
#define PA14_PULLUP          GPIOA->CRH &= ~PIN14_CLEAR; GPIOA->CRH |= PIN14_IN_PULL; GPIOA->BSRR = BIT14;
#define PA14_PULLDOWN        GPIOA->CRH &= ~PIN14_CLEAR; GPIOA->CRH |= PIN14_IN_PULL; GPIOA->BRR = BIT14;
#define PA14_ANALOG          GPIOA->CRH &= ~PIN14_CLEAR; GPIOA->CRH |= PIN14_IN_ANALOG;
#define PA14_GET             ( GPIOA->IDR & BIT14 )
#define PA14_STATE           PA14_GET
#define PA14_HIGH            GPIOA->BSRR = BIT14;
#define PA14_LOW             GPIOA->BRR = BIT14;

#define PA15_ALT             GPIOA->CRH &= ~PIN15_CLEAR; GPIOA->CRH |= PIN15_OUT50_ALTPUSHPULL;
#define PA15_OUT             GPIOA->CRH &= ~PIN15_CLEAR; GPIOA->CRH |= PIN15_OUT50_GPPUSHPULL;
#define PA15_FLOAT           GPIOA->CRH &= ~PIN15_CLEAR; GPIOA->CRH |= PIN15_IN_FLOAT;
#define PA15_PULLUP          GPIOA->CRH &= ~PIN15_CLEAR; GPIOA->CRH |= PIN15_IN_PULL; GPIOA->BSRR = BIT15;
#define PA15_PULLDOWN        GPIOA->CRH &= ~PIN15_CLEAR; GPIOA->CRH |= PIN15_IN_PULL; GPIOA->BRR = BIT15;
#define PA15_ANALOG          GPIOA->CRH &= ~PIN15_CLEAR; GPIOA->CRH |= PIN15_IN_ANALOG;
#define PA15_GET             ( GPIOA->IDR & BIT15 )
#define PA15_STATE           PA15_GET
#define PA15_HIGH            GPIOA->BSRR = BIT15;
#define PA15_LOW             GPIOA->BRR = BIT15;

#define PB0_ALT             GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_OUT50_ALTPUSHPULL;
#define PB0_OUT             GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_OUT50_GPPUSHPULL;
#define PB0_FLOAT           GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_IN_FLOAT;
#define PB0_PULLUP          GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_IN_PULL; GPIOB->BSRR = BIT0;
#define PB0_PULLDOWN        GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_IN_PULL; GPIOB->BRR = BIT0;
#define PB0_ANALOG          GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_IN_ANALOG;
#define PB0_GET             ( GPIOB->IDR & BIT0 )
#define PB0_STATE           PB0_GET
#define PB0_HIGH            GPIOB->BSRR = BIT0;
#define PB0_LOW             GPIOB->BRR = BIT0;

#define PB1_ALT             GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_OUT50_ALTPUSHPULL;
#define PB1_OUT             GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_OUT50_GPPUSHPULL;
#define PB1_FLOAT           GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_IN_FLOAT;
#define PB1_PULLUP          GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_IN_PULL; GPIOB->BSRR = BIT1;
#define PB1_PULLDOWN        GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_IN_PULL; GPIOB->BRR = BIT1;
#define PB1_ANALOG          GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_IN_ANALOG;
#define PB1_GET             ( GPIOB->IDR & BIT1 )
#define PB1_HIGH            GPIOB->BSRR = BIT1;
#define PB1_LOW             GPIOB->BRR = BIT1;

#define PB2_ALT             GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_OUT50_ALTPUSHPULL;
#define PB2_OUT             GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_OUT50_GPPUSHPULL;
#define PB2_FLOAT           GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_IN_FLOAT;
#define PB2_PULLUP          GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_IN_PULL; GPIOB->BSRR = BIT2;
#define PB2_PULLDOWN        GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_IN_PULL; GPIOB->BRR = BIT2;
#define PB2_ANALOG          GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_IN_ANALOG;
#define PB2_GET             ( GPIOB->IDR & BIT2 )
#define PB2_STATE           PB2_GET
#define PB2_HIGH            GPIOB->BSRR = BIT2;
#define PB2_LOW             GPIOB->BRR = BIT2;

#define PB3_ALT             GPIOB->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_OUT50_ALTPUSHPULL;
#define PB3_OUT             GPIOB->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_OUT50_GPPUSHPULL;
#define PB3_FLOAT           GPIOB->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_IN_FLOAT;
#define PB3_PULLUP          GPIOB->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_IN_PULL; GPIOB->BSRR = BIT3;
#define PB3_PULLDOWN        GPIOB->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_IN_PULL; GPIOB->BRR = BIT3;
#define PB3_ANALOG          GPIOB->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_IN_ANALOG;
#define PB3_GET             ( GPIOB->IDR & BIT3 )
#define PB3_STATE           PB3_GET
#define PB3_HIGH            GPIOB->BSRR = BIT3;
#define PB3_LOW             GPIOB->BRR = BIT3;

#define PB4_ALT             GPIOB->CRL &= ~PIN4_CLEAR; GPIOB->CRL |= PIN4_OUT50_ALTPUSHPULL;
#define PB4_OUT             GPIOB->CRL &= ~PIN4_CLEAR; GPIOB->CRL |= PIN4_OUT50_GPPUSHPULL;
#define PB4_FLOAT           GPIOB->CRL &= ~PIN4_CLEAR; GPIOB->CRL |= PIN4_IN_FLOAT;
#define PB4_PULLUP          GPIOB->CRL &= ~PIN4_CLEAR; GPIOB->CRL |= PIN4_IN_PULL; GPIOB->BSRR = BIT4;
#define PB4_PULLDOWN        GPIOB->CRL &= ~PIN4_CLEAR; GPIOB->CRL |= PIN4_IN_PULL; GPIOB->BRR = BIT4;
#define PB4_ANALOG          GPIOB->CRL &= ~PIN4_CLEAR; GPIOB->CRL |= PIN4_IN_ANALOG;
#define PB4_GET             ( GPIOB->IDR & BIT4 )
#define PB4_STATE           PB4_GET
#define PB4_HIGH            GPIOB->BSRR = BIT4;
#define PB4_LOW             GPIOB->BRR = BIT4;

#define PB5_ALT             GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_OUT50_ALTPUSHPULL;
#define PB5_OUT             GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_OUT50_GPPUSHPULL;
#define PB5_FLOAT           GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_IN_FLOAT;
#define PB5_PULLUP          GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_IN_PULL; GPIOB->BSRR = BIT5;
#define PB5_PULLDOWN        GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_IN_PULL; GPIOB->BRR = BIT5;
#define PB5_ANALOG          GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_IN_ANALOG;
#define PB5_GET             ( GPIOB->IDR & BIT5 )
#define PB5_STATE           PB5_GET
#define PB5_HIGH            GPIOB->BSRR = BIT5;
#define PB5_LOW             GPIOB->BRR = BIT5;

#define PB6_ALT             GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_OUT50_ALTPUSHPULL;
#define PB6_OUT             GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_OUT50_GPPUSHPULL;
#define PB6_FLOAT           GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_FLOAT;
#define PB6_PULLUP          GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_PULL; GPIOB->BSRR = BIT6;
#define PB6_PULLDOWN        GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_PULL; GPIOB->BRR = BIT6;
#define PB6_ANALOG          GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_ANALOG;
#define PB6_GET             ( GPIOB->IDR & BIT6 )
#define PB6_STATE           PB6_GET
#define PB6_HIGH            GPIOB->BSRR = BIT6;
#define PB6_LOW             GPIOB->BRR = BIT6;

#define PB7_ALT             GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_OUT50_ALTPUSHPULL;
#define PB7_OUT             GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_OUT50_GPPUSHPULL;
#define PB7_FLOAT           GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_FLOAT;
#define PB7_PULLUP          GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_PULL; GPIOB->BSRR = BIT7;
#define PB7_PULLDOWN        GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_PULL; GPIOB->BRR = BIT7;
#define PB7_ANALOG          GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_ANALOG;
#define PB7_GET             ( GPIOB->IDR & BIT7 )
#define PB7_STATE           PB7_GET
#define PB7_HIGH            GPIOB->BSRR = BIT7;
#define PB7_LOW             GPIOB->BRR = BIT7;

#define PB8_ALT             GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_OUT50_ALTPUSHPULL;
#define PB8_OUT             GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_OUT50_GPPUSHPULL;
#define PB8_FLOAT           GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_FLOAT;
#define PB8_PULLUP          GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_PULL; GPIOB->BSRR = BIT8;
#define PB8_PULLDOWN        GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_PULL; GPIOB->BRR = BIT8;
#define PB8_ANALOG          GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_ANALOG;
#define PB8_GET             ( GPIOB->IDR & BIT8 )
#define PB8_STATE           PB8_GET
#define PB8_HIGH            GPIOB->BSRR = BIT8;
#define PB8_LOW             GPIOB->BRR = BIT8;

#define PB9_ALT             GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_OUT50_ALTPUSHPULL;
#define PB9_OUT             GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_OUT50_GPPUSHPULL;
#define PB9_FLOAT           GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_FLOAT;
#define PB9_PULLUP          GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_PULL; GPIOB->BSRR = BIT9;
#define PB9_PULLDOWN        GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_PULL; GPIOB->BRR = BIT9;
#define PB9_ANALOG          GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_ANALOG;
#define PB9_GET             ( GPIOB->IDR & BIT9 )
#define PB9_STATE           PB9_GET
#define PB9_HIGH            GPIOB->BSRR = BIT9;
#define PB9_LOW             GPIOB->BRR = BIT9;
                            
#define PB10_ALT            GPIOB->CRH &= ~PIN10_CLEAR; GPIOB->CRH |= PIN10_OUT50_ALTPUSHPULL;
#define PB10_OUT            GPIOB->CRH &= ~PIN10_CLEAR; GPIOB->CRH |= PIN10_OUT50_GPPUSHPULL;
#define PB10_FLOAT          GPIOB->CRH &= ~PIN10_CLEAR; GPIOB->CRH |= PIN10_IN_FLOAT;
#define PB10_PULLUP         GPIOB->CRH &= ~PIN10_CLEAR; GPIOB->CRH |= PIN10_IN_PULL; GPIOB->BSRR = BIT10;
#define PB10_PULLDOWN       GPIOB->CRH &= ~PIN10_CLEAR; GPIOB->CRH |= PIN10_IN_PULL; GPIOB->BRR = BIT10;
#define PB10_ANALOG         GPIOB->CRH &= ~PIN10_CLEAR; GPIOB->CRH |= PIN10_IN_ANALOG;
#define PB10_GET            ( GPIOB->IDR & BIT10 )
#define PB10_STATE          PB10_GET
#define PB10_HIGH           GPIOB->BSRR = BIT10;
#define PB10_LOW            GPIOB->BRR = BIT10;

#define PB11_ALT            GPIOB->CRH &= ~PIN11_CLEAR; GPIOB->CRH |= PIN11_OUT50_ALTPUSHPULL;
#define PB11_OUT            GPIOB->CRH &= ~PIN11_CLEAR; GPIOB->CRH |= PIN11_OUT50_GPPUSHPULL;
#define PB11_FLOAT          GPIOB->CRH &= ~PIN11_CLEAR; GPIOB->CRH |= PIN11_IN_FLOAT;
#define PB11_PULLUP         GPIOB->CRH &= ~PIN11_CLEAR; GPIOB->CRH |= PIN11_IN_PULL; GPIOB->BSRR = BIT11;
#define PB11_PULLDOWN       GPIOB->CRH &= ~PIN11_CLEAR; GPIOB->CRH |= PIN11_IN_PULL; GPIOB->BRR = BIT11;
#define PB11_ANALOG         GPIOB->CRH &= ~PIN11_CLEAR; GPIOB->CRH |= PIN11_IN_ANALOG;
#define PB11_GET            ( GPIOB->IDR & BIT11 )
#define PB11_STATE          PB11_GET
#define PB11_HIGH           GPIOB->BSRR = BIT11;
#define PB11_LOW            GPIOB->BRR = BIT11;

#define PB12_ALT            GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_OUT50_ALTPUSHPULL;
#define PB12_OUT            GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_OUT50_GPPUSHPULL;
#define PB12_FLOAT          GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_IN_FLOAT;
#define PB12_PULLUP         GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_IN_PULL; GPIOB->BSRR = BIT12;
#define PB12_PULLDOWN       GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_IN_PULL; GPIOB->BRR = BIT12;
#define PB12_ANALOG         GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_IN_ANALOG;
#define PB12_GET            ( GPIOB->IDR & BIT12 )
#define PB12_STATE          PB12_GET
#define PB12_HIGH           GPIOB->BSRR = BIT12;
#define PB12_LOW            GPIOB->BRR = BIT12;

#define PB13_ALT            GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_OUT50_ALTPUSHPULL;
#define PB13_OUT            GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_OUT50_GPPUSHPULL;
#define PB13_FLOAT          GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_IN_FLOAT;
#define PB13_PULLUP         GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_IN_PULL; GPIOB->BSRR = BIT13;
#define PB13_PULLDOWN       GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_IN_PULL; GPIOB->BRR = BIT13;
#define PB13_ANALOG         GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_IN_ANALOG;
#define PB13_GET            ( GPIOB->IDR & BIT13 )
#define PB13_STATE          PB13_GET
#define PB13_HIGH           GPIOB->BSRR = BIT13;
#define PB13_LOW            GPIOB->BRR = BIT13;

#define PB14_ALT            GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_OUT50_ALTPUSHPULL;
#define PB14_OUT            GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_OUT50_GPPUSHPULL;
#define PB14_FLOAT          GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_FLOAT;
#define PB14_PULLUP         GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_PULL; GPIOB->BSRR = BIT14;
#define PB14_PULLDOWN       GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_PULL; GPIOB->BRR = BIT14;
#define PB14_ANALOG         GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_ANALOG;
#define PB14_GET            ( GPIOB->IDR & BIT14 )
#define PB14_STATE          PB14_GET
#define PB14_HIGH           GPIOB->BSRR = BIT14;
#define PB14_LOW            GPIOB->BRR = BIT14;

#define PB15_ALT            GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_OUT50_ALTPUSHPULL;
#define PB15_OUT            GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_OUT50_GPPUSHPULL;
#define PB15_FLOAT          GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_IN_FLOAT;
#define PB15_PULLUP         GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_IN_PULL; GPIOB->BSRR = BIT15;
#define PB15_PULLDOWN       GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_IN_PULL; GPIOB->BRR = BIT15;
#define PB15_ANALOG         GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_IN_ANALOG;
#define PB15_GET            ( GPIOB->IDR & BIT15 )
#define PB15_STATE          PB15_GET
#define PB15_HIGH           GPIOB->BSRR = BIT15;
#define PB15_LOW            GPIOB->BRR = BIT15;

#define PC0_ALT             GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_OUT50_ALTPUSHPULL;
#define PC0_OUT             GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_OUT50_GPPUSHPULL;
#define PC0_FLOAT           GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_IN_FLOAT;
#define PC0_PULLUP          GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_IN_PULL; GPIOC->BSRR = BIT0;
#define PC0_PULLDOWN        GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_IN_PULL; GPIOC->BRR = BIT0;
#define PC0_ANALOG          GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_IN_ANALOG;
#define PC0_GET             ( GPIOC->IDR & BIT0 )
#define PC0_STATE           PC0_STATE
#define PC0_HIGH            GPIOC->BSRR = BIT0;
#define PC0_LOW             GPIOC->BRR = BIT0;

#define PC1_ALT             GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_OUT50_ALTPUSHPULL;
#define PC1_OUT             GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_OUT50_GPPUSHPULL;
#define PC1_FLOAT           GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_IN_FLOAT;
#define PC1_PULLUP          GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_IN_PULL; GPIOC->BSRR = BIT1;
#define PC1_PULLDOWN        GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_IN_PULL; GPIOC->BRR = BIT1;
#define PC1_ANALOG          GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_IN_ANALOG;
#define PC1_GET             ( GPIOC->IDR & BIT1 )
#define PC1_STATE           PC1_GET
#define PC1_HIGH            GPIOC->BSRR = BIT1;
#define PC1_LOW             GPIOC->BRR = BIT1;

#define PC2_ALT             GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_OUT50_ALTPUSHPULL;
#define PC2_OUT             GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_OUT50_GPPUSHPULL;
#define PC2_FLOAT           GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_IN_FLOAT;
#define PC2_PULLUP          GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_IN_PULL; GPIOC->BSRR = BIT2;
#define PC2_PULLDOWN        GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_IN_PULL; GPIOC->BRR = BIT2;
#define PC2_ANALOG          GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_IN_ANALOG;
#define PC2_GET             ( GPIOC->IDR & BIT2 )
#define PC2_STATE           PC2_GET
#define PC2_HIGH            GPIOC->BSRR = BIT2;
#define PC2_LOW             GPIOC->BRR = BIT2;

#define PC3_ALT             GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_OUT50_ALTPUSHPULL;
#define PC3_OUT             GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_OUT50_GPPUSHPULL;
#define PC3_FLOAT           GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_IN_FLOAT;
#define PC3_PULLUP          GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_IN_PULL; GPIOC->BSRR = BIT3;
#define PC3_PULLDOWN        GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_IN_PULL; GPIOC->BRR = BIT3;
#define PC3_ANALOG          GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_IN_ANALOG;
#define PC3_GET             ( GPIOC->IDR & BIT3 )
#define PC3_STATE           PC3_GET
#define PC3_HIGH            GPIOC->BSRR = BIT3;
#define PC3_LOW             GPIOC->BRR = BIT3;

#define PC4_ALT             GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_OUT50_ALTPUSHPULL;
#define PC4_OUT             GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_OUT50_GPPUSHPULL;
#define PC4_FLOAT           GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_IN_FLOAT;
#define PC4_PULLUP          GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_IN_PULL; GPIOC->BSRR = BIT4;
#define PC4_PULLDOWN        GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_IN_PULL; GPIOC->BRR = BIT4;
#define PC4_ANALOG          GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_IN_ANALOG;
#define PC4_GET             ( GPIOC->IDR & BIT4 )
#define PC4_STATE           PC4_GET
#define PC4_HIGH            GPIOC->BSRR = BIT4;
#define PC4_LOW             GPIOC->BRR = BIT4;

#define PC5_ALT             GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_OUT50_ALTPUSHPULL;
#define PC5_OUT             GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_OUT50_GPPUSHPULL;
#define PC5_FLOAT           GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_IN_FLOAT;
#define PC5_PULLUP          GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_IN_PULL; GPIOC->BSRR = BIT5;
#define PC5_PULLDOWN        GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_IN_PULL; GPIOC->BRR = BIT5;
#define PC5_ANALOG          GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_IN_ANALOG;
#define PC5_GET             ( GPIOC->IDR & BIT5 )
#define PC5_STATE           PC5_GET
#define PC5_HIGH            GPIOC->BSRR = BIT5;
#define PC5_LOW             GPIOC->BRR = BIT5;

#define PC6_ALT             GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_OUT50_ALTPUSHPULL;
#define PC6_OUT             GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_OUT50_GPPUSHPULL;
#define PC6_FLOAT           GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_IN_FLOAT;
#define PC6_PULLUP          GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_IN_PULL; GPIOC->BSRR = BIT6;
#define PC6_PULLDOWN        GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_IN_PULL; GPIOC->BRR = BIT6;
#define PC6_ANALOG          GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_IN_ANALOG;
#define PC6_GET             ( GPIOC->IDR & BIT6 )
#define PC6_STATE           PC6_GET
#define PC6_HIGH            GPIOC->BSRR = BIT6;
#define PC6_LOW             GPIOC->BRR = BIT6;

#define PC7_ALT             GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_OUT50_ALTPUSHPULL;
#define PC7_OUT             GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_OUT50_GPPUSHPULL;
#define PC7_FLOAT           GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_IN_FLOAT;
#define PC7_PULLUP          GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_IN_PULL; GPIOC->BSRR = BIT7;
#define PC7_PULLDOWN        GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_IN_PULL; GPIOC->BRR = BIT7;
#define PC7_ANALOG          GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_IN_ANALOG;
#define PC7_GET             ( GPIOC->IDR & BIT7 )
#define PC7_STATE           PC7_GET
#define PC7_HIGH            GPIOC->BSRR = BIT7;
#define PC7_LOW             GPIOC->BRR = BIT7;

#define PC8_ALT             GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_OUT50_ALTPUSHPULL;
#define PC8_OUT             GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_OUT50_GPPUSHPULL;
#define PC8_FLOAT           GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_IN_FLOAT;
#define PC8_PULLUP          GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_IN_PULL; GPIOC->BSRR = BIT8;
#define PC8_PULLDOWN        GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_IN_PULL; GPIOC->BRR = BIT8;
#define PC8_ANALOG          GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_IN_ANALOG;
#define PC8_GET             ( GPIOC->IDR & BIT8 )
#define PC8_STATE           PC8_GET
#define PC8_HIGH            GPIOC->BSRR = BIT8;
#define PC8_LOW             GPIOC->BRR = BIT8;

#define PC9_ALT             GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_OUT50_ALTPUSHPULL;
#define PC9_OUT             GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_OUT50_GPPUSHPULL;
#define PC9_FLOAT           GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_IN_FLOAT;
#define PC9_PULLUP          GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_IN_PULL; GPIOC->BSRR = BIT9;
#define PC9_PULLDOWN        GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_IN_PULL; GPIOC->BRR = BIT9;
#define PC9_ANALOG          GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_IN_ANALOG;
#define PC9_GET             ( GPIOC->IDR & BIT9 )
#define PC9_STATE           PC9_GET
#define PC9_HIGH            GPIOC->BSRR = BIT9;
#define PC9_LOW             GPIOC->BRR = BIT9;

#define PC10_ALT            GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_OUT50_ALTPUSHPULL;
#define PC10_OUT            GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_OUT50_GPPUSHPULL;
#define PC10_FLOAT          GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_IN_FLOAT;
#define PC10_PULLUP         GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_IN_PULL; GPIOC->BSRR = BIT10;
#define PC10_PULLDOWN       GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_IN_PULL; GPIOC->BRR = BIT10;
#define PC10_ANALOG         GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_IN_ANALOG;
#define PC10_GET            ( GPIOC->IDR & BIT10 )
#define PC10_STATE          PC10_GET
#define PC10_HIGH           GPIOC->BSRR = BIT10;
#define PC10_LOW            GPIOC->BRR = BIT10;

#define PC11_ALT            GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_OUT50_ALTPUSHPULL;
#define PC11_OUT            GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_OUT50_GPPUSHPULL;
#define PC11_FLOAT          GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_IN_FLOAT;
#define PC11_PULLUP         GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_IN_PULL; GPIOC->BSRR = BIT11;
#define PC11_PULLDOWN       GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_IN_PULL; GPIOC->BRR = BIT11;
#define PC11_ANALOG         GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_IN_ANALOG;
#define PC11_GET            ( GPIOC->IDR & BIT11 )
#define PC11_STATE          PC11_GET
#define PC11_HIGH           GPIOC->BSRR = BIT11;
#define PC11_LOW            GPIOC->BRR = BIT11;

#define PC12_ALT            GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_OUT50_ALTPUSHPULL;
#define PC12_OUT            GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_OUT50_GPPUSHPULL;
#define PC12_FLOAT          GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_IN_FLOAT;
#define PC12_PULLUP         GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_IN_PULL; GPIOC->BSRR = BIT12;
#define PC12_PULLDOWN       GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_IN_PULL; GPIOC->BRR = BIT12;
#define PC12_ANALOG         GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_IN_ANALOG;
#define PC12_GET            ( GPIOC->IDR & BIT12 )
#define PC12_STATE          PC12_GET
#define PC12_HIGH           GPIOC->BSRR = BIT12;
#define PC12_LOW            GPIOC->BRR = BIT12;

#define PC13_ALT            GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_OUT50_ALTPUSHPULL;
#define PC13_OUT            GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_OUT50_GPPUSHPULL;
#define PC13_FLOAT          GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_IN_FLOAT;
#define PC13_PULLUP         GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_IN_PULL; GPIOC->BSRR = BIT13;
#define PC13_PULLDOWN       GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_IN_PULL; GPIOC->BRR = BIT13;
#define PC13_ANALOG         GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_IN_ANALOG;
#define PC13_GET            ( GPIOC->IDR & BIT13 )
#define PC13_STATE          PC13_GET
#define PC13_HIGH           GPIOC->BSRR = BIT13;
#define PC13_LOW            GPIOC->BRR = BIT13;

#define PC14_ALT            GPIOC->CRH &= ~PIN14_CLEAR; GPIOC->CRH |= PIN14_OUT50_ALTPUSHPULL;
#define PC14_OUT            GPIOC->CRH &= ~PIN14_CLEAR; GPIOC->CRH |= PIN14_OUT50_GPPUSHPULL;
#define PC14_FLOAT          GPIOC->CRH &= ~PIN14_CLEAR; GPIOC->CRH |= PIN14_IN_FLOAT;
#define PC14_PULLUP         GPIOC->CRH &= ~PIN14_CLEAR; GPIOC->CRH |= PIN14_IN_PULL; GPIOC->BSRR = BIT14;
#define PC14_PULLDOWN       GPIOC->CRH &= ~PIN14_CLEAR; GPIOC->CRH |= PIN14_IN_PULL; GPIOC->BRR = BIT14;
#define PC14_ANALOG         GPIOC->CRH &= ~PIN14_CLEAR; GPIOC->CRH |= PIN14_IN_ANALOG;
#define PC14_GET            ( GPIOC->IDR & BIT14 )
#define PC14_STATE          PC14_GET
#define PC14_HIGH           GPIOC->BSRR = BIT14;
#define PC14_LOW            GPIOC->BRR = BIT14;

#define PC15_ALT            GPIOC->CRH &= ~PIN15_CLEAR; GPIOC->CRH |= PIN15_OUT50_ALTPUSHPULL;
#define PC15_OUT            GPIOC->CRH &= ~PIN15_CLEAR; GPIOC->CRH |= PIN15_OUT50_GPPUSHPULL;
#define PC15_FLOAT          GPIOC->CRH &= ~PIN15_CLEAR; GPIOC->CRH |= PIN15_IN_FLOAT;
#define PC15_PULLUP         GPIOC->CRH &= ~PIN15_CLEAR; GPIOC->CRH |= PIN15_IN_PULL; GPIOC->BSRR = BIT15;
#define PC15_PULLDOWN       GPIOC->CRH &= ~PIN15_CLEAR; GPIOC->CRH |= PIN15_IN_PULL; GPIOC->BRR = BIT15;
#define PC15_ANALOG         GPIOC->CRH &= ~PIN15_CLEAR; GPIOC->CRH |= PIN15_IN_ANALOG;
#define PC15_GET            ( GPIOC->IDR & BIT15 )
#define PC15_STATE          PC15_GET
#define PC15_HIGH           GPIOC->BSRR = BIT15;
#define PC15_LOW            GPIOC->BRR = BIT15;

#define PD0_ALT             GPIOD->CRL &= ~PIN0_CLEAR; GPIOD->CRL |= PIN0_OUT50_ALTPUSHPULL;
#define PD0_OUT             GPIOD->CRL &= ~PIN0_CLEAR; GPIOD->CRL |= PIN0_OUT50_GPPUSHPULL;
#define PD0_FLOAT           GPIOD->CRL &= ~PIN0_CLEAR; GPIOD->CRL |= PIN0_IN_FLOAT;
#define PD0_PULLUP          GPIOD->CRL &= ~PIN0_CLEAR; GPIOD->CRL |= PIN0_IN_PULL; GPIOD->BSRR = BIT0;
#define PD0_PULLDOWN        GPIOD->CRL &= ~PIN0_CLEAR; GPIOD->CRL |= PIN0_IN_PULL; GPIOD->BRR = BIT0;
#define PD0_ANALOG          GPIOD->CRL &= ~PIN0_CLEAR; GPIOD->CRL |= PIN0_IN_ANALOG;
#define PD0_GET             ( GPIOD->IDR & BIT0 )
#define PD0_STATE           PD0_GET
#define PD0_HIGH            GPIOD->BSRR = BIT0;
#define PD0_LOW             GPIOD->BRR = BIT0;

#define PD1_ALT             GPIOD->CRL &= ~PIN1_CLEAR; GPIOD->CRL |= PIN1_OUT50_ALTPUSHPULL;
#define PD1_OUT             GPIOD->CRL &= ~PIN1_CLEAR; GPIOD->CRL |= PIN1_OUT50_GPPUSHPULL;
#define PD1_FLOAT           GPIOD->CRL &= ~PIN1_CLEAR; GPIOD->CRL |= PIN1_IN_FLOAT;
#define PD1_PULLUP          GPIOD->CRL &= ~PIN1_CLEAR; GPIOD->CRL |= PIN1_IN_PULL; GPIOC->BSRR = BIT1;
#define PD1_PULLDOWN        GPIOD->CRL &= ~PIN1_CLEAR; GPIOD->CRL |= PIN1_IN_PULL; GPIOC->BRR = BIT1;
#define PD1_ANALOG          GPIOD->CRL &= ~PIN1_CLEAR; GPIOD->CRL |= PIN1_IN_ANALOG;
#define PD1_GET             ( GPIOD->IDR & BIT1 )
#define PD1_STATE           PD1_GET
#define PD1_HIGH            GPIOD->BSRR = BIT1;
#define PD1_LOW             GPIOD->BRR = BIT1;

#define PD2_ALT             GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_OUT50_ALTPUSHPULL;
#define PD2_OUT             GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_OUT50_GPPUSHPULL;
#define PD2_FLOAT           GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_IN_FLOAT;
#define PD2_PULLUP          GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_IN_PULL; GPIOD->BSRR = BIT2;
#define PD2_PULLDOWN        GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_IN_PULL; GPIOD->BRR = BIT2;
#define PD2_ANALOG          GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_IN_ANALOG;
#define PD2_GET             ( GPIOD->IDR & BIT2 )
#define PD2_STATE           PD2_GET
#define PD2_HIGH            GPIOD->BSRR = BIT2;
#define PD2_LOW             GPIOD->BRR = BIT2;

#define PD3_ALT             GPIOD->CRL &= ~PIN3_CLEAR; GPIOD->CRL |= PIN3_OUT50_ALTPUSHPULL;
#define PD3_OUT             GPIOD->CRL &= ~PIN3_CLEAR; GPIOD->CRL |= PIN3_OUT50_GPPUSHPULL;
#define PD3_FLOAT           GPIOD->CRL &= ~PIN3_CLEAR; GPIOD->CRL |= PIN3_IN_FLOAT;
#define PD3_PULLUP          GPIOD->CRL &= ~PIN3_CLEAR; GPIOD->CRL |= PIN3_IN_PULL; GPIOD->BSRR = BIT3;
#define PD3_PULLDOWN        GPIOD->CRL &= ~PIN3_CLEAR; GPIOD->CRL |= PIN3_IN_PULL; GPIOD->BRR = BIT3;
#define PD3_ANALOG          GPIOD->CRL &= ~PIN3_CLEAR; GPIOD->CRL |= PIN3_IN_ANALOG;
#define PD3_GET             ( GPIOD->IDR & BIT3 )
#define PD3_STATE           PD3_GET
#define PD3_HIGH            GPIOD->BSRR = BIT3;
#define PD3_LOW             GPIOD->BRR = BIT3;

#define PD4_ALT             GPIOD->CRL &= ~PIN4_CLEAR; GPIOD->CRL |= PIN4_OUT50_ALTPUSHPULL;
#define PD4_OUT             GPIOD->CRL &= ~PIN4_CLEAR; GPIOD->CRL |= PIN4_OUT50_GPPUSHPULL;
#define PD4_FLOAT           GPIOD->CRL &= ~PIN4_CLEAR; GPIOD->CRL |= PIN4_IN_FLOAT;
#define PD4_PULLUP          GPIOD->CRL &= ~PIN4_CLEAR; GPIOD->CRL |= PIN4_IN_PULL; GPIOD->BSRR = BIT4;
#define PD4_PULLDOWN        GPIOD->CRL &= ~PIN4_CLEAR; GPIOD->CRL |= PIN4_IN_PULL; GPIOD->BRR = BIT4;
#define PD4_ANALOG          GPIOD->CRL &= ~PIN4_CLEAR; GPIOD->CRL |= PIN4_IN_ANALOG;
#define PD4_GET             ( GPIOD->IDR & BIT4 )
#define PD4_STATE           PD4_GET
#define PD4_HIGH            GPIOD->BSRR = BIT4;
#define PD4_LOW             GPIOD->BRR = BIT4;

#define PD5_ALT             GPIOD->CRL &= ~PIN5_CLEAR; GPIOD->CRL |= PIN5_OUT50_ALTPUSHPULL;
#define PD5_OUT             GPIOD->CRL &= ~PIN5_CLEAR; GPIOD->CRL |= PIN5_OUT50_GPPUSHPULL;
#define PD5_FLOAT           GPIOD->CRL &= ~PIN5_CLEAR; GPIOD->CRL |= PIN5_IN_FLOAT;
#define PD5_PULLUP          GPIOD->CRL &= ~PIN5_CLEAR; GPIOD->CRL |= PIN5_IN_PULL; GPIOD->BSRR = BIT5;
#define PD5_PULLDOWN        GPIOD->CRL &= ~PIN5_CLEAR; GPIOD->CRL |= PIN5_IN_PULL; GPIOD->BRR = BIT5;
#define PD5_ANALOG          GPIOD->CRL &= ~PIN5_CLEAR; GPIOD->CRL |= PIN5_IN_ANALOG;
#define PD5_GET             ( GPIOD->IDR & BIT5 )
#define PD5_STATE           PD5_GET
#define PD5_HIGH            GPIOD->BSRR = BIT5;
#define PD5_LOW             GPIOD->BRR = BIT5;

#define PD6_ALT             GPIOD->CRL &= ~PIN6_CLEAR; GPIOD->CRL |= PIN6_OUT50_ALTPUSHPULL;
#define PD6_OUT             GPIOD->CRL &= ~PIN6_CLEAR; GPIOD->CRL |= PIN6_OUT50_GPPUSHPULL;
#define PD6_FLOAT           GPIOD->CRL &= ~PIN6_CLEAR; GPIOD->CRL |= PIN6_IN_FLOAT;
#define PD6_PULLUP          GPIOD->CRL &= ~PIN6_CLEAR; GPIOD->CRL |= PIN6_IN_PULL; GPIOD->BSRR = BIT6;
#define PD6_PULLDOWN        GPIOD->CRL &= ~PIN6_CLEAR; GPIOD->CRL |= PIN6_IN_PULL; GPIOD->BRR = BIT6;
#define PD6_ANALOG          GPIOD->CRL &= ~PIN6_CLEAR; GPIOD->CRL |= PIN6_IN_ANALOG;
#define PD6_GET             ( GPIOD->IDR & BIT6 )
#define PD6_STATE           PD6_GET
#define PD6_HIGH            GPIOD->BSRR = BIT6;
#define PD6_LOW             GPIOD->BRR = BIT6;

#define PD7_ALT             GPIOD->CRL &= ~PIN7_CLEAR; GPIOD->CRL |= PIN7_OUT50_ALTPUSHPULL;
#define PD7_OUT             GPIOD->CRL &= ~PIN7_CLEAR; GPIOD->CRL |= PIN7_OUT50_GPPUSHPULL;
#define PD7_FLOAT           GPIOD->CRL &= ~PIN7_CLEAR; GPIOD->CRL |= PIN7_IN_FLOAT;
#define PD7_PULLUP          GPIOD->CRL &= ~PIN7_CLEAR; GPIOD->CRL |= PIN7_IN_PULL; GPIOD->BSRR = BIT7;
#define PD7_PULLDOWN        GPIOD->CRL &= ~PIN7_CLEAR; GPIOD->CRL |= PIN7_IN_PULL; GPIOD->BRR = BIT7;
#define PD7_ANALOG          GPIOD->CRL &= ~PIN7_CLEAR; GPIOD->CRL |= PIN7_IN_ANALOG;
#define PD7_GET             ( GPIOD->IDR & BIT7 )
#define PD7_STATE           PD7_GET
#define PD7_HIGH            GPIOD->BSRR = BIT7;
#define PD7_LOW             GPIOD->BRR = BIT7;

#define PD8_ALT             GPIOD->CRH &= ~PIN8_CLEAR; GPIOD->CRH |= PIN8_OUT50_ALTPUSHPULL;
#define PD8_OUT             GPIOD->CRH &= ~PIN8_CLEAR; GPIOD->CRH |= PIN8_OUT50_GPPUSHPULL;
#define PD8_FLOAT           GPIOD->CRH &= ~PIN8_CLEAR; GPIOD->CRH |= PIN8_IN_FLOAT;
#define PD8_PULLUP          GPIOD->CRH &= ~PIN8_CLEAR; GPIOD->CRH |= PIN8_IN_PULL; GPIOD->BSRR = BIT8;
#define PD8_PULLDOWN        GPIOD->CRH &= ~PIN8_CLEAR; GPIOD->CRH |= PIN8_IN_PULL; GPIOD->BRR = BIT8;
#define PD8_ANALOG          GPIOD->CRH &= ~PIN8_CLEAR; GPIOD->CRH |= PIN8_IN_ANALOG;
#define PD8_GET             ( GPIOD->IDR & BIT8 )
#define PD8_STATE           PD8_GET
#define PD8_HIGH            GPIOD->BSRR = BIT8;
#define PD8_LOW             GPIOD->BRR = BIT8;

#define PD9_ALT             GPIOD->CRH &= ~PIN9_CLEAR; GPIOD->CRH |= PIN9_OUT50_ALTPUSHPULL;
#define PD9_OUT             GPIOD->CRH &= ~PIN9_CLEAR; GPIOD->CRH |= PIN9_OUT50_GPPUSHPULL;
#define PD9_FLOAT           GPIOD->CRH &= ~PIN9_CLEAR; GPIOD->CRH |= PIN9_IN_FLOAT;
#define PD9_PULLUP          GPIOD->CRH &= ~PIN9_CLEAR; GPIOD->CRH |= PIN9_IN_PULL; GPIOD->BSRR = BIT9;
#define PD9_PULLDOWN        GPIOD->CRH &= ~PIN9_CLEAR; GPIOD->CRH |= PIN9_IN_PULL; GPIOD->BRR = BIT9;
#define PD9_ANALOG          GPIOD->CRH &= ~PIN9_CLEAR; GPIOD->CRH |= PIN9_IN_ANALOG;
#define PD9_GET             ( GPIOD->IDR & BIT9 )
#define PD9_STATE           PD9_GET
#define PD9_HIGH            GPIOD->BSRR = BIT9;
#define PD9_LOW             GPIOD->BRR = BIT9;

#define PD10_ALT            GPIOD->CRH &= ~PIN10_CLEAR; GPIOD->CRH |= PIN10_OUT50_ALTPUSHPULL;
#define PD10_OUT            GPIOD->CRH &= ~PIN10_CLEAR; GPIOD->CRH |= PIN10_OUT50_GPPUSHPULL;
#define PD10_FLOAT          GPIOD->CRH &= ~PIN10_CLEAR; GPIOD->CRH |= PIN10_IN_FLOAT;
#define PD10_PULLUP         GPIOD->CRH &= ~PIN10_CLEAR; GPIOD->CRH |= PIN10_IN_PULL; GPIOD->BSRR = BIT10;
#define PD10_PULLDOWN       GPIOD->CRH &= ~PIN10_CLEAR; GPIOD->CRH |= PIN10_IN_PULL; GPIOD->BRR = BIT10;
#define PD10_ANALOG         GPIOD->CRH &= ~PIN10_CLEAR; GPIOD->CRH |= PIN10_IN_ANALOG;
#define PD10_GET            ( GPIOD->IDR & BIT10 )
#define PD10_HIGH           GPIOD->BSRR = BIT10;
#define PD10_LOW            GPIOD->BRR = BIT10;
                            
#define PD11_ALT            GPIOD->CRH &= ~PIN11_CLEAR; GPIOD->CRH |= PIN11_OUT50_ALTPUSHPULL;
#define PD11_OUT            GPIOD->CRH &= ~PIN11_CLEAR; GPIOD->CRH |= PIN11_OUT50_GPPUSHPULL;
#define PD11_FLOAT          GPIOD->CRH &= ~PIN11_CLEAR; GPIOD->CRH |= PIN11_IN_FLOAT;
#define PD11_PULLUP         GPIOD->CRH &= ~PIN11_CLEAR; GPIOD->CRH |= PIN11_IN_PULL; GPIOD->BSRR = BIT11;
#define PD11_PULLDOWN       GPIOD->CRH &= ~PIN11_CLEAR; GPIOD->CRH |= PIN11_IN_PULL; GPIOD->BRR = BIT11;
#define PD11_ANALOG         GPIOD->CRH &= ~PIN11_CLEAR; GPIOD->CRH |= PIN11_IN_ANALOG;
#define PD11_GET            ( GPIOD->IDR & BIT11 )
#define PD11_STATE          PD11_GET
#define PD11_HIGH           GPIOD->BSRR = BIT11;
#define PD11_LOW            GPIOD->BRR = BIT11;

#define PD12_ALT            GPIOD->CRH &= ~PIN12_CLEAR; GPIOD->CRH |= PIN12_OUT50_ALTPUSHPULL;
#define PD12_OUT            GPIOD->CRH &= ~PIN12_CLEAR; GPIOD->CRH |= PIN12_OUT50_GPPUSHPULL;
#define PD12_FLOAT          GPIOD->CRH &= ~PIN12_CLEAR; GPIOD->CRH |= PIN12_IN_FLOAT;
#define PD12_PULLUP         GPIOD->CRH &= ~PIN12_CLEAR; GPIOD->CRH |= PIN12_IN_PULL; GPIOD->BSRR = BIT12;
#define PD12_PULLDOWN       GPIOD->CRH &= ~PIN12_CLEAR; GPIOD->CRH |= PIN12_IN_PULL; GPIOD->BRR = BIT12;
#define PD12_ANALOG         GPIOD->CRH &= ~PIN12_CLEAR; GPIOD->CRH |= PIN12_IN_ANALOG;
#define PD12_GET            ( GPIOD->IDR & BIT12 )
#define PD12_STATE          PD12_GET
#define PD12_HIGH           GPIOD->BSRR = BIT12;
#define PD12_LOW            GPIOD->BRR = BIT12;

#define PD13_ALT            GPIOD->CRH &= ~PIN13_CLEAR; GPIOD->CRH |= PIN13_OUT50_ALTPUSHPULL;
#define PD13_OUT            GPIOD->CRH &= ~PIN13_CLEAR; GPIOD->CRH |= PIN13_OUT50_GPPUSHPULL;
#define PD13_FLOAT          GPIOD->CRH &= ~PIN13_CLEAR; GPIOD->CRH |= PIN13_IN_FLOAT;
#define PD13_PULLUP         GPIOD->CRH &= ~PIN13_CLEAR; GPIOD->CRH |= PIN13_IN_PULL; GPIOD->BSRR = BIT13;
#define PD13_PULLDOWN       GPIOD->CRH &= ~PIN13_CLEAR; GPIOD->CRH |= PIN13_IN_PULL; GPIOD->BRR = BIT13;
#define PD13_ANALOG         GPIOD->CRH &= ~PIN13_CLEAR; GPIOD->CRH |= PIN13_IN_ANALOG;
#define PD13_GET            ( GPIOD->IDR & BIT13 )
#define PD13_STATE          PD13_GET
#define PD13_HIGH           GPIOD->BSRR = BIT13;
#define PD13_LOW            GPIOD->BRR = BIT13;

#define PD14_ALT            GPIOD->CRH &= ~PIN14_CLEAR; GPIOD->CRH |= PIN14_OUT50_ALTPUSHPULL;
#define PD14_OUT            GPIOD->CRH &= ~PIN14_CLEAR; GPIOD->CRH |= PIN14_OUT50_GPPUSHPULL;
#define PD14_FLOAT          GPIOD->CRH &= ~PIN14_CLEAR; GPIOD->CRH |= PIN14_IN_FLOAT;
#define PD14_PULLUP         GPIOD->CRH &= ~PIN14_CLEAR; GPIOD->CRH |= PIN14_IN_PULL; GPIOD->BSRR = BIT14;
#define PD14_PULLDOWN       GPIOD->CRH &= ~PIN14_CLEAR; GPIOD->CRH |= PIN14_IN_PULL; GPIOD->BRR = BIT14;
#define PD14_ANALOG         GPIOD->CRH &= ~PIN14_CLEAR; GPIOD->CRH |= PIN14_IN_ANALOG;
#define PD14_GET            ( GPIOD->IDR & BIT14 )
#define PD14_STATE          PD14_GET
#define PD14_HIGH           GPIOD->BSRR = BIT14;
#define PD14_LOW            GPIOD->BRR = BIT14;

#define PD15_ALT            GPIOD->CRH &= ~PIN15_CLEAR; GPIOD->CRH |= PIN15_OUT50_ALTPUSHPULL;
#define PD15_OUT            GPIOD->CRH &= ~PIN15_CLEAR; GPIOD->CRH |= PIN15_OUT50_GPPUSHPULL;
#define PD15_FLOAT          GPIOD->CRH &= ~PIN15_CLEAR; GPIOD->CRH |= PIN15_IN_FLOAT;
#define PD15_PULLUP         GPIOD->CRH &= ~PIN15_CLEAR; GPIOD->CRH |= PIN15_IN_PULL; GPIOD->BSRR = BIT15;
#define PD15_PULLDOWN       GPIOD->CRH &= ~PIN15_CLEAR; GPIOD->CRH |= PIN15_IN_PULL; GPIOD->BRR = BIT15;
#define PD15_ANALOG         GPIOD->CRH &= ~PIN15_CLEAR; GPIOD->CRH |= PIN15_IN_ANALOG;
#define PD15_GET            ( GPIOD->IDR & BIT15 )
#define PD15_STATE          PD15_GET
#define PD15_HIGH           GPIOD->BSRR = BIT15;
#define PD15_LOW            GPIOD->BRR = BIT15;

#ifdef GPIOE
#define PE0_ALT             GPIOE->CRL &= ~PIN0_CLEAR; GPIOE->CRL |= PIN0_OUT50_ALTPUSHPULL;
#define PE0_OUT             GPIOE->CRL &= ~PIN0_CLEAR; GPIOE->CRL |= PIN0_OUT50_GPPUSHPULL;
#define PE0_FLOAT           GPIOE->CRL &= ~PIN0_CLEAR; GPIOE->CRL |= PIN0_IN_FLOAT;
#define PE0_PULLUP          GPIOE->CRL &= ~PIN0_CLEAR; GPIOE->CRL |= PIN0_IN_PULL; GPIOE->BSRR = BIT0;
#define PE0_PULLDOWN        GPIOE->CRL &= ~PIN0_CLEAR; GPIOE->CRL |= PIN0_IN_PULL; GPIOE->BRR = BIT0;
#define PE0_ANALOG          GPIOE->CRL &= ~PIN0_CLEAR; GPIOE->CRL |= PIN0_IN_ANALOG;
#define PE0_GET             ( GPIOE->IDR & BIT0 )
#define PE0_STATE           PE0_GET
#define PE0_HIGH            GPIOE->BSRR = BIT0;
#define PE0_LOW             GPIOE->BRR = BIT0;

#define PE1_ALT             GPIOE->CRL &= ~PIN1_CLEAR; GPIOE->CRL |= PIN1_OUT50_ALTPUSHPULL;
#define PE1_OUT             GPIOE->CRL &= ~PIN1_CLEAR; GPIOE->CRL |= PIN1_OUT50_GPPUSHPULL;
#define PE1_FLOAT           GPIOE->CRL &= ~PIN1_CLEAR; GPIOE->CRL |= PIN1_IN_FLOAT;
#define PE1_PULLUP          GPIOE->CRL &= ~PIN1_CLEAR; GPIOE->CRL |= PIN1_IN_PULL; GPIOE->BSRR = BIT1;
#define PE1_PULLDOWN        GPIOE->CRL &= ~PIN1_CLEAR; GPIOE->CRL |= PIN1_IN_PULL; GPIOE->BRR = BIT1;
#define PE1_ANALOG          GPIOE->CRL &= ~PIN1_CLEAR; GPIOE->CRL |= PIN1_IN_ANALOG;
#define PE1_GET             ( GPIOE->IDR & BIT1 )
#define PE1_STATE           PE1_GET
#define PE1_HIGH            GPIOE->BSRR = BIT1;
#define PE1_LOW             GPIOE->BRR = BIT1;

#define PE2_ALT             GPIOE->CRL &= ~PIN2_CLEAR; GPIOE->CRL |= PIN2_OUT50_ALTPUSHPULL;
#define PE2_OUT             GPIOE->CRL &= ~PIN2_CLEAR; GPIOE->CRL |= PIN2_OUT50_GPPUSHPULL;
#define PE2_FLOAT           GPIOE->CRL &= ~PIN2_CLEAR; GPIOE->CRL |= PIN2_IN_FLOAT;
#define PE2_PULLUP          GPIOE->CRL &= ~PIN2_CLEAR; GPIOE->CRL |= PIN2_IN_PULL; GPIOE->BSRR = BIT2;
#define PE2_PULLDOWN        GPIOE->CRL &= ~PIN2_CLEAR; GPIOE->CRL |= PIN2_IN_PULL; GPIOE->BRR = BIT2;
#define PE2_ANALOG          GPIOE->CRL &= ~PIN2_CLEAR; GPIOE->CRL |= PIN2_IN_ANALOG;
#define PE2_GET             ( GPIOE->IDR & BIT2 )
#define PE2_STATE           PE2_GET
#define PE2_HIGH            GPIOE->BSRR = BIT2;
#define PE2_LOW             GPIOE->BRR = BIT2;

#define PE3_ALT             GPIOE->CRL &= ~PIN3_CLEAR; GPIOE->CRL |= PIN3_OUT50_ALTPUSHPULL;
#define PE3_OUT             GPIOE->CRL &= ~PIN3_CLEAR; GPIOE->CRL |= PIN3_OUT50_GPPUSHPULL;
#define PE3_FLOAT           GPIOE->CRL &= ~PIN3_CLEAR; GPIOE->CRL |= PIN3_IN_FLOAT;
#define PE3_PULLUP          GPIOE->CRL &= ~PIN3_CLEAR; GPIOE->CRL |= PIN3_IN_PULL; GPIOE->BSRR = BIT3;
#define PE3_PULLDOWN        GPIOE->CRL &= ~PIN3_CLEAR; GPIOE->CRL |= PIN3_IN_PULL; GPIOE->BRR = BIT3;
#define PE3_ANALOG          GPIOE->CRL &= ~PIN3_CLEAR; GPIOE->CRL |= PIN3_IN_ANALOG;
#define PE3_GET             ( GPIOE->IDR & BIT3 )
#define PE3_STATE           PE3_GET
#define PE3_HIGH            GPIOE->BSRR = BIT3;
#define PE3_LOW             GPIOE->BRR = BIT3;

#define PE4_ALT             GPIOE->CRL &= ~PIN4_CLEAR; GPIOE->CRL |= PIN4_OUT50_ALTPUSHPULL;
#define PE4_OUT             GPIOE->CRL &= ~PIN4_CLEAR; GPIOE->CRL |= PIN4_OUT50_GPPUSHPULL;
#define PE4_FLOAT           GPIOE->CRL &= ~PIN4_CLEAR; GPIOE->CRL |= PIN4_IN_FLOAT;
#define PE4_PULLUP          GPIOE->CRL &= ~PIN4_CLEAR; GPIOE->CRL |= PIN4_IN_PULL; GPIOE->BSRR = BIT4;
#define PE4_PULLDOWN        GPIOE->CRL &= ~PIN4_CLEAR; GPIOE->CRL |= PIN4_IN_PULL; GPIOE->BRR = BIT4;
#define PE4_ANALOG          GPIOE->CRL &= ~PIN4_CLEAR; GPIOE->CRL |= PIN4_IN_ANALOG;
#define PE4_GET             ( GPIOE->IDR & BIT4 )
#define PE4_STATE           PE4_GET
#define PE4_HIGH            GPIOE->BSRR = BIT4;
#define PE4_LOW             GPIOE->BRR = BIT4;

#define PE5_ALT             GPIOE->CRL &= ~PIN5_CLEAR; GPIOE->CRL |= PIN5_OUT50_ALTPUSHPULL;
#define PE5_OUT             GPIOE->CRL &= ~PIN5_CLEAR; GPIOE->CRL |= PIN5_OUT50_GPPUSHPULL;
#define PE5_FLOAT           GPIOE->CRL &= ~PIN5_CLEAR; GPIOE->CRL |= PIN5_IN_FLOAT;
#define PE5_PULLUP          GPIOE->CRL &= ~PIN5_CLEAR; GPIOE->CRL |= PIN5_IN_PULL; GPIOE->BSRR = BIT5;
#define PE5_PULLDOWN        GPIOE->CRL &= ~PIN5_CLEAR; GPIOE->CRL |= PIN5_IN_PULL; GPIOE->BRR = BIT5;
#define PE5_ANALOG          GPIOE->CRL &= ~PIN5_CLEAR; GPIOE->CRL |= PIN5_IN_ANALOG;
#define PE5_GET             ( GPIOE->IDR & BIT5 )
#define PE5_STATE           PE5_GET
#define PE5_HIGH            GPIOE->BSRR = BIT5;
#define PE5_LOW             GPIOE->BRR = BIT5;

#define PE6_ALT             GPIOE->CRL &= ~PIN6_CLEAR; GPIOE->CRL |= PIN6_OUT50_ALTPUSHPULL;
#define PE6_OUT             GPIOE->CRL &= ~PIN6_CLEAR; GPIOE->CRL |= PIN6_OUT50_GPPUSHPULL;
#define PE6_FLOAT           GPIOE->CRL &= ~PIN6_CLEAR; GPIOE->CRL |= PIN6_IN_FLOAT;
#define PE6_PULLUP          GPIOE->CRL &= ~PIN6_CLEAR; GPIOE->CRL |= PIN6_IN_PULL; GPIOE->BSRR = BIT6;
#define PE6_PULLDOWN        GPIOE->CRL &= ~PIN6_CLEAR; GPIOE->CRL |= PIN6_IN_PULL; GPIOE->BRR = BIT6;
#define PE6_ANALOG          GPIOE->CRL &= ~PIN6_CLEAR; GPIOE->CRL |= PIN6_IN_ANALOG;
#define PE6_GET             ( GPIOE->IDR & BIT6 )
#define PE6_STATE           PE6_GET
#define PE6_HIGH            GPIOE->BSRR = BIT6;
#define PE6_LOW             GPIOE->BRR = BIT6;

#define PE7_ALT             GPIOE->CRL &= ~PIN7_CLEAR; GPIOE->CRL |= PIN7_OUT50_ALTPUSHPULL;
#define PE7_OUT             GPIOE->CRL &= ~PIN7_CLEAR; GPIOE->CRL |= PIN7_OUT50_GPPUSHPULL;
#define PE7_FLOAT           GPIOE->CRL &= ~PIN7_CLEAR; GPIOE->CRL |= PIN7_IN_FLOAT;
#define PE7_PULLUP          GPIOE->CRL &= ~PIN7_CLEAR; GPIOE->CRL |= PIN7_IN_PULL; GPIOE->BSRR = BIT7;
#define PE7_PULLDOWN        GPIOE->CRL &= ~PIN7_CLEAR; GPIOE->CRL |= PIN7_IN_PULL; GPIOE->BRR = BIT7;
#define PE7_ANALOG          GPIOE->CRL &= ~PIN7_CLEAR; GPIOE->CRL |= PIN7_IN_ANALOG;
#define PE7_GET             ( GPIOE->IDR & BIT7 )
#define PE7_STATE           PE7_GET
#define PE7_HIGH            GPIOE->BSRR = BIT7;
#define PE7_LOW             GPIOE->BRR = BIT7;

#define PE8_ALT             GPIOE->CRH &= ~PIN8_CLEAR; GPIOE->CRH |= PIN8_OUT50_ALTPUSHPULL;
#define PE8_OUT             GPIOE->CRH &= ~PIN8_CLEAR; GPIOE->CRH |= PIN8_OUT50_GPPUSHPULL;
#define PE8_FLOAT           GPIOE->CRH &= ~PIN8_CLEAR; GPIOE->CRH |= PIN8_IN_FLOAT;
#define PE8_PULLUP          GPIOE->CRH &= ~PIN8_CLEAR; GPIOE->CRH |= PIN8_IN_PULL; GPIOE->BSRR = BIT8;
#define PE8_PULLDOWN        GPIOE->CRH &= ~PIN8_CLEAR; GPIOE->CRH |= PIN8_IN_PULL; GPIOE->BRR = BIT8;
#define PE8_ANALOG          GPIOE->CRH &= ~PIN8_CLEAR; GPIOE->CRH |= PIN8_IN_ANALOG;
#define PE8_GET             ( GPIOE->IDR & BIT8 )
#define PE8_STATE           PE8_GET
#define PE8_HIGH            GPIOE->BSRR = BIT8;
#define PE8_LOW             GPIOE->BRR = BIT8;

#define PE9_ALT             GPIOE->CRH &= ~PIN9_CLEAR; GPIOE->CRH |= PIN9_OUT50_ALTPUSHPULL;
#define PE9_OUT             GPIOE->CRH &= ~PIN9_CLEAR; GPIOE->CRH |= PIN9_OUT50_GPPUSHPULL;
#define PE9_FLOAT           GPIOE->CRH &= ~PIN9_CLEAR; GPIOE->CRH |= PIN9_IN_FLOAT;
#define PE9_PULLUP          GPIOE->CRH &= ~PIN9_CLEAR; GPIOE->CRH |= PIN9_IN_PULL; GPIOE->BSRR = BIT9;
#define PE9_PULLDOWN        GPIOE->CRH &= ~PIN9_CLEAR; GPIOE->CRH |= PIN9_IN_PULL; GPIOE->BRR = BIT9;
#define PE9_ANALOG          GPIOE->CRH &= ~PIN9_CLEAR; GPIOE->CRH |= PIN9_IN_ANALOG;
#define PE9_GET             ( GPIOE->IDR & BIT9 )
#define PE9_STATE           PE9_GET
#define PE9_HIGH            GPIOE->BSRR = BIT9;
#define PE9_LOW             GPIOE->BRR = BIT9;

#define PE10_ALT            GPIOE->CRH &= ~PIN10_CLEAR; GPIOE->CRH |= PIN10_OUT50_ALTPUSHPULL;
#define PE10_OUT            GPIOE->CRH &= ~PIN10_CLEAR; GPIOE->CRH |= PIN10_OUT50_GPPUSHPULL;
#define PE10_FLOAT          GPIOE->CRH &= ~PIN10_CLEAR; GPIOE->CRH |= PIN10_IN_FLOAT;
#define PE10_PULLUP         GPIOE->CRH &= ~PIN10_CLEAR; GPIOE->CRH |= PIN10_IN_PULL; GPIOE->BSRR = BIT10;
#define PE10_PULLDOWN       GPIOE->CRH &= ~PIN10_CLEAR; GPIOE->CRH |= PIN10_IN_PULL; GPIOE->BRR = BIT10;
#define PE10_ANALOG         GPIOE->CRH &= ~PIN10_CLEAR; GPIOE->CRH |= PIN10_IN_ANALOG;
#define PE10_GET            ( GPIOE->IDR & BIT10 )
#define PE10_STATE          PE10_GET
#define PE10_HIGH           GPIOE->BSRR = BIT10;
#define PE10_LOW            GPIOE->BRR = BIT10;

#define PE11_ALT            GPIOE->CRH &= ~PIN11_CLEAR; GPIOE->CRH |= PIN11_OUT50_ALTPUSHPULL;
#define PE11_OUT            GPIOE->CRH &= ~PIN11_CLEAR; GPIOE->CRH |= PIN11_OUT50_GPPUSHPULL;
#define PE11_FLOAT          GPIOE->CRH &= ~PIN11_CLEAR; GPIOE->CRH |= PIN11_IN_FLOAT;
#define PE11_PULLUP         GPIOE->CRH &= ~PIN11_CLEAR; GPIOE->CRH |= PIN11_IN_PULL; GPIOE->BSRR = BIT11;
#define PE11_PULLDOWN       GPIOE->CRH &= ~PIN11_CLEAR; GPIOE->CRH |= PIN11_IN_PULL; GPIOE->BRR = BIT11;
#define PE11_ANALOG         GPIOE->CRH &= ~PIN11_CLEAR; GPIOE->CRH |= PIN11_IN_ANALOG;
#define PE11_GET            ( GPIOE->IDR & BIT11 )
#define PE11_STATE          PE11_GET
#define PE11_HIGH           GPIOE->BSRR = BIT11;
#define PE11_LOW            GPIOE->BRR = BIT11;

#define PE12_ALT            GPIOE->CRH &= ~PIN12_CLEAR; GPIOE->CRH |= PIN12_OUT50_ALTPUSHPULL;
#define PE12_OUT            GPIOE->CRH &= ~PIN12_CLEAR; GPIOE->CRH |= PIN12_OUT50_GPPUSHPULL;
#define PE12_FLOAT          GPIOE->CRH &= ~PIN12_CLEAR; GPIOE->CRH |= PIN12_IN_FLOAT;
#define PE12_PULLUP         GPIOE->CRH &= ~PIN12_CLEAR; GPIOE->CRH |= PIN12_IN_PULL; GPIOE->BSRR = BIT12;
#define PE12_PULLDOWN       GPIOE->CRH &= ~PIN12_CLEAR; GPIOE->CRH |= PIN12_IN_PULL; GPIOE->BRR = BIT12;
#define PE12_ANALOG         GPIOE->CRH &= ~PIN12_CLEAR; GPIOE->CRH |= PIN12_IN_ANALOG;
#define PE12_GET            ( GPIOE->IDR & BIT12 )
#define PE12_STATE          PE12_GET
#define PE12_HIGH           GPIOE->BSRR = BIT12;
#define PE12_LOW            GPIOE->BRR = BIT12;

#define PE13_ALT            GPIOE->CRH &= ~PIN13_CLEAR; GPIOE->CRH |= PIN13_OUT50_ALTPUSHPULL;
#define PE13_OUT            GPIOE->CRH &= ~PIN13_CLEAR; GPIOE->CRH |= PIN13_OUT50_GPPUSHPULL;
#define PE13_FLOAT          GPIOE->CRH &= ~PIN13_CLEAR; GPIOE->CRH |= PIN13_IN_FLOAT;
#define PE13_PULLUP         GPIOE->CRH &= ~PIN13_CLEAR; GPIOE->CRH |= PIN13_IN_PULL; GPIOE->BSRR = BIT13;
#define PE13_PULLDOWN       GPIOE->CRH &= ~PIN13_CLEAR; GPIOE->CRH |= PIN13_IN_PULL; GPIOE->BRR = BIT13;
#define PE13_ANALOG         GPIOE->CRH &= ~PIN13_CLEAR; GPIOE->CRH |= PIN13_IN_ANALOG;
#define PE13_GET            ( GPIOE->IDR & BIT13 )
#define PE13_HIGH           GPIOE->BSRR = BIT13;
#define PE13_LOW            GPIOE->BRR = BIT13;

#define PE14_ALT            GPIOE->CRH &= ~PIN14_CLEAR; GPIOE->CRH |= PIN14_OUT50_ALTPUSHPULL;
#define PE14_OUT            GPIOE->CRH &= ~PIN14_CLEAR; GPIOE->CRH |= PIN14_OUT50_GPPUSHPULL;
#define PE14_FLOAT          GPIOE->CRH &= ~PIN14_CLEAR; GPIOE->CRH |= PIN14_IN_FLOAT;
#define PE14_PULLUP         GPIOE->CRH &= ~PIN14_CLEAR; GPIOE->CRH |= PIN14_IN_PULL; GPIOE->BSRR = BIT14;
#define PE14_PULLDOWN       GPIOE->CRH &= ~PIN14_CLEAR; GPIOE->CRH |= PIN14_IN_PULL; GPIOE->BRR = BIT14;
#define PE14_ANALOG         GPIOE->CRH &= ~PIN14_CLEAR; GPIOE->CRH |= PIN14_IN_ANALOG;
#define PE14_GET            ( GPIOE->IDR & BIT14 )
#define PE14_STATE          PE14_GET
#define PE14_HIGH           GPIOE->BSRR = BIT14;
#define PE14_LOW            GPIOE->BRR = BIT14;

#define PE15_ALT            GPIOE->CRH &= ~PIN15_CLEAR; GPIOE->CRH |= PIN15_OUT50_ALTPUSHPULL;
#define PE15_OUT            GPIOE->CRH &= ~PIN15_CLEAR; GPIOE->CRH |= PIN15_OUT50_GPPUSHPULL;
#define PE15_FLOAT          GPIOE->CRH &= ~PIN15_CLEAR; GPIOE->CRH |= PIN15_IN_FLOAT;
#define PE15_PULLUP         GPIOE->CRH &= ~PIN15_CLEAR; GPIOE->CRH |= PIN15_IN_PULL; GPIOE->BSRR = BIT15;
#define PE15_PULLDOWN       GPIOE->CRH &= ~PIN15_CLEAR; GPIOE->CRH |= PIN15_IN_PULL; GPIOE->BRR = BIT15;
#define PE15_ANALOG         GPIOE->CRH &= ~PIN15_CLEAR; GPIOE->CRH |= PIN15_IN_ANALOG;
#define PE15_GET            ( GPIOE->IDR & BIT15 )
#define PE15_STATE          PE15_GET
#define PE15_HIGH           GPIOE->BSRR = BIT15;
#define PE15_LOW            GPIOE->BRR = BIT15;
#endif // GPIOE

#ifdef GPIOF
#define PF0_ALT             GPIOF->CRL &= ~PIN0_CLEAR; GPIOF->CRL |= PIN0_OUT50_ALTPUSHPULL;
#define PF0_OUT             GPIOF->CRL &= ~PIN0_CLEAR; GPIOF->CRL |= PIN0_OUT50_GPPUSHPULL;
#define PF0_FLOAT           GPIOF->CRL &= ~PIN0_CLEAR; GPIOF->CRL |= PIN0_IN_FLOAT;
#define PF0_PULLUP          GPIOF->CRL &= ~PIN0_CLEAR; GPIOF->CRL |= PIN0_IN_PULL; GPIOF->BSRR = BIT0;
#define PF0_PULLDOWN        GPIOF->CRL &= ~PIN0_CLEAR; GPIOF->CRL |= PIN0_IN_PULL; GPIOF->BRR = BIT0;
#define PF0_ANALOG          GPIOF->CRL &= ~PIN0_CLEAR; GPIOF->CRL |= PIN0_IN_ANALOG;
#define PF0_GET             ( GPIOF->IDR & BIT0 )
#define PF0_STATE           PF0_GET
#define PF0_HIGH            GPIOF->BSRR = BIT0;
#define PF0_LOW             GPIOF->BRR = BIT0;

#define PF1_ALT             GPIOF->CRL &= ~PIN1_CLEAR; GPIOF->CRL |= PIN1_OUT50_ALTPUSHPULL;
#define PF1_OUT             GPIOF->CRL &= ~PIN1_CLEAR; GPIOF->CRL |= PIN1_OUT50_GPPUSHPULL;
#define PF1_FLOAT           GPIOF->CRL &= ~PIN1_CLEAR; GPIOF->CRL |= PIN1_IN_FLOAT;
#define PF1_PULLUP          GPIOF->CRL &= ~PIN1_CLEAR; GPIOF->CRL |= PIN1_IN_PULL; GPIOF->BSRR = BIT1;
#define PF1_PULLDOWN        GPIOF->CRL &= ~PIN1_CLEAR; GPIOF->CRL |= PIN1_IN_PULL; GPIOF->BRR = BIT1;
#define PF1_ANALOG          GPIOF->CRL &= ~PIN1_CLEAR; GPIOF->CRL |= PIN1_IN_ANALOG;
#define PF1_GET             ( GPIOF->IDR & BIT1 )
#define PF1_STATE           PF1_GET
#define PF1_HIGH            GPIOF->BSRR = BIT1;
#define PF1_LOW             GPIOF->BRR = BIT1;

#define PF2_ALT             GPIOF->CRL &= ~PIN2_CLEAR; GPIOF->CRL |= PIN2_OUT50_ALTPUSHPULL;
#define PF2_OUT             GPIOF->CRL &= ~PIN2_CLEAR; GPIOF->CRL |= PIN2_OUT50_GPPUSHPULL;
#define PF2_FLOAT           GPIOF->CRL &= ~PIN2_CLEAR; GPIOF->CRL |= PIN2_IN_FLOAT;
#define PF2_PULLUP          GPIOF->CRL &= ~PIN2_CLEAR; GPIOF->CRL |= PIN2_IN_PULL; GPIOF->BSRR = BIT2;
#define PF2_PULLDOWN        GPIOF->CRL &= ~PIN2_CLEAR; GPIOF->CRL |= PIN2_IN_PULL; GPIOF->BRR = BIT2;
#define PF2_ANALOG          GPIOF->CRL &= ~PIN2_CLEAR; GPIOF->CRL |= PIN2_IN_ANALOG;
#define PF2_GET             ( GPIOF->IDR & BIT2 )
#define PF2_STATE           PF2_GET
#define PF2_HIGH            GPIOF->BSRR = BIT2;
#define PF2_LOW             GPIOF->BRR = BIT2;

#define PF3_ALT             GPIOF->CRL &= ~PIN3_CLEAR; GPIOF->CRL |= PIN3_OUT50_ALTPUSHPULL;
#define PF3_OUT             GPIOF->CRL &= ~PIN3_CLEAR; GPIOF->CRL |= PIN3_OUT50_GPPUSHPULL;
#define PF3_FLOAT           GPIOF->CRL &= ~PIN3_CLEAR; GPIOF->CRL |= PIN3_IN_FLOAT;
#define PF3_PULLUP          GPIOF->CRL &= ~PIN3_CLEAR; GPIOF->CRL |= PIN3_IN_PULL; GPIOF->BSRR = BIT3;
#define PF3_PULLDOWN        GPIOF->CRL &= ~PIN3_CLEAR; GPIOF->CRL |= PIN3_IN_PULL; GPIOF->BRR = BIT3;
#define PF3_ANALOG          GPIOF->CRL &= ~PIN3_CLEAR; GPIOF->CRL |= PIN3_IN_ANALOG;
#define PF3_GET             ( GPIOF->IDR & BIT3 )
#define PF3_STATE           PF3_GET
#define PF3_HIGH            GPIOF->BSRR = BIT3;
#define PF3_LOW             GPIOF->BRR = BIT3;

#define PF4_ALT             GPIOF->CRL &= ~PIN4_CLEAR; GPIOF->CRL |= PIN4_OUT50_ALTPUSHPULL;
#define PF4_OUT             GPIOF->CRL &= ~PIN4_CLEAR; GPIOF->CRL |= PIN4_OUT50_GPPUSHPULL;
#define PF4_FLOAT           GPIOF->CRL &= ~PIN4_CLEAR; GPIOF->CRL |= PIN4_IN_FLOAT;
#define PF4_PULLUP          GPIOF->CRL &= ~PIN4_CLEAR; GPIOF->CRL |= PIN4_IN_PULL; GPIOF->BSRR = BIT4;
#define PF4_PULLDOWN        GPIOF->CRL &= ~PIN4_CLEAR; GPIOF->CRL |= PIN4_IN_PULL; GPIOF->BRR = BIT4;
#define PF4_ANALOG          GPIOF->CRL &= ~PIN4_CLEAR; GPIOF->CRL |= PIN4_IN_ANALOG;
#define PF4_GET             ( GPIOF->IDR & BIT4 )
#define PF4_STATE           PF4_GET
#define PF4_HIGH            GPIOF->BSRR = BIT4;
#define PF4_LOW             GPIOF->BRR = BIT4;

#define PF5_ALT             GPIOF->CRL &= ~PIN5_CLEAR; GPIOF->CRL |= PIN5_OUT50_ALTPUSHPULL;
#define PF5_OUT             GPIOF->CRL &= ~PIN5_CLEAR; GPIOF->CRL |= PIN5_OUT50_GPPUSHPULL;
#define PF5_FLOAT           GPIOF->CRL &= ~PIN5_CLEAR; GPIOF->CRL |= PIN5_IN_FLOAT;
#define PF5_PULLUP          GPIOF->CRL &= ~PIN5_CLEAR; GPIOF->CRL |= PIN5_IN_PULL; GPIOF->BSRR = BIT5;
#define PF5_PULLDOWN        GPIOF->CRL &= ~PIN5_CLEAR; GPIOF->CRL |= PIN5_IN_PULL; GPIOF->BRR = BIT5;
#define PF5_ANALOG          GPIOF->CRL &= ~PIN5_CLEAR; GPIOF->CRL |= PIN5_IN_ANALOG;
#define PF5_GET             ( GPIOF->IDR & BIT5 )
#define PF5_STATE           PF5_GET
#define PF5_HIGH            GPIOF->BSRR = BIT5;
#define PF5_LOW             GPIOF->BRR = BIT5;

#define PF6_ALT             GPIOF->CRL &= ~PIN6_CLEAR; GPIOF->CRL |= PIN6_OUT50_ALTPUSHPULL;
#define PF6_OUT             GPIOF->CRL &= ~PIN6_CLEAR; GPIOF->CRL |= PIN6_OUT50_GPPUSHPULL;
#define PF6_FLOAT           GPIOF->CRL &= ~PIN6_CLEAR; GPIOF->CRL |= PIN6_IN_FLOAT;
#define PF6_PULLUP          GPIOF->CRL &= ~PIN6_CLEAR; GPIOF->CRL |= PIN6_IN_PULL; GPIOF->BSRR = BIT6;
#define PF6_PULLDOWN        GPIOF->CRL &= ~PIN6_CLEAR; GPIOF->CRL |= PIN6_IN_PULL; GPIOF->BRR = BIT6;
#define PF6_ANALOG          GPIOF->CRL &= ~PIN6_CLEAR; GPIOF->CRL |= PIN6_IN_ANALOG;
#define PF6_GET             ( GPIOF->IDR & BIT6 )
#define PF6_STATE           PF6_GET
#define PF6_HIGH            GPIOF->BSRR = BIT6;
#define PF6_LOW             GPIOF->BRR = BIT6;

#define PF7_ALT             GPIOF->CRL &= ~PIN7_CLEAR; GPIOF->CRL |= PIN7_OUT50_ALTPUSHPULL;
#define PF7_OUT             GPIOF->CRL &= ~PIN7_CLEAR; GPIOF->CRL |= PIN7_OUT50_GPPUSHPULL;
#define PF7_FLOAT           GPIOF->CRL &= ~PIN7_CLEAR; GPIOF->CRL |= PIN7_IN_FLOAT;
#define PF7_PULLUP          GPIOF->CRL &= ~PIN7_CLEAR; GPIOF->CRL |= PIN7_IN_PULL; GPIOF->BSRR = BIT7;
#define PF7_PULLDOWN        GPIOF->CRL &= ~PIN7_CLEAR; GPIOF->CRL |= PIN7_IN_PULL; GPIOF->BRR = BIT7;
#define PF7_ANALOG          GPIOF->CRL &= ~PIN7_CLEAR; GPIOF->CRL |= PIN7_IN_ANALOG;
#define PF7_GET             ( GPIOF->IDR & BIT7 )
#define PF7_STATE           PF7_GET
#define PF7_HIGH            GPIOF->BSRR = BIT7;
#define PF7_LOW             GPIOF->BRR = BIT7;

#define PF8_ALT             GPIOF->CRH &= ~PIN8_CLEAR; GPIOF->CRH |= PIN8_OUT50_ALTPUSHPULL;
#define PF8_OUT             GPIOF->CRH &= ~PIN8_CLEAR; GPIOF->CRH |= PIN8_OUT50_GPPUSHPULL;
#define PF8_FLOAT           GPIOF->CRH &= ~PIN8_CLEAR; GPIOF->CRH |= PIN8_IN_FLOAT;
#define PF8_PULLUP          GPIOF->CRH &= ~PIN8_CLEAR; GPIOF->CRH |= PIN8_IN_PULL; GPIOF->BSRR = BIT8;
#define PF8_PULLDOWN        GPIOF->CRH &= ~PIN8_CLEAR; GPIOF->CRH |= PIN8_IN_PULL; GPIOF->BRR = BIT8;
#define PF8_ANALOG          GPIOF->CRH &= ~PIN8_CLEAR; GPIOF->CRH |= PIN8_IN_ANALOG;
#define PF8_GET             ( GPIOF->IDR & BIT8 )
#define PF8_STATE           PF8_GET
#define PF8_HIGH            GPIOF->BSRR = BIT8;
#define PF8_LOW             GPIOF->BRR = BIT8;

#define PF9_ALT             GPIOF->CRH &= ~PIN9_CLEAR; GPIOF->CRH |= PIN9_OUT50_ALTPUSHPULL;
#define PF9_OUT             GPIOF->CRH &= ~PIN9_CLEAR; GPIOF->CRH |= PIN9_OUT50_GPPUSHPULL;
#define PF9_FLOAT           GPIOF->CRH &= ~PIN9_CLEAR; GPIOF->CRH |= PIN9_IN_FLOAT;
#define PF9_PULLUP          GPIOF->CRH &= ~PIN9_CLEAR; GPIOF->CRH |= PIN9_IN_PULL; GPIOF->BSRR = BIT9;
#define PF9_PULLDOWN        GPIOF->CRH &= ~PIN9_CLEAR; GPIOF->CRH |= PIN9_IN_PULL; GPIOF->BRR = BIT9;
#define PF9_ANALOG          GPIOF->CRH &= ~PIN9_CLEAR; GPIOF->CRH |= PIN9_IN_ANALOG;
#define PF9_GET             ( GPIOF->IDR & BIT9 )
#define PF9_STATE           PF9_GET
#define PF9_HIGH            GPIOF->BSRR = BIT9;
#define PF9_LOW             GPIOF->BRR = BIT9;

#define PF10_ALT            GPIOF->CRH &= ~PIN10_CLEAR; GPIOF->CRH |= PIN10_OUT50_ALTPUSHPULL;
#define PF10_OUT            GPIOF->CRH &= ~PIN10_CLEAR; GPIOF->CRH |= PIN10_OUT50_GPPUSHPULL;
#define PF10_FLOAT          GPIOF->CRH &= ~PIN10_CLEAR; GPIOF->CRH |= PIN10_IN_FLOAT;
#define PF10_PULLUP         GPIOF->CRH &= ~PIN10_CLEAR; GPIOF->CRH |= PIN10_IN_PULL; GPIOF->BSRR = BIT10;
#define PF10_PULLDOWN       GPIOF->CRH &= ~PIN10_CLEAR; GPIOF->CRH |= PIN10_IN_PULL; GPIOF->BRR = BIT10;
#define PF10_ANALOG         GPIOF->CRH &= ~PIN10_CLEAR; GPIOF->CRH |= PIN10_IN_ANALOG;
#define PF10_GET            ( GPIOF->IDR & BIT10 )
#define PF10_STATE          PF10_GET
#define PF10_HIGH           GPIOF->BSRR = BIT10;
#define PF10_LOW            GPIOF->BRR = BIT10;

#define PF11_ALT            GPIOF->CRH &= ~PIN11_CLEAR; GPIOF->CRH |= PIN11_OUT50_ALTPUSHPULL;
#define PF11_OUT            GPIOF->CRH &= ~PIN11_CLEAR; GPIOF->CRH |= PIN11_OUT50_GPPUSHPULL;
#define PF11_FLOAT          GPIOF->CRH &= ~PIN11_CLEAR; GPIOF->CRH |= PIN11_IN_FLOAT;
#define PF11_PULLUP         GPIOF->CRH &= ~PIN11_CLEAR; GPIOF->CRH |= PIN11_IN_PULL; GPIOF->BSRR = BIT11;
#define PF11_PULLDOWN       GPIOF->CRH &= ~PIN11_CLEAR; GPIOF->CRH |= PIN11_IN_PULL; GPIOF->BRR = BIT11;
#define PF11_ANALOG         GPIOF->CRH &= ~PIN11_CLEAR; GPIOF->CRH |= PIN11_IN_ANALOG;
#define PF11_GET            ( GPIOF->IDR & BIT11 )
#define PF11_STATE          PF11_GET
#define PF11_HIGH           GPIOF->BSRR = BIT11;
#define PF11_LOW            GPIOF->BRR = BIT11;

#define PF12_ALT            GPIOF->CRH &= ~PIN12_CLEAR; GPIOF->CRH |= PIN12_OUT50_ALTPUSHPULL;
#define PF12_OUT            GPIOF->CRH &= ~PIN12_CLEAR; GPIOF->CRH |= PIN12_OUT50_GPPUSHPULL;
#define PF12_FLOAT          GPIOF->CRH &= ~PIN12_CLEAR; GPIOF->CRH |= PIN12_IN_FLOAT;
#define PF12_PULLUP         GPIOF->CRH &= ~PIN12_CLEAR; GPIOF->CRH |= PIN12_IN_PULL; GPIOF->BSRR = BIT12;
#define PF12_PULLDOWN       GPIOF->CRH &= ~PIN12_CLEAR; GPIOF->CRH |= PIN12_IN_PULL; GPIOF->BRR = BIT12;
#define PF12_ANALOG         GPIOF->CRH &= ~PIN12_CLEAR; GPIOF->CRH |= PIN12_IN_ANALOG;
#define PF12_GET            ( GPIOF->IDR & BIT12 )
#define PF12_STATE          PF12_GET
#define PF12_HIGH           GPIOF->BSRR = BIT12;
#define PF12_LOW            GPIOF->BRR = BIT12;

#define PF13_ALT            GPIOF->CRH &= ~PIN13_CLEAR; GPIOF->CRH |= PIN13_OUT50_ALTPUSHPULL;
#define PF13_OUT            GPIOF->CRH &= ~PIN13_CLEAR; GPIOF->CRH |= PIN13_OUT50_GPPUSHPULL;
#define PF13_FLOAT          GPIOF->CRH &= ~PIN13_CLEAR; GPIOF->CRH |= PIN13_IN_FLOAT;
#define PF13_PULLUP         GPIOF->CRH &= ~PIN13_CLEAR; GPIOF->CRH |= PIN13_IN_PULL; GPIOF->BSRR = BIT13;
#define PF13_PULLDOWN       GPIOF->CRH &= ~PIN13_CLEAR; GPIOF->CRH |= PIN13_IN_PULL; GPIOF->BRR = BIT13;
#define PF13_ANALOG         GPIOF->CRH &= ~PIN13_CLEAR; GPIOF->CRH |= PIN13_IN_ANALOG;
#define PF13_GET            ( GPIOF->IDR & BIT13 )
#define PF13_STATE          PF13_GET
#define PF13_HIGH           GPIOF->BSRR = BIT13;
#define PF13_LOW            GPIOF->BRR = BIT13;

#define PF14_ALT            GPIOF->CRH &= ~PIN14_CLEAR; GPIOF->CRH |= PIN14_OUT50_ALTPUSHPULL;
#define PF14_OUT            GPIOF->CRH &= ~PIN14_CLEAR; GPIOF->CRH |= PIN14_OUT50_GPPUSHPULL;
#define PF14_FLOAT          GPIOF->CRH &= ~PIN14_CLEAR; GPIOF->CRH |= PIN14_IN_FLOAT;
#define PF14_PULLUP         GPIOF->CRH &= ~PIN14_CLEAR; GPIOF->CRH |= PIN14_IN_PULL; GPIOF->BSRR = BIT14;
#define PF14_PULLDOWN       GPIOF->CRH &= ~PIN14_CLEAR; GPIOF->CRH |= PIN14_IN_PULL; GPIOF->BRR = BIT14;
#define PF14_ANALOG         GPIOF->CRH &= ~PIN14_CLEAR; GPIOF->CRH |= PIN14_IN_ANALOG;
#define PF14_GET            ( GPIOF->IDR & BIT14 )
#define PF14_STATE          PF14_GET
#define PF14_HIGH           GPIOF->BSRR = BIT14;
#define PF14_LOW            GPIOF->BRR = BIT14;

#define PF15_ALT            GPIOF->CRH &= ~PIN15_CLEAR; GPIOF->CRH |= PIN15_OUT50_ALTPUSHPULL;
#define PF15_OUT            GPIOF->CRH &= ~PIN15_CLEAR; GPIOF->CRH |= PIN15_OUT50_GPPUSHPULL;
#define PF15_FLOAT          GPIOF->CRH &= ~PIN15_CLEAR; GPIOF->CRH |= PIN15_IN_FLOAT;
#define PF15_PULLUP         GPIOF->CRH &= ~PIN15_CLEAR; GPIOF->CRH |= PIN15_IN_PULL; GPIOF->BSRR = BIT15;
#define PF15_PULLDOWN       GPIOF->CRH &= ~PIN15_CLEAR; GPIOF->CRH |= PIN15_IN_PULL; GPIOF->BRR = BIT15;
#define PF15_ANALOG         GPIOF->CRH &= ~PIN15_CLEAR; GPIOF->CRH |= PIN15_IN_ANALOG;
#define PF15_GET            ( GPIOF->IDR & BIT15 )
#define PF15_STATE          PF15_GET
#define PF15_HIGH           GPIOF->BSRR = BIT15;
#define PF15_LOW            GPIOF->BRR = BIT15;
                            
#endif // GPIOF

#define PORTA_ALT            PORTA_MSB_ALT; PORTA_LSB_ALT;       
#define PORTA_OUT            PORTA_MSB_OUT; PORTA_LSB_OUT;                       
#define PORTA_FLOAT          PORTA_MSB_FLOAT; PORTA_LSB_FLOAT;
#define PORTA_PULLUP         PORTA_MSB_PULLUP; PORTA_LSB_PULLUP;
#define PORTA_PULLDOWN       PORTA_MSB_PULLDOWN; PORTA_LSB_PULLDOWN;
#define PORTA_ANALOG         PORTA_MSB_ANALOG; PORTA_LSB_ANALOG;
#define PORTA_GET            ( GPIOA->IDR )
#define PORTA_HIGH           GPIOA->ODR = 0xFFFF;
#define PORTA_LOW            GPIOA->ODR = 0x0000;
#define PORTA_SET( pVal )    GPIOA->ODR = pVal;

#define PORTA_LSB_ALT         GPIOA->CRL = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTA_LSB_OUT         GPIOA->CRL = ALLPINS_OUT50_GPPUSHPULL;
#define PORTA_LSB_FLOAT       GPIOA->CRL = ALLPINS_IN_FLOAT;  
#define PORTA_LSB_PULLUP      GPIOA->CRL = ALLPINS_IN_PULL; GPIOA->BSRR = 0x00FF;
#define PORTA_LSB_PULLDOWN    GPIOA->CRL = ALLPINS_IN_PULL; GPIOA->BRR = 0x00FF;
#define PORTA_LSB_ANALOG      GPIOA->CRL = ALLPINS_IN_ANALOG;  
#define PORTA_LSB_GET         ( GPIOA->IDR & 0x00FF )  
#define PORTA_LSB_HIGH        GPIOA->BSRR = 0x00FF;  
#define PORTA_LSB_LOW         GPIOA->BRR = 0x00FF;   
#define PORTA_LSB_SET( pVal ) GPIOA->BRR = 0x00FF; GPIOA->BSRR = pVal;

#define PORTA_MSB_ALT         GPIOA->CRH = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTA_MSB_OUT         GPIOA->CRH = ALLPINS_OUT50_GPPUSHPULL;
#define PORTA_MSB_FLOAT       GPIOA->CRH = ALLPINS_IN_FLOAT;  
#define PORTA_MSB_PULLUP      GPIOA->CRH = ALLPINS_IN_PULL; GPIOA->BSRR = 0xFF00;
#define PORTA_MSB_PULLDOWN    GPIOA->CRH = ALLPINS_IN_PULL; GPIOA->BRR = 0xFF00;  
#define PORTA_MSB_ANALOG      GPIOA->CRH = ALLPINS_IN_ANALOG;  
#define PORTA_MSB_GET         ( GPIOA->IDR >> 8 )
#define PORTA_MSB_HIGH        GPIOA->BSRR = 0xFF00;  
#define PORTA_MSB_LOW         GPIOA->BRR = 0xFF00;  
#define PORTA_MSB_SET( pVal ) GPIOA->BRR = 0xFF00; GPIOA->BSRR = ( pVal << 8 );        

#define PORTB_ALT            PORTB_MSB_ALT; PORTB_LSB_ALT;       
#define PORTB_OUT            PORTB_MSB_OUT; PORTB_LSB_OUT;                       
#define PORTB_FLOAT          PORTB_MSB_FLOAT; PORTB_LSB_FLOAT;
#define PORTB_PULLUP         PORTB_MSB_PULLUP; PORTB_LSB_PULLUP;
#define PORTB_PULLDOWN       PORTB_MSB_PULLDOWN; PORTB_LSB_PULLDOWN;
#define PORTB_ANALOG         PORTB_MSB_ANALOG; PORTB_LSB_ANALOG;
#define PORTB_GET            ( GPIOB->IDR )
#define PORTB_HIGH           GPIOB->ODR = 0xFFFF;
#define PORTB_LOW            GPIOB->ODR = 0x0000;
#define PORTB_SET( pVal )    GPIOB->ODR = pVal;

#define PORTB_LSB_ALT         GPIOB->CRL = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTB_LSB_OUT         GPIOB->CRL = ALLPINS_OUT50_GPPUSHPULL;
#define PORTB_LSB_FLOAT       GPIOB->CRL = ALLPINS_IN_FLOAT;  
#define PORTB_LSB_PULLUP      GPIOB->CRL = ALLPINS_IN_PULL; GPIOA->BSRR = 0x00FF;
#define PORTB_LSB_PULLDOWN    GPIOB->CRL = ALLPINS_IN_PULL; GPIOA->BRR = 0x00FF;
#define PORTB_LSB_ANALOG      GPIOB->CRL = ALLPINS_IN_ANALOG;  
#define PORTB_LSB_GET         ( GPIOB->IDR & 0x00FF )  
#define PORTB_LSB_HIGH        GPIOB->BSRR = 0x00FF;  
#define PORTB_LSB_LOW         GPIOB->BRR = 0x00FF;   
#define PORTB_LSB_SET( pVal ) GPIOB->BRR = 0x00FF; GPIOB->BSRR = pVal;

#define PORTB_MSB_ALT         GPIOB->CRH = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTB_MSB_OUT         GPIOB->CRH = ALLPINS_OUT50_GPPUSHPULL;
#define PORTB_MSB_FLOAT       GPIOB->CRH = ALLPINS_IN_FLOAT;  
#define PORTB_MSB_PULLUP      GPIOB->CRH = ALLPINS_IN_PULL; GPIOB->BSRR = 0xFF00;
#define PORTB_MSB_PULLDOWN    GPIOB->CRH = ALLPINS_IN_PULL; GPIOB->BRR = 0xFF00;  
#define PORTB_MSB_ANALOG      GPIOB->CRH = ALLPINS_IN_ANALOG;  
#define PORTB_MSB_GET         ( GPIOB->IDR >> 8 )
#define PORTB_MSB_HIGH        GPIOB->BSRR = 0xFF00;  
#define PORTB_MSB_LOW         GPIOB->BRR = 0xFF00;  
#define PORTB_MSB_SET( pVal ) GPIOB->BRR = 0xFF00; GPIOB->BSRR = ( pVal << 8 );

#define PORTC_ALT            PORTC_MSB_ALT; PORTC_LSB_ALT;       
#define PORTC_OUT            PORTC_MSB_OUT; PORTC_LSB_OUT;                       
#define PORTC_FLOAT          PORTC_MSB_FLOAT; PORTC_LSB_FLOAT;
#define PORTC_PULLUP         PORTC_MSB_PULLUP; PORTC_LSB_PULLUP;
#define PORTC_PULLDOWN       PORTC_MSB_PULLDOWN; PORTC_LSB_PULLDOWN;
#define PORTC_ANALOG         PORTC_MSB_ANALOG; PORTC_LSB_ANALOG;
#define PORTC_GET            ( GPIOC->IDR )
#define PORTC_HIGH           GPIOC->ODR = 0xFFFF;
#define PORTC_LOW            GPIOC->ODR = 0x0000;
#define PORTC_SET( pVal )    GPIOC->ODR = pVal;

#define PORTC_LSB_ALT         GPIOC->CRL = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTC_LSB_OUT         GPIOC->CRL = ALLPINS_OUT50_GPPUSHPULL;
#define PORTC_LSB_FLOAT       GPIOC->CRL = ALLPINS_IN_FLOAT;  
#define PORTC_LSB_PULLUP      GPIOC->CRL = ALLPINS_IN_PULL; GPIOC->BSRR = 0x00FF;
#define PORTC_LSB_PULLDOWN    GPIOC->CRL = ALLPINS_IN_PULL; GPIOC->BRR = 0x00FF;
#define PORTC_LSB_ANALOG      GPIOC->CRL = ALLPINS_IN_ANALOG;  
#define PORTC_LSB_GET         ( GPIOC->IDR & 0x00FF )  
#define PORTC_LSB_HIGH        GPIOC->BSRR = 0x00FF;  
#define PORTC_LSB_LOW         GPIOC->BRR = 0x00FF;   
#define PORTC_LSB_SET( pVal ) GPIOC->BRR = 0x00FF; GPIOC->BSRR = pVal;

#define PORTC_MSB_ALT         GPIOC->CRH = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTC_MSB_OUT         GPIOC->CRH = ALLPINS_OUT50_GPPUSHPULL;
#define PORTC_MSB_FLOAT       GPIOC->CRH = ALLPINS_IN_FLOAT;  
#define PORTC_MSB_PULLUP      GPIOC->CRH = ALLPINS_IN_PULL; GPIOC->BSRR = 0xFF00;
#define PORTC_MSB_PULLDOWN    GPIOC->CRH = ALLPINS_IN_PULL; GPIOC->BRR = 0xFF00;  
#define PORTC_MSB_ANALOG      GPIOC->CRH = ALLPINS_IN_ANALOG;  
#define PORTC_MSB_GET         ( GPIOC->IDR >> 8 )
#define PORTC_MSB_HIGH        GPIOC->BSRR = 0xFF00;  
#define PORTC_MSB_LOW         GPIOC->BRR = 0xFF00;  
#define PORTC_MSB_SET( pVal ) GPIOC->BRR = 0xFF00; GPIOC->BSRR = ( pVal << 8 );

#define PORTD_ALT            PORTD_MSB_ALT; PORTD_LSB_ALT;       
#define PORTD_OUT            PORTD_MSB_OUT; PORTD_LSB_OUT;                       
#define PORTD_FLOAT          PORTD_MSB_FLOAT; PORTD_LSB_FLOAT;
#define PORTD_PULLUP         PORTD_MSB_PULLUP; PORTD_LSB_PULLUP;
#define PORTD_PULLDOWN       PORTD_MSB_PULLDOWN; PORTD_LSB_PULLDOWN;
#define PORTD_ANALOG         PORTD_MSB_ANALOG; PORTD_LSB_ANALOG;
#define PORTD_GET            ( GPIOD->IDR )
#define PORTD_HIGH           GPIOD->ODR = 0xFFFF;
#define PORTD_LOW            GPIOD->ODR = 0x0000;
#define PORTD_SET( pVal )    GPIOD->ODR = pVal;

#define PORTD_LSB_ALT         GPIOD->CRL = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTD_LSB_OUT         GPIOD->CRL = ALLPINS_OUT50_GPPUSHPULL;
#define PORTD_LSB_FLOAT       GPIOD->CRL = ALLPINS_IN_FLOAT;  
#define PORTD_LSB_PULLUP      GPIOD->CRL = ALLPINS_IN_PULL; GPIOD->BSRR = 0x00FF;
#define PORTD_LSB_PULLDOWN    GPIOD->CRL = ALLPINS_IN_PULL; GPIOD->BRR = 0x00FF;
#define PORTD_LSB_ANALOG      GPIOD->CRL = ALLPINS_IN_ANALOG;  
#define PORTD_LSB_GET         ( GPIOD->IDR & 0x00FF )  
#define PORTD_LSB_HIGH        GPIOD->BSRR = 0x00FF;  
#define PORTD_LSB_LOW         GPIOD->BRR = 0x00FF;   
#define PORTD_LSB_SET( pVal ) GPIOD->BRR = 0x00FF; GPIOD->BSRR = pVal;

#define PORTD_MSB_ALT         GPIOD->CRH = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTD_MSB_OUT         GPIOD->CRH = ALLPINS_OUT50_GPPUSHPULL;
#define PORTD_MSB_FLOAT       GPIOD->CRH = ALLPINS_IN_FLOAT;  
#define PORTD_MSB_PULLUP      GPIOD->CRH = ALLPINS_IN_PULL; GPIOD->BSRR = 0xFF00;
#define PORTD_MSB_PULLDOWN    GPIOD->CRH = ALLPINS_IN_PULL; GPIOD->BRR = 0xFF00;  
#define PORTD_MSB_ANALOG      GPIOD->CRH = ALLPINS_IN_ANALOG;  
#define PORTD_MSB_GET         ( GPIOD->IDR >> 8 )
#define PORTD_MSB_HIGH        GPIOD->BSRR = 0xFF00;  
#define PORTD_MSB_LOW         GPIOD->BRR = 0xFF00;  
#define PORTD_MSB_SET( pVal ) GPIOD->BRR = 0xFF00; GPIOD->BSRR = ( pVal << 8 );

#ifdef GPIOE
#define PORTE_ALT            PORTE_MSB_ALT; PORTE_LSB_ALT;       
#define PORTE_OUT            PORTE_MSB_OUT; PORTE_LSB_OUT;                       
#define PORTE_FLOAT          PORTE_MSB_FLOAT; PORTE_LSB_FLOAT;
#define PORTE_PULLUP         PORTE_MSB_PULLUP; PORTE_LSB_PULLUP;
#define PORTE_PULLDOWN       PORTE_MSB_PULLDOWN; PORTE_LSB_PULLDOWN;
#define PORTE_ANALOG         PORTE_MSB_ANALOG; PORTE_LSB_ANALOG;
#define PORTE_GET            ( GPIOE->IDR )
#define PORTE_HIGH           GPIOE->ODR = 0xFFFF;
#define PORTE_LOW            GPIOE->ODR = 0x0000;
#define PORTE_SET( pVal )    GPIOE->ODR = pVal;

#define PORTE_LSB_ALT         GPIOE->CRL = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTE_LSB_OUT         GPIOE->CRL = ALLPINS_OUT50_GPPUSHPULL;
#define PORTE_LSB_FLOAT       GPIOE->CRL = ALLPINS_IN_FLOAT;  
#define PORTE_LSB_PULLUP      GPIOE->CRL = ALLPINS_IN_PULL; GPIOE->BSRR = 0x00FF;
#define PORTE_LSB_PULLDOWN    GPIOE->CRL = ALLPINS_IN_PULL; GPIOE->BRR = 0x00FF;
#define PORTE_LSB_ANALOG      GPIOE->CRL = ALLPINS_IN_ANALOG;  
#define PORTE_LSB_GET         ( GPIOE->IDR & 0x00FF )  
#define PORTE_LSB_HIGH        GPIOE->BSRR = 0x00FF;  
#define PORTE_LSB_LOW         GPIOE->BRR = 0x00FF;   
#define PORTE_LSB_SET( pVal ) GPIOE->BRR = 0x00FF; GPIOE->BSRR = pVal;

#define PORTE_MSB_ALT         GPIOE->CRH = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTE_MSB_OUT         GPIOE->CRH = ALLPINS_OUT50_GPPUSHPULL;
#define PORTE_MSB_FLOAT       GPIOE->CRH = ALLPINS_IN_FLOAT;  
#define PORTE_MSB_PULLUP      GPIOE->CRH = ALLPINS_IN_PULL; GPIOE->BSRR = 0xFF00;
#define PORTE_MSB_PULLDOWN    GPIOE->CRH = ALLPINS_IN_PULL; GPIOE->BRR = 0xFF00;  
#define PORTE_MSB_ANALOG      GPIOE->CRH = ALLPINS_IN_ANALOG;  
#define PORTE_MSB_GET         ( GPIOE->IDR >> 8 )
#define PORTE_MSB_HIGH        GPIOE->BSRR = 0xFF00;  
#define PORTE_MSB_LOW         GPIOE->BRR = 0xFF00;  
#define PORTE_MSB_SET( pVal ) GPIOE->BRR = 0xFF00; GPIOE->BSRR = ( pVal << 8 );
#endif // GPIOE

#ifdef GPIOF
#define PORTF_ALT            PORTF_MSB_ALT; PORTF_LSB_ALT;       
#define PORTF_OUT            PORTF_MSB_OUT; PORTF_LSB_OUT;                       
#define PORTF_FLOAT          PORTF_MSB_FLOAT; PORTF_LSB_FLOAT;
#define PORTF_PULLUP         PORTF_MSB_PULLUP; PORTF_LSB_PULLUP;
#define PORTF_PULLDOWN       PORTF_MSB_PULLDOWN; PORTF_LSB_PULLDOWN;
#define PORTF_ANALOG         PORTF_MSB_ANALOG; PORTF_LSB_ANALOG;
#define PORTF_GET            ( GPIOF->IDR )
#define PORTF_HIGH           GPIOF->ODR = 0xFFFF;
#define PORTF_LOW            GPIOF->ODR = 0x0000;
#define PORTF_SET( pVal )    GPIOF->ODR = pVal;

#define PORTF_LSB_ALT         GPIOF->CRL = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTF_LSB_OUT         GPIOF->CRL = ALLPINS_OUT50_GPPUSHPULL;
#define PORTF_LSB_FLOAT       GPIOF->CRL = ALLPINS_IN_FLOAT;  
#define PORTF_LSB_PULLUP      GPIOF->CRL = ALLPINS_IN_PULL; GPIOF->BSRR = 0x00FF;
#define PORTF_LSB_PULLDOWN    GPIOF->CRL = ALLPINS_IN_PULL; GPIOF->BRR = 0x00FF;
#define PORTF_LSB_ANALOG      GPIOF->CRL = ALLPINS_IN_ANALOG;  
#define PORTF_LSB_GET         ( GPIOF->IDR & 0x00FF )  
#define PORTF_LSB_HIGH        GPIOF->BSRR = 0x00FF;  
#define PORTF_LSB_LOW         GPIOF->BRR = 0x00FF;   
#define PORTF_LSB_SET( pVal ) GPIOF->BRR = 0x00FF; GPIOF->BSRR = pVal;

#define PORTF_MSB_ALT         GPIOF->CRH = ALLPINS_OUT50_ALTPUSHPULL;          
#define PORTF_MSB_OUT         GPIOF->CRH = ALLPINS_OUT50_GPPUSHPULL;
#define PORTF_MSB_FLOAT       GPIOF->CRH = ALLPINS_IN_FLOAT;  
#define PORTF_MSB_PULLUP      GPIOF->CRH = ALLPINS_IN_PULL; GPIOF->BSRR = 0xFF00;
#define PORTF_MSB_PULLDOWN    GPIOF->CRH = ALLPINS_IN_PULL; GPIOF->BRR = 0xFF00;  
#define PORTF_MSB_ANALOG      GPIOF->CRH = ALLPINS_IN_ANALOG;  
#define PORTF_MSB_GET         ( GPIOF->IDR >> 8 )
#define PORTF_MSB_HIGH        GPIOF->BSRR = 0xFF00;  
#define PORTF_MSB_LOW         GPIOF->BRR = 0xFF00;  
#define PORTF_MSB_SET( pVal ) GPIOF->BRR = 0xFF00; GPIOF->BSRR = ( pVal << 8 );
#endif

#define PIN_PULLUP                              BIT4
#define PIN_PULLDOWN                            BIT5
#define PIN_OUT2                                BIT6    // deprecated
#define PIN_OUT10                               BIT7    // deprecated

#define OUT                                     PIN_OUT50_GPPUSHPULL
#define IN                                      PIN_IN_FLOAT
#define ANALOG                                  PIN_IN_ANALOG
#define PULLUP                                  (PIN_IN_PULL|PIN_PULLUP)
#define PULLDOWN                                (PIN_IN_PULL|PIN_PULLDOWN)
#define ALT                                     PIN_OUT50_ALTPUSHPULL

#define HIGH                                    1
#define LOW                                     0
#define NONE                                    0xFF

enum {
        PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
        PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
        PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
        PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
        PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
        PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
        ADCTEMP,ADCREF};


#define PYGMY_PWM_EN                  BIT0
#define PYGMY_PWM_DIR                 BIT1           
    
typedef struct {    
                    u16 UpCount;
                    u16 DownCount;
                    u16 Count;
                    u8 Pin;
                    u8 CR;
                } PYGMYPWM;

extern const TIMER *PYGMY_TIMERS[];
            
void pinSetSoftPWMFreq( u32 ulFreq );
u32 pinGetSoftPWMFreq( void );
void pinInitSoftPWM( void );            
u8 pinRemoveSoftPWM( u8 ucPin );
u8 pinAddSoftPWM( u8 ucPin, u32 ulFreq, u8 ucDutyCycle );
            
            
void pinConfig( u8 ucPin, u8 ucMode );
void pinInterrupt( PYGMYVOIDPTR pygmyFunc, u8 ucPin, u16 uiMode );
void enablePortClock( u8 ucPin );
GPIO *pinGetPort( u8 ucPin );
void pinSet( u8 ucPin, u8 ucState);
u8 pinGet( u8 ucPin );
void *pinGetTimer( u8 ucPin );
u8 pinGetChannel( u8 ucPin );
u8 pinPWM( u8 ucPin, u32 uiFreq, u8 ucDutyCycle );
u8 setTimer( u8 ucPin, u32 uiFreq, u8 ucDutyCycle );
    