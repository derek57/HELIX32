#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "helix.h"


#define DEBUG	1


PAGE_STRUCT MCGAPageTable[2] = {
	{ 1, 0x0A0000, 0x0FA00, 0x140, 0x0C8, 0x140, 0, 0 },
	{ 0, 0x0AFA00, 0x500, 0x140, 4, 0x140, 0, 0 }
};

char byte_10DCC[12] = { 0x88, 0x20, 8, 0x30, 0x88, 7, 0x20, 7, 8, 7, 0x30, 7 };

int dword_10CBC[4];

int dword_10CB7;


int MCGAValid(void)
{
	return 1;
}

void MCGAInit(void)
{
	int *v0; // ebx
	int v1; // ecx
	int v2; // eax
	unsigned int bytesPerRow; // edx

	gPages = sizeof(MCGAPageTable) / sizeof(PAGE_STRUCT);
	memcpy(gPageTable, MCGAPageTable, sizeof(MCGAPageTable));
	v0 = gYLookup;
	v1 = sizeof(gYLookup) / sizeof(unsigned int);
	v2 = 0;
	bytesPerRow = MCGAPageTable[0].bytesPerRow;

	do
	{
		*v0++ = v2;
		v2 += bytesPerRow;
		--v1;
	}
	while (v1);
}

void MCGASetMode(void)
{
	setVideoMode(0x03);
	MCGASetPage(1, 0, 0, 0);
}

void MCGASetPage(int a1, int a2, int a3, int a4)
{
	unsigned int v4; // edi
	unsigned char v5[2]; // bx
	unsigned char v6[2]; // cx
	unsigned char v7; // al MAPDST
	unsigned char v9[2]; // ax

	dword_10CB7 = a4;
	v4 = (a3 >> 2) + gYLookup[a2];
	v5[1] = (unsigned char)((gPageTable[a4].begin + v4) & 0xFF);
	v5[0] = 13;
	v6[1] = (unsigned char)(((gPageTable[a4].begin + v4) >> 8) & 0xFF);
	v6[0] = 12;

	do
		v7 = inp(0x3DA);
	while ((v7 & 1) != 0);

	_disable();
	outpw(0x3D4, *(unsigned short *)v5);
	outpw(0x3D4, *(unsigned short *)v6);
	_enable();

	if (a1)
	{
		do
			v7 = inp(0x3DA);
		while ((v7 & 8) == 0);
	}

	v9[1] = gPageTable[a4].bytesPerRow >> 1;
	v9[0] = 19;
	outpw(0x3D4, *(unsigned short *)v9);
	inp(0x3DA);
	outp(0x3C0, 0x33);
	outp(0x3C0, 2 * (a3 & 3));
}

int MCGAGetPage(void)
{
	return dword_10CB7;
}

void MCGAClear(int a1)
{
	unsigned int begin; // edi
	unsigned int size; // ecx
	int v3; // ecx
	unsigned int v4; // ebx
	void *v5; // edi
	char v6; // cf
	unsigned int i; // ecx

	begin = gPageTable[a1].begin;
	size = gPageTable[a1].size;

	if (size)
	{
		if (size >= 0xC)
		{
			v3 = -begin & 3;
			v4 = gPageTable[a1].size - v3;
			memset((void *)begin, 0, v3);
			v5 = (void *)(begin + v3);
			memset(v5, 0, 4 * (v4 >> 2));
			begin = (unsigned int)v5 + 4 * (v4 >> 2);
			size = v4 & 3;
		}

		v6 = size & 1;

		for (i = size >> 1; i; i--)
		{
			*(unsigned short *)begin = 0;
			begin += 2;
		}

		if (v6)
			*(unsigned char *)begin = 0;
	}
}

void MCGASetPixel(int a1, int a2, int a3)
{
	*(unsigned char *)(a2 + gYLookup[a1] + gPageTable[a3].begin) = gColor;
}

int MCGAGetPixel(void)
{
	int v1 = 0;					// Placeholder for actual input or variable
	int v2 = 0;					// Placeholder for actual input or variable
	int v3 = 0;					// Placeholder for actual input or variable

	unsigned int temp = v1;			// Use v1 to demonstrate the operation
	unsigned short highWord = (unsigned short)(temp >> 16);	// Extract high word from v1
	unsigned short lowWord = *(unsigned short *)(v2 + gYLookup[v1] + gPageTable[v3].begin);	// Get the low word from memory

	int result = (highWord << 16) | lowWord;	// Combine high word and low word into a single 32-bit result

	return result;
}

void MCGAHLine(int a1, int a2, int a3, int a4)
{
	unsigned int v4;		// edi
	unsigned int v6;		// ecx
	int v7;				// ecx
	unsigned int v8;		// ebx
	unsigned int *v9;		 // edi
	unsigned int i;		 // ecx
	unsigned char v10;		// cf
	unsigned short v5;		// ax
	unsigned short v12[2];	// [esp-8h] [ebp-Ch]

	v4 = a2 + gYLookup[a3] + gPageTable[a4].begin;
	
	// Setting up color
	v5 = (unsigned short)gColor;
	
	v6 = a1 - a2 + 1;
	
	if (a1 - a2 != -1) // Check if there's something to draw
	{
		// Use the color for high byte
		v5 |= (unsigned short)(gColor << 8);
		
		if (a1 - a2 != 11)
		{
			v7 = -v4 & 3; // Alignment
			v8 = a1 - a2 + 1 - v7;
			
			// Fill the initial part with color
			memset((void *)v4, gColor, v7);
			v9 = (unsigned int *)(v4 + v7);
			
			// Fill the middle part with 32-bit color values
			v12[1] = v5; // high word
			v12[0] = v5; // low word
			memset32(v9, *(int *)v12, v8 >> 2);
			v4 = (unsigned int)&v9[v8 >> 2];
			v6 = v8 & 3;
		}
		
		// Fill the remaining part with 16-bit color values
		v10 = v6 & 1;
		for (i = v6 >> 1; i; --i)
		{
			*(unsigned short *)v4 = v5;
			v4 += 2;
		}
		
		// Fill the last byte if needed
		if (v10)
			*(unsigned char *)v4 = (unsigned char)v5;
	}
}

void MCGAVLine(int a1, int a2, int a3, int a4)
{
	unsigned int bytesPerRow; // edx
	unsigned char *v5; // ebx
	int v6; // ecx
	unsigned char v7; // al

	bytesPerRow = gPageTable[a4].bytesPerRow;
	v5 = (unsigned char *)(a3 + gYLookup[a2] + gPageTable[a4].begin);
	v6 = a1 - a2 + 1;
	v7 = gColor;

	do
	{
		*v5 = v7;
		v5 += bytesPerRow;
		--v6;
	}
	while (v6);
}

void MCGAFillBox(int a1, int a2, int a3, int a4, int a5)
{
	unsigned int v5; // edi
	unsigned char v6[2]; // ax
	int v7; // eax
	int v8; // ebx
	int v9; // edx
	unsigned int v10; // esi
	unsigned int v11; // ecx
	unsigned int v12; // ecx
	unsigned int v13; // ebx
	unsigned int v14; // edi
	unsigned char v15; // cf
	unsigned int i; // ecx
	unsigned short v17[2]; // [esp-4h] [ebp-10h]
	int v18; // [esp-4h] [ebp-10h]

	v5 = a4 + gYLookup[a3] + gPageTable[a5].begin;
	v6[0] = gColor;
	v6[1] = gColor;
	v17[1] = *(unsigned short *)v6;
	v17[0] = *(unsigned short *)v6;
	v7 = *(unsigned int *)v17;
	v8 = a2 - a4;
	v9 = a1 - a3;
	v10 = gPageTable[a5].bytesPerRow - (a2 - a4);

	do
	{
		v11 = v8;

		if (v8)
		{
			if (v8 != 12)
			{
				v18 = v8;
				v12 = -v5 & 3;
				v13 = v8 - v12;
				memset((void *)v5, v7, v12);
				v14 = v5 + v12;
				memset32((void *)v14, v7, v13 >> 2);
				v5 = v14 + 4 * (v13 >> 2);
				v11 = v13 & 3;
				v8 = v18;
			}

			v15 = v11 & 1;

			for (i = v11 >> 1; i; --i)
			{
				*(unsigned short *)v5 = v7;
				v5 += 2;
			}

			if (v15)
				*(unsigned char *)v5++ = v7;
		}

		v5 += v10;
		--v9;
	}
	while (v9);
}

static inline char CFADD(int x, int y)
{
	// Assuming we are dealing with unsigned integers to mimic carry behavior
	if (sizeof(x) == 1)  // 8-bit addition
	{
		return (unsigned char)x > (unsigned char)(x + y);
	}
	else if (sizeof(x) == 2)  // 16-bit addition
	{
		return (unsigned short)x > (unsigned short)(x + y);
	}
	else if (sizeof(x) == 4)  // 32-bit addition
	{
		return (unsigned int)x > (unsigned int)(x + y);
	}
#if DEBUG
	printf("Error: No 64-bit support in this (WATCOM C/C++ v10.6) environment\n");

	return 0;  // Return 0 to indicate no carry, as fallback
#endif
}

void MCGALine(int a1, int a2, int a3, int a4, int a5)
{
	int v5; // eax
	int v6; // edx
	unsigned int v7; // ecx
	unsigned int v8; // esi
	unsigned char *v9; // edi
	unsigned int v10; // ebx
	int v11; // cf
	unsigned int v12; // esi
	unsigned int v13; // eax
	unsigned int v14; // edx
	int v15; // esi
	int v16; // esi
	int v17; // edx
	unsigned int v18; // ecx

//	*(&loc_10371 + 1) = gColor; // This seems to write the value of gColor
	v5 = a3;
	v6 = a1;
	v7 = a4;
	v8 = a2;

	if (a3 >= (unsigned int)a1)
	{
		v7 = a2;
		v8 = a4;
		v5 = a1;
		v6 = a3;
	}

	v9 = (unsigned char *)(v7 + gYLookup[v5] + gPageTable[a5].begin);
	v10 = v6 - v5;
	v11 = v8 < v7;
	v12 = v8 - v7;
	v13 = 1;

	if (v11)
	{
		v13 = -1;
		v12 = -v12;
	}

	if (v12 >= v10)
	{
		v14 = v10;
		v10 = v12;
		v15 = 320;
	}
	else
	{
		v14 = v12;
		v15 = v13;
		v13 = 320;
	}

	if (v10)
	{
		dword_10CBC[1] = v13;
		dword_10CBC[0] = v13 + v15;
		v16 = -v10;
		dword_10CBC[2] = -2 * v10;
		v17 = 2 * v14;
		v18 = (v10 + 64) >> 6;

//		v13 = 0xFF;
		v13 = gColor;

		switch (((unsigned char)v10 + 1) & 0x3F)
		{
			case 0:
				goto LABEL_10;
			case 1:
				goto LABEL_11;
			case 2:
				goto LABEL_12;
			case 3:
				goto LABEL_13;
			case 4:
				goto LABEL_14;
			case 5:
				goto LABEL_15;
			case 6:
				goto LABEL_16;
			case 7:
				goto LABEL_17;
			case 8:
				goto LABEL_18;
			case 9:
				goto LABEL_19;
			case 0xA:
				goto LABEL_20;
			case 0xB:
				goto LABEL_21;
			case 0xC:
				goto LABEL_22;
			case 0xD:
				goto LABEL_23;
			case 0xE:
				goto LABEL_24;
			case 0xF:
				goto LABEL_25;
			case 0x10:
				goto LABEL_26;
			case 0x11:
				goto LABEL_27;
			case 0x12:
				goto LABEL_28;
			case 0x13:
				goto LABEL_29;
			case 0x14:
				goto LABEL_30;
			case 0x15:
				goto LABEL_31;
			case 0x16:
				goto LABEL_32;
			case 0x17:
				goto LABEL_33;
			case 0x18:
				goto LABEL_34;
			case 0x19:
				goto LABEL_35;
			case 0x1A:
				goto LABEL_36;
			case 0x1B:
				goto LABEL_37;
			case 0x1C:
				goto LABEL_38;
			case 0x1D:
				goto LABEL_39;
			case 0x1E:
				goto LABEL_40;
			case 0x1F:
				goto LABEL_41;
			case 0x20:
				goto LABEL_42;
			case 0x21:
				goto LABEL_43;
			case 0x22:
				goto LABEL_44;
			case 0x23:
				goto LABEL_45;
			case 0x24:
				goto LABEL_46;
			case 0x25:
				goto LABEL_47;
			case 0x26:
				goto LABEL_48;
			case 0x27:
				goto LABEL_49;
			case 0x28:
				goto LABEL_50;
			case 0x29:
				goto LABEL_51;
			case 0x2A:
				goto LABEL_52;
			case 0x2B:
				goto LABEL_53;
			case 0x2C:
				goto LABEL_54;
			case 0x2D:
				goto LABEL_55;
			case 0x2E:
				goto LABEL_56;
			case 0x2F:
				goto LABEL_57;
			case 0x30:
				goto LABEL_58;
			case 0x31:
				goto LABEL_59;
			case 0x32:
				goto LABEL_60;
			case 0x33:
				goto LABEL_61;
			case 0x34:
				goto LABEL_62;
			case 0x35:
				goto LABEL_63;
			case 0x36:
				goto LABEL_64;
			case 0x37:
				goto LABEL_65;
			case 0x38:
				goto LABEL_66;
			case 0x39:
				goto LABEL_67;
			case 0x3A:
				goto LABEL_68;
			case 0x3B:
				goto LABEL_69;
			case 0x3C:
				goto LABEL_70;
			case 0x3D:
				goto LABEL_71;
			case 0x3E:
				goto LABEL_72;
			case 0x3F:
				while (1)
				{
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;

					if (!--v18)
						break;
LABEL_10:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_11:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_12:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_13:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_14:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_15:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_16:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_17:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_18:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_19:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_20:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_21:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_22:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_23:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_24:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_25:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_26:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_27:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_28:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_29:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_30:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_31:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_32:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_33:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_34:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_35:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_36:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_37:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_38:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_39:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_40:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_41:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_42:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_43:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_44:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_45:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_46:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_47:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_48:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_49:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_50:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_51:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_52:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_53:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_54:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_55:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_56:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_57:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_58:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_59:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_60:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_61:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_62:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_63:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_64:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_65:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_66:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_67:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_68:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_69:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_70:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_71:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
LABEL_72:
					*v9 = -1;
					v9 += dword_10CBC[-CFADD(v17, v16) + 1];
					v16 += dword_10CBC[-CFADD(v17, v16) + 3] + v17;
				}
				break;
		}
	}
}

void MCGAHLineROP(int a1, int a2, int a3, int a4)
{
	unsigned char *v4; // edi
	int v5; // ecx
//	unsigned char v6; // al

	v4 = (unsigned char *)(a2 + gYLookup[a3] + gPageTable[a4].begin);
	v5 = a1 - a3 + 1;

//	loc_10908 = byte_10DCC[gROP]; // This seems to write the value of gColor
//	v6 = gColor;

	do
	{
//		*v4++ = v6;
		*v4++ = byte_10DCC[gROP];
		--v5;
	}
	while (v5);
}

void MCGAVLineROP(int a1, int a2, int a3, int a4)
{
	unsigned int bytesPerRow; // edx
	unsigned char *v5; // ebx
	int v6; // ecx
//	unsigned char v7; // al

	bytesPerRow = gPageTable[a4].bytesPerRow;
	v5 = (unsigned char *)(a3 + gYLookup[a2] + gPageTable[a4].begin);
	v6 = a1 - a2 + 1;

//	loc_10954 = byte_10DCC[gROP]; // This seems to write the value of gColor
//	v7 = gColor;

	do
	{
//		*v5 = v7;
		*v5 = byte_10DCC[gROP];
		v5 += bytesPerRow;
		--v6;
	}
	while (v6);
}

void MCGABlitRLE2V(int a1, int a2, int a3, int a4)
{
	unsigned int bytesPerRow; // ebx
	unsigned int v5;          // edx
	unsigned int v6;          // eax
	unsigned char *v7;        // esi
	unsigned int v8;          // ecx
	unsigned char *v9;        // edi
	unsigned char *v10;       // esi
	unsigned char *v11;       // edi
	unsigned int v12;         // ecx
	unsigned char v13;        // [esp-4h] [ebp-10h]

	bytesPerRow = gPageTable[a3].bytesPerRow;
	v5 = a2 + gYLookup[a1] + gPageTable[a3].begin;
	v6 = *(unsigned short *)(a4 + 4) + 1;
	v7 = (unsigned char *)(a4 + 12);
	v8 = 0;

	while (1)
	{
		v9 = (unsigned char *)v5;
		v5 += bytesPerRow;
		if (!--v6)
			break;

		while (1)
		{
			v8 = (v8 & 0xFFFFFF00) | (*v7 & 0xFF); // Set low byte of v8 to *v7
			v10 = v7 + 1;
			v11 = &v9[v8];
			v8 = (v8 & 0xFFFFFF00) | (*v10 & 0xFF); // Set low byte of v8 to *v10
			v7 = v10 + 1;

			if (!v8)
				break;

			if (v8 > 0xC)
			{
				if ((unsigned char)v7 & 1)
				{
					*v11++ = *v7++;
					--v8;
				}

				v13 = v8;
				v12 = v8 >> 2;
				memcpy(v11, v7, 4 * v12);
				v7 += 4 * v12;
				v11 += 4 * v12;
				v8 = v13 & 3;
			}

			memcpy(v11, v7, v8);
			v7 += v8;
			v9 = &v11[v8];
			v8 = 0;
		}
	}
}

void MCGABlitV2M(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
	unsigned char *v8; // esi
	unsigned int v9; // ecx
	unsigned char *v10; // edi
	int v11; // edx
	unsigned int v12; // ecx
	unsigned int v13; // ecx
	unsigned char v14; // [esp-4h] [ebp-20h]
	unsigned int v15; // [esp+Ch] [ebp-10h]
	unsigned int v16; // [esp+14h] [ebp-8h]
	unsigned int v17; // [esp+18h] [ebp-4h]

	v8 = (unsigned char *)(a7 + gYLookup[a6] + gPageTable[a8].begin);
	v9 = *(unsigned short *)(a3 + 6);
	v10 = (unsigned char *)(v9 * a1 + a3 + 12);
	v15 = a5 - a7;
	v17 = v9 - (a5 - a7);
	v16 = gPageTable[a8].bytesPerRow - (a5 - a7);
	v11 = a4 - a6;

	do
	{
		v12 = a5 - a7;

		if (v15 > 0xC)
		{
			if (((unsigned char)v8 & 1) != 0)
			{
				*v10++ = *v8++;
				v12 = v15 - 1;
			}

			v14 = v12;
			v13 = v12 >> 2;
			memcpy(v10, v8, 4 * v13);
			v8 += 4 * v13;
			v10 += 4 * v13;
			v12 = v14 & 3;
		}

		memcpy(v10, v8, v12);
		v8 += v16 + v12;
		v10 += v17 + v12;
		--v11;
	}
	while (v11);
}

void MCGABlitM2V(int a1, int a2, int a3, int a4, unsigned int a5, int a6, unsigned char *a7)
{
	unsigned char *v7; // edi
	unsigned int v8; // ebx
	unsigned int v11; // ecx
	unsigned int v12; // ecx
	unsigned char v13; // [esp-4h] [ebp-14h]

	v7 = (unsigned char *)(a2 + gYLookup[a1] + gPageTable[a3].begin);
	v8 = gPageTable[a3].bytesPerRow - a5;

	do
	{
		v11 = a5;

		if (a5 > 0xC)
		{
			if (((unsigned char)a7 & 1) != 0)
			{
				*v7++ = *a7++;
				v11 = a5 - 1;
			}

			v13 = v11;
			v12 = v11 >> 2;
			memcpy(v7, a7, 4 * v12);
			a7 += 4 * v12;
			v7 += 4 * v12;
			v11 = v13 & 3;
		}

		memcpy(v7, a7, v11);
		a7 += a6 - a5 + v11;
		v7 += v8 + v11;
		--a4;
	}
	while (a4);
}

void MCGABlitMT2V(int a1, int a2, int a3, int a4, int a5, int a6, unsigned char a7, unsigned char *a8)
{
	unsigned char *v8; // edi
	unsigned int v9; // ebx
	unsigned int v12; // ecx
	unsigned char v13; // al

	v8 = (unsigned char *)(a2 + gYLookup[a1] + gPageTable[a3].begin);
	v9 = gPageTable[a3].bytesPerRow - a5;

	do
	{
		v12 = a5;

		do
		{
			v13 = *a8++;

			if (v13 != a7)
				*v8 = v13;

			++v8;
			--v12;
		}
		while (v12);

		a8 += a6 - a5;
		v8 += v9;
		--a4;
	}
	while (a4);
}

// Function to generate the carry flag for a left shift (x << y)
static inline char CFSHL(unsigned int x, unsigned int y)
{
	unsigned int high_bits;

	// Calculate the number of bits in the integer type
	unsigned int num_bits = sizeof(x) * 8;  // 8 bits per byte

	// If shift is larger or equal to the bit size of the integer, all bits are shifted out
	if (y >= num_bits)
	{
		return x != 0; // If x is not 0, shifting all bits out means at least one 1 was shifted out
	}

	// Calculate the carry by checking if any of the high bits that would be shifted out are set
	high_bits = x >> (num_bits - y);

	// If any of the high bits that would be shifted out are 1, a carry occurs
	return high_bits != 0;
}

// Rotate left function for unsigned int
static inline unsigned int __ROL__(unsigned int value, int count)
{
	const unsigned int nbits = sizeof(unsigned int) * 8; // Number of bits in the type

	if (count > 0)
	{
		count %= nbits;  // Ensure the count is within the bit-width
		unsigned int high = value >> (nbits - count);  // Get the bits that will be rotated to the right
		value <<= count;  // Shift left
		value |= high;	// Wrap the high bits around
	}
	else
	{
		count = -count % nbits;  // Ensure the count is within the bit-width
		unsigned int low = value << (nbits - count);  // Get the bits that will be rotated to the left
		value >>= count;  // Shift right
		value |= low;	 // Wrap the low bits around
	}

	return value;
}

static inline unsigned char  __ROL1__(unsigned char value, int count)
{
	return __ROL__((unsigned char)value, count);
}

void MCGABlitMono(int a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned char *a8)
{
	unsigned char *v8; // edi
	unsigned int v9; // ebx
	unsigned char v12; // ah
	int v13; // ecx
	unsigned char v14; // al
	int v15; // tt

	v8 = (unsigned char *)(a2 + gYLookup[a1] + gPageTable[a3].begin);
	v9 = gPageTable[a3].bytesPerRow - a5;
	v12 = a7;
//	*(&loc_10BDA + 2) = gColor; // This seems to write the value of gColor

	do
	{
		v13 = a5;

		do
		{
			v14 = *a8++;

			if ((v14 & v12) != 0)
//				*v8 = 18;
				*v8 = gColor;

			++v8;
			--v13;
		}
		while (v13);

		a8 -= a5;
		v8 += v9;
		v15 = CFSHL(v12, 1);
		v12 = __ROL1__(v12, 1);

		if (v15)
			a8 += a6;

		--a4;
	}
	while (a4);
}

