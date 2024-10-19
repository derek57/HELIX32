#include <dos.h>
#include <conio.h>
#include <string.h>
#include "helix.h"


int cdecl gGetMode(void)
{
	union REGS regs;

	// Set up registers for interrupt call
	regs.h.ah = 0x0F;  // Function to get current video mode

	// Call BIOS interrupt 0x10
	int386(0x10, &regs, &regs);

	// The current video mode is returned in AL
	return regs.h.al;  // Return the video mode
}

void cdecl gSetMode(int mode)
{
	union REGS regs;

	// Set up registers for interrupt call
	regs.h.ah = 0x00;  // Function to set video mode
	regs.h.al = mode;  // Video mode to set

	// Call BIOS interrupt 0x10
	int386(0x10, &regs, &regs);
}

void setVideoMode(unsigned char mode)
{
	union REGS regs;	// Union to hold register values

	// Set up registers for the BIOS interrupt call
	regs.w.ax = 0x0000 | mode;	// Set AX to 0x0000 and AL to mode (AH is function code 0)
	
	// Call BIOS interrupt 0x10 (Video Services)
	int386(0x10, &regs, &regs);
}

void memset32(void *dest, unsigned int value, unsigned int count)
{
	unsigned int i;

	// Cast the destination to a pointer to uint32_t
	unsigned int *ptr = (unsigned int *)dest;

	// Iterate over the count and set each 32-bit block
	for (i = 0; i < count; i++)
		ptr[i] = value;
}

