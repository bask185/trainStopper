#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/XpressNetMaster.h"


const int daPin = 3 ;
Debounce masterSwitch( daPin ) ;

XpressNetMasterClass Xnet ;

const int nSlots = 25 ;
struct 
{
    uint16_t    address ;
    uint8_t     speed ;
} loco[nSlots];

uint8_t currentLoco ;

void freeSlot( uint8_t address )
{
    for( int i = 0 ; i < nSlots ; i ++ )
    {
        if( address == loco[i].address )
        {
            loco[i].address = 0xFFFF ;
            return ;
        }
    }
}

uint8_t slotInUse( uint16_t address )
{
    for( int i = 0 ; i < nSlots ; i ++ )
    {
        if( loco[i].address == address ) return i;
    }

    return 255 ;
}

uint8_t allocateSlot( uint8_t address )
{
    for( int i = 0 ; i < nSlots ; i ++ )
    {
        if( loco[i].address == 0xFFFF )
        {
            loco[i].address = address ;
            return i ;
        }
    }
    
    return 255 ;
}

void notifyXNetLocoDrive128( uint16_t address, uint8_t speed ) // keep track of active locos and alocate slots to them.
{
    if( speed == 0 )        // if speed is 0, free up the slot. Loco does not have to be stopped
    {
        freeSlot( address ) ;
    }
    else
    { 
        uint8_t slot = slotInUse( address ) ;       // see if slot is in use or not

        if( slot == 255 ) slot = allocateSlot( address ) ; // if not yet in use, allocate a slot

        loco[slot].speed = speed ;              // update speed
    }
}


void setup()
{
    Xnet.setup( Loco128,  2 ) ;

    for( int i = 0 ; i < nSlots ; i ++ )
    {
        loco[i].address = 0xFFFF ;
    }
}

void loop()
{
    REPEAT_MS( 20 )
    {
        masterSwitch.debounce() ;
    }
    END_REPEAT

    uint8_t state = masterSwitch.getState() ;
    if( state == FALLING )
    {
        
    }
    if( state == RISING )
    {

    }
    if( state == LOW )
    {
        REPEAT_MS(50)
        {
            uint16_t address = loco[currentLoco].address ;
            Xnet.setSpeed( address, Loco128, 0 ) ;      
            if( ++ currentLoco == nSlots ) currentLoco = 0 ;
        }
        END_REPEAT

    }

    Xnet.update() ;
}

/* humanifacation
    if a train is driving, a slot must be allocated
    if a train is stationary, a slot must be freed
    we work under the assumption that no more than 25 trains are used (though there is more memory avaiable.
*/