#include "pygmy_profile.h"
#include "profiles/memory/flashsim.h"

const PYGMYMEMIO PYGMYFLASHSIM = {
    (void*)simGetChar,
    (void*)simPutChar,
    (void*)simGetWord,
    (void*)simPutWord,
    (void*)simGetLong,
    (void*)simPutLong,
    (void*)simGetBuffer,
    (void*)simPutBuffer,
    (void*)simOpen,
    (void*)simVOpen,
    (void*)simClose,
    (void*)simDesc,
    (void*)simStatus,
    (void*)simEraseAll,
    (void*)simEraseSector
};

u16 simGetWord( PYGMYFLASHSIM *pygmyPort, u64 ulAddress )
{
    u16 uiData;

    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simGetWord -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simGetWord -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    uiData = pygmyPort->Buffer[ ulAddress ] << 8;
    uiData |= pygmyPort->Buffer[ ulAddress + 1 ];

    return( uiData );
}

u32 simGetLong( PYGMYFLASHSIM *pygmyPort, u64 ulAddress )
{
    u32 ulData;

    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simGetLong -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simGetLong -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    ulData =  globalHeap[ ulAddress++ ] << 24;
    ulData |= globalHeap[ ulAddress++ ] << 16;
    ulData |= globalHeap[ ulAddress++ ] << 8;
    ulData |= globalHeap[ ulAddress ];

    return( ulData );
}

u8 simGetChar( PYGMYFLASHSIM *pygmyPort, u64 ulAddress )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simGetChar -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simGetChar -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    return( pygmyPort->Buffer[ ulAddress ] );
}

u8 simGetBuffer( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    u32 i;

    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simGetBuffer -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simGetBuffer -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    for( i = 0; i < uiLen; i++ ){
        ucBuffer[ i ] = pygmyPort->Buffer[ ulAddress + i ];
    } // for

    return( TRUE );
}

u32 simStatus( PYGMYFLASHSIM *pygmyPort )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simStatus -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    return( pygmyPort->Status );
}

u32 simID( PYGMYFLASHSIM *pygmyPort )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simID -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    return( pygmyPort->ID );
}

u8 simEraseSector( PYGMYFLASHSIM *pygmyPort, u64 ulSector )
{
    u64 ulAddress;


    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simEraseSector -> Sector: %d", ulAddress );
        exit( 1 );
    } // if
    ulAddress = ( ulSector / pygmyPort->SectorSize ) * pygmyPort->SectorSize;
    if( ulAddress + pygmyPort->SectorSize >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simEraseSector -> Sector: %d", ulSector );
        exit( 1 );
    } // if

    memset( pygmyPort->Buffer+ulSector, 0xFF, pygmyPort->SectorSize );

    return( TRUE );
}

u8 simEraseAll( PYGMYFLASHSIM *pygmyPort )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simEraseAll" );
        exit( 1 );
    } // if
    memset( pygmyPort->Buffer 0xFF, pygmyPort->Capacity );

    return( TRUE );
}

void simWaitForBusy( PYGMYFLASHSIM *pygmyPort )
{
    // ToDo: implement status and delay
    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simWaitForBusy -> Address: %d", ulAddress );
        exit( 1 );
    } // if

}

u8 simPutWord( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u16 uiWord )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simPutWord -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simPutWord -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    pygmyPort->Buffer[ ulAddress++ ] = (u8)( uiWord >> 8 );
    pygmyPort->Buffer[ ulAddress ]   = (u8)( uiWord );
}

u8 simPutLong( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u32 ulLong )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simPutLong -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simPutLong -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    pygmyPort->Buffer[ ulAddress++ ] = (u8)((u32)ulLong>>24);
    pygmyPort->Buffer[ ulAddress++ ] = (u8)((u32)ulLong>>16);
    pygmyPort->Buffer[ ulAddress++ ] = (u8)((u32)ulLong>>8);
    pygmyPort->Buffer[ ulAddress ]   = (u8)((u32)ulLong);

    return( TRUE );
}

u8 simPutBuffer( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    u32 i;

    if( pygmyPort == NULL ){
        print( DEBUG, "Null heap -> simPutBufer -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress + pygmyPort->SectorSize >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simPutBuffer -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    for( i = 0; i < uiLen; i++ ){
        pygmyPort->Buffer[ ulAddress + i ] = ucBuffer[ i ];
    } // for
}

u8 simPutChar( PYGMYFLASHSIM *pygmyPort, u64 ulAddress, u8 ucByte )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simPutChar -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    if( ulAddress >= pygmyPort->Capacity ){
        print( DEBUG, "Address out of bounds -> simPutChar -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    pygmyPort->Buffer[ ulAddress] = ucByte;

    return( TRUE );
}

u8 simOpen( PYGMYFLASHSIM **pygmyPort, u64 ulAddress, ... )
{
    u8 ucResult;
    va_list vaList;

    if( pygmyPort != NULL ){
        print( DEBUG, "Port in use -> simOpen -> Address: %d", ulAddress );
        exit( 1 );
    } // if
    va_start( vaList, ulAddress );
    ucResult = simVOpen( ucPort, ulAddress, vaList );
    va_end( vaList );

    return( ucResult );
}

u8 simVOpen( PYGMYFLASHSIM **pygmyPort, u64 ulAddress, va_list vaList )
{
    // Variadic open
    PYGMYFLASHSIM *pygmyFlashSim;
    u8 *ucBuffer;
    u8 ucCS, ucSCK, ucMISO, ucMOSI;
    // This function MUST be passed a pointer to a pointer
    // The PYGMYMEMIO interface assumes that the Open call will allocate memory
    //    for the port needed by the interface. The pointer passed will be
    //    encapsulated by the parent data structer or interface.
    //    Must be called before any write operations to FLASH

    if( pygmyPort != NULL ){
        print( DEBUG, "Port in use -> simVOpen -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    pygmyPort = malloc( sizeof( PYGMYFLASHSIM ) );
    if( pygmyPort == NULL ){
        print( DEBUG, "\rMalloc fail -> simVOpen -> Len: %d", sizeof( PYGMYFLASHSIM ) );
        return( FALSE );
    } // if
    
    pygmyPort->Sectors = (u8)va_arg( vaList, int );
    pygmyPort->SectorSize = va_arg( vaList, int );
    pygmyPort->Capacity = pygmyPort->Sectors * pygmyPort->SectorSize;
    pygmyPort->Buffer = malloc( pygmyPort->Capacity );
    if( ptrPort == NULL ){
        print( DEBUG, "\rMalloc fail -> simVOpen -> Len: %d", pygmyPort->Capacity );
        return( FALSE );
    } // if
    
    va_end( vaList );
    *ucPort = ptrPort;
    memset( ptrPort, 0xFF, pygmyPort->Capacity );
    
    return( TRUE );
}

u8 simClose( PYGMYFLASHSIM **pygmyPort )
{
    if( pygmyPort == NULL ){
        print( DEBUG, "\rNull port -> simClose" );
        exit( 1 );
    } // if

    free( *pygmyPort );

    return( TRUE );
}

u8 simDesc( PYGMYFLASHSIM *pygmyPort, PYGMYMEMDESC *pygmyDesc )
{
    // The simulator isn't communicating with any hardware, so it stores
    // the hardware parameters within the port structure. To maintain seamless
    // compatibility with hardware interfaces, the memory descriptor is still used
    if( pygmyPort == NULL ){
        print( DEBUG, "Null port -> simDesc -> Address: %d", ulAddress );
        exit( 1 );
    } // if

    pygmyDesc->Manufacturer   = pygmyPort->Manufacturer; // No real value, 0 is used to denote a simulator
    pygmyDesc->Type           = pygmyPort->Type;
    pygmyDesc->SectorSize     = pygmyPort->SectorSize;
    pygmyDesc->Sectors        = pygmyPort->Sectors;
    pygmyDesc->Attributes     = pygmyPort->Attributes;
    pygmyDesc->Capacity       = pygmyDesc->Capacity;
    //pygmyDesc->Attributes = READ|WRITE|globalSimType;

    print( COM3, "\rCapacity: %d", pygmyDesc->Capacity );
    print( COM3, "\rSectors: %d", pygmyDesc->Sectors );
    print( COM3, "\rSectorSize: %d", pygmyDesc->SectorSize );
    print( COM3, "\rAttributes: %d", pygmyDesc->Attributes );
    print( COM3, "\rManufacturer: %d", pygmyDesc->Manufacturer );
    print( COM3, "\rType: %d", pygmyDesc->Type );

    return( TRUE );
}
