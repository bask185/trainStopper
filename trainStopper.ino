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

void allocateSlot( uint8_t address )
{
    for( int i = 0 ; i < nSlots ; i ++ )
    {
        if( loco[i].address == 0xFFFF ) loco[i].address = address ;
        return ;
    }
}

void notifyXNetLocoDrive128( uint16_t address, uint8_t speed ) // keep track of active locos and alocate slots to them.
{
    if( speed == 0 ) freeSlot( address ) ;
    else             allocateSlot( address ) ;
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