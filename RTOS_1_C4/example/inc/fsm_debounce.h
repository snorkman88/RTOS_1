#define DEBOUNCE_TIME 40
#include "semphr.h"

void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate( const int TEC[2] );
void buttonPressed( void );
void buttonReleased( void );
