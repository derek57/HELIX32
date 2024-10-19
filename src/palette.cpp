#include <conio.h>
#include "helix.h"

void cdecl gSetDACRange(int a1, int a2, unsigned char *a3)
{
	unsigned v5; // al
	unsigned char *v6; // ebx
	unsigned v7; // al
	unsigned v8; // al

	outp(0x3C8, a1);

	do
	{
		v5 = *a3 >> 2;
		v6 = a3 + 1;
		outp(0x3C9, v5);
		v7 = *v6++ >> 2;
		outp(0x3C9, v7);
		v8 = *v6 >> 2;
		a3 = v6 + 1;
		outp(0x3C9, v8);
		--a2;
	}
	while (a2);
}

void cdecl gSetDAC(unsigned a1, unsigned a2, unsigned a3, unsigned a4)
{
	outp(0x3C8, a1);
	outp(0x3C9, a2 >> 2);
	outp(0x3C9, a3 >> 2);
	outp(0x3C9, a4 >> 2);
}

