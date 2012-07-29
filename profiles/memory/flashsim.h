#include "pygmy_profile.h"

typedef struct {
                u8 *Buffer;
                u8 Attributes;
                u32 ID;
                u32 Delay;
                u32 Status;
                u32 Sectors;
                u32 SectorSize;
                u32 Manufacturer;
                u64 Capacity;
                } PYGMYFLASHSIM;
                

u16 simGetWord( PYGMYFLASHSIM *pygmyPort, u64 ulAddress );
u32 simGetLong( PYGMYFLASHSIM *pygmyPort, u64 ulAddress );
u8 simGetChar( PYGMYFLASHSIM *pygmyPort, u64 ulAddress );
u8 simGetBuffer( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u8 *ucBuffer, u16 uiLen );
u32 simStatus( PYGMYFLASHSIM *pygmyPort );
u32 simID( PYGMYFLASHSIM *pygmyPort );
u8 simEraseSector( PYGMYFLASHSIM *pygmyPort, u64 ulSector );
u8 simEraseAll( PYGMYFLASHSIM *pygmyPort );
void simWaitForBusy( PYGMYFLASHSIM *pygmyPort );
u8 simPutWord( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u16 uiWord );
u8 simPutLong( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u32 ulLong );
u8 simPutBuffer( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u8 *ucBuffer, u16 uiLen );
u8 simPutChar( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u8 ucByte );
u8 simOpen( PYGMYFLASHSIM **pygmyPort, u64 ulAddress, ... );
u8 simVOpen( PYGMYFLASHSIM **pygmyPort, u64 ulAddress, va_list vaList );
u8 simClose( PYGMYFLASHSIM **pygmyPort );
u8 simDesc( PYGMYFLASHSIM *pygmyPort, PYGMYMEMDESC *pygmyDesc );


u8 simGetChar( u8 *ucBuffer, u64 ulAddress );
u8 simPutChar( u8 *ucBuffer, u64 ulAddress );
u16 simGetWord( PYGMYFLASHSIM *pygmyPort, u64 ulAddress )
u8 simPutWord( u8 *ucBuffer, u64 ulAddress );
u64 simGetLong( u8 *ucBuffer, u64 ulAddress );
u8 simPutLong( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u32 ulLong );
u8 simGetBuffer( u8 *ucBuffer, u64 ulAddress, u8 *ucBuffer, u16 uiLen );
u8 simPutBuffer( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 *ucBuffer, u16 uiLen );
u8 simOpen( PYGMYSPIPORT **pygmySPI, u64 ulAddress, ... );
u8 simVOpen( PYGMYSPIPORT **pygmySPI, u64 ulAddress, va_list vaList );
u8 simClose( u8 **ucBuffer );
u8 simDesc( u8 *ucBuffer, PYGMYMEMDESC *pygmyDesc );
u32 simStatus( u8 *ucBuffer );
u8 simEraseAll( u8 *ucBuffer );
u8 simEraseSector( u8 *ucBuffer, u64 ulSector );

extern const PYGMYMEMIO PYGMYFLASHSIM; 