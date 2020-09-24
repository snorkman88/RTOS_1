#define DEBOUNCE_TIME 40

void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate(const int TEC[2], const int TECreset[2]);
void buttonPressed( void );
void buttonReleased( void );
