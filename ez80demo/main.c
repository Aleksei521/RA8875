#include <stdio.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
//#include "ZDevice.h"
#include "ZInterrupt.h"
#include <ez80.h>



void InitializeResources(void);
extern void RA8875Ini(void);

void InitializeResources(void)
{
	 RA8875Ini();
}


void main(void)
  {
    
    RZK_KernelInit() ;
    
    InitializeResources() ;
    
    RZK_KernelStart() ; 
}
