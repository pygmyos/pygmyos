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

void pinConfig( u8 ucPin, u8 ucMode );
void pinInterrupt( PYGMYVOIDPTR pygmyFunc, u8 ucPin, u16 uiMode );
void enablePortClock( u8 ucPin );
GPIO *getPortFromPin( u8 ucPin );
void pinSet( u8 ucPin, u8 ucState);
u8 pinGet( u8 ucPin );
void *getTimerFromPin( u8 ucPin );
u8 getChannelFromPin( u8 ucPin );
u8 pinPWM( u8 ucPin, u32 uiFreq, u8 ucDutyCycle );
u8 setTimer( u8 ucPin, u32 uiFreq, u8 ucDutyCycle );
    