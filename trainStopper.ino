#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/XpressNetMaster.h"


const int daPin = 3 ;
Debounce masterSwitch( daPin ) ;

XpressNetMasterClass Xnet ;

const int nLocos = 25 ;
struct 
{
    uint16_t address ;
    uint8_t speed ;
    uint8_t order ;
} loco[nLocos];

uint8_t currentLoco ;

void notifyXNetLocoDrive128() // keep track of active locos and alocate slots to them.
{

}


void setup()
{
    Xnet.setup( Loco128,  2 ) ;
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
            if( ++ currentLoco == nLocos ) currentLoco = 0 ;
        }
        END_REPEAT

    }

    Xnet.update() ;
}