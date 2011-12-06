#pragma once

#define PYGMY_XTAL          8000000
// end Board Defines for Pygmy OS 

// Internal Pin Defines
//#define BAT_LVL1            PA6
//#define BAT_LVL2            PA7
#define BAT_CHG             PC8 
#define BAT_PG              PC9
#define USB_DM              PA11 
#define USB_DP              PA12
#define USB_DISC            PD2
#define RF24_IRQ            PB5
#define RF24_CS             PB6
#define RF24_CE             PB7
#define FLASH_CS            PB8         
#define SRAM_CS             PB9
#define SPI_MISO            PC10
    #define FLASH_MISO      SPI_MISO
#define SPI_MOSI            PC11
    #define FLASH_MOSI      SPI_MOSI
#define SPI_SCK             PC12
    #define FLASH_SCK       SPI_SCK
// End Internal Pin Defines