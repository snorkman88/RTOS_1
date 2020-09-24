#define DEBOUNCE_TIME 40

void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate(const int TEC[2], uint8_t sentido);
void buttonPressed( void );
void buttonReleased( uint8_t sentido );
