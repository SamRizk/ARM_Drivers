
#include "../include/MCLA/RCC/RCC.h"
#define  ADDR       (u32)0x40023800
int
main(void)
{

	u32 SS =MEM_ADDR(BIT_BAND(ADDR,17));
	if (SS == 0)
		SS = 1 ;

	// At this stage the system clock should have already been configured
  // at high speed.

  // Infinite loop
  while (1)
    {
       // Add your code here.
    }
}
