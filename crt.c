//#define __CRT_C__

#include <stdint.h>

extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
//extern unsigned long _estack;

/* This is the main */
extern int main (void);

//void SystemInit (void) __attribute__((weak));

//void SystemInit (void)
//{
//} 

/***************************************************************************/
/*  ResetHandler                                                           */
/*                                                                         */
/*  This function is used for the C runtime initialisation,                */
/*  for handling the .data and .bss segments.                              */
/***************************************************************************/
void ResetHandler (void)
{
   uint32_t *pSrc;
   uint32_t *pDest;
   
   //SystemInit();    
   
   /*
    * Set the "Vector Table Offset Register". From the ARM
    * documentation, we got the following information:
    *
    * Use the Vector Table Offset Register to determine:
    *  - if the vector table is in RAM or code memory
    *  - the vector table offset.    
    */
    *((uint32_t*)0xE000ED08) = (uint32_t)&_stext;
   
   
    // Copy init data from FLASH to RAM
   
   pSrc  = &_etext;
   pDest = &_sdata;
   while(pDest < &_edata){
      *pDest++ = *pSrc++;
   }
   
   // clear bss
   pDest = &_sbss;
   while(pDest < &_ebss){
      *pDest++ = 0;
   }
   
   main();    
   
   while(1) {};    

}

