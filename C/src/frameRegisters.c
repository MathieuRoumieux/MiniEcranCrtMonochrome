#include "frameRegisters.h"
#include "definitions.h"

//inits everything at 0
volatile frameReg reg1 = {{128}, {128}, 0};
volatile frameReg reg2 = {{128}, {128}, 0};
volatile frameReg reg3 = {{128}, {128}, 0};
volatile frameReg regDisplay = {{128}, {128}, 0};
//volatile gifReg reg4 = {{128}, {128}, {0}, 0, 0};