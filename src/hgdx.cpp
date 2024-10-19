#include <dos.h>
#include <conio.h>
#include <string.h>
#include "helix.h"


//
// This basically is all the decompiled code of file "HGDX.ASM".
//
// NO IDEA IF THAT WOULD EVEN WORK AS IT WAS NEVER USED AT ALL.
//

#if 1
PAGE_STRUCT ModeWPageTable[3] = {
	{ 1, 0x0A0000, 0x5EEC, 0x168, 0x10E, 0x5A, 0, 0 },
	{ 1, 0x0A5EEC, 0x5EEC, 0x168, 0x10E, 0x5A, 0, 0 },
	{ 0, 0x0ABDD8, 0x4218, 0x168, 0x0B4, 0x5A, 0, 0 }
};

PAGE_STRUCT ModeXPageTable[4] = {
	{ 1, 0x0A0000, 0x4B00, 0x140, 0x0F0, 0x50, 0, 0 },
	{ 1, 0x0A4B00, 0x4B00, 0x140, 0x0F0, 0x50, 0, 0 },
	{ 1, 0x0A9600, 0x4B00, 0x140, 0x0F0, 0x50, 0, 0 },
	{ 0, 0x0AE100, 0x1EF0, 0x140, 0x63, 0x50, 0, 0 }
};

PAGE_STRUCT ModeYPageTable[3] = {
	{ 1, 0x0A0000, 0x7D00, 0x140, 0x190, 0x50, 0, 0 },
	{ 1, 0x0A7D00, 0x7D00, 0x140, 0x190, 0x50, 0, 0 },
	{ 0, 0x0AFA00, 0x5F0, 0x140, 0x13, 0x50, 0, 0 }
};

PAGE_STRUCT ModeZPageTable[2] = {
	{ 1, 0x0A0000, 0x0A8C0, 0x168, 0x1E0, 0x5A, 0, 0 },
	{ 0, 0x0AA8C0, 0x5730, 0x168, 0x0F8, 0x5A, 0, 0 }
};

unsigned short word_106F9[17] = {
	0x6B00, 0x5901, 0x5A02, 0x8E03, 0x5E04, 0x8A05, 0x4006, 0xF007,
	0x6109, 0x2410, 0xAA11, 0x1B12, 0x2D13, 0x0014, 0x1B15, 0x3A16,
	0xE317
};

unsigned short word_108E1[3] = { 0x4009, 0x14, 0x0E317 };

unsigned short word_10990[17] = {
	0x6B00, 0x5901, 0x5A02, 0x8E03, 0x5E04, 0x8A05, 0x0D06, 0x3E07,
	0x4009, 0x0EA10, 0x0AC11, 0x0DF12, 0x2D13, 0x14, 0x0E715, 0x616,
	0x0E317
};

unsigned char byte_109B2[4] = { 0x0F, 0x0E, 0x0C, 8 };
unsigned char byte_109B6[4] = { 1, 3, 7, 0x0F };

int dword_109BA;


void ModeWInit(void)
{
	int *v0; // ebx
	int v1; // ecx
	int v2; // eax
	unsigned int bytesPerRow; // edx

	gPages = sizeof(ModeWPageTable) / sizeof(PAGE_STRUCT);
	memcpy(gPageTable, ModeWPageTable, sizeof(ModeWPageTable));
	v0 = gYLookup;
	v1 = sizeof(gYLookup) / sizeof(unsigned int);
	v2 = 0;
	bytesPerRow = ModeWPageTable[0].bytesPerRow;

	do
	{
		*v0++ = v2;
		v2 += bytesPerRow;
		--v1;
	}
	while (v1);
}

int ModeXValid(void)
{
	return 1;
}

void ModeXSetPage(int a1, int a2, int a3, int a4)
{
	unsigned int v4; // edi
	unsigned char v5[2]; // bx
	unsigned char v6[2]; // cx
	unsigned char v7; // al MAPDST
	unsigned char v9[2]; // ax

	dword_109BA = a4;
	v4 = ((unsigned int)a3 >> 2) + gYLookup[a2];
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

int ModeXGetPage(void)
{
	return dword_109BA;
}

void ModeXClear(int a1)
{
	unsigned int begin; // edi
	unsigned int size; // ecx
	int v3; // ecx
	unsigned int v4; // ebx
	void *v5; // edi
	char v6; // cf
	unsigned int i; // ecx

	outp(0x3C5, 0xF);
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

void ModeXSetPixel(int a1, int a2, int a3)
{
	outp(0x3C5, 1 << (a2 & 3));
	*(unsigned char *)(gYLookup[a1] + gPageTable[a3].begin + ((unsigned int)a2 >> 2)) = gColor;
}

int ModeXGetPixel(void)
{
	int result; // eax

	return result;
}

void ModeXHLine(int a1, int a2, int a3, int a4)
{
	unsigned char v4; // bh
	unsigned char v5; // bl
	unsigned int v6;  // edi
	unsigned short v7; // ax
	unsigned int v8;  // edi
	unsigned int v9;  // ecx
	int v10;		  // ecx
	unsigned int v11; // ebx
	unsigned int *v12; // edi
	unsigned char v13; // cf
	unsigned int i;   // ecx
	unsigned short v15[2]; // [esp-8h] [ebp-10h]
	unsigned char v16; // [esp-4h] [ebp-Ch]

	// Replacing macro usage with direct array access
	v4 = byte_109B2[a2 & 3];
	v5 = byte_109B6[a1 & 3];
	v6 = ((unsigned int)a2 >> 2) + gYLookup[a3] + gPageTable[a4].begin;

	// Directly accessing low and high bytes of gColor
	unsigned char low_byte_gColor = (unsigned char)(gColor & 0xFF);
	unsigned char high_byte_gColor = (unsigned char)((gColor >> 8) & 0xFF);

	// Assigning high byte to HIBYTE(v7) equivalent
	v7 = (high_byte_gColor << 8) | low_byte_gColor;

	if ((unsigned int)a1 >> 2 == (unsigned int)a2 >> 2)
	{
		outp(0x3C5, v4 & v5);
		*(unsigned char *)v6 = gColor;
	}
	else
	{
		outp(0x3C5, v4);
		*(unsigned char *)v6 = gColor;
		v8 = v6 + 1;
		v9 = ((unsigned int)a1 >> 2) - ((unsigned int)a2 >> 2) - 1;
		outp(0x3C5, 0xFu);
		v7 = (high_byte_gColor << 8) | low_byte_gColor;

		if (v9)
		{
			if (v9 >= 0xC)
			{
				v16 = v5;
				v10 = -v8 & 3;
				v11 = ((unsigned int)a1 >> 2) - ((unsigned int)a2 >> 2) - 1 - v10;
				memset((void *)v8, high_byte_gColor, v10);
				v12 = (unsigned int *)(v8 + v10);
				v15[1] = v7;
				v15[0] = v7;
				memset32(v12, *(unsigned int *)v15, v11 >> 2);
				v8 = (unsigned int)&v12[v11 >> 2];
				v9 = v11 & 3;
				v5 = v16;
			}

			v13 = v9 & 1;

			for (i = v9 >> 1; i; --i)
			{
				*(unsigned short *)v8 = v7;
				v8 += 2;
			}

			if (v13)
				*(unsigned char *)v8++ = high_byte_gColor;
		}

		outp(0x3C5, v5);
		*(unsigned char *)v8 = high_byte_gColor;
	}
}

void ModeXVLine(int a1, int a2, int a3, int a4)
{
	unsigned int bytesPerRow; // edx
	unsigned char *v6; // ebx
	int v7; // ecx
	char v8; // al

	outp(0x3C5, 1 << (a3 & 3));
	bytesPerRow = gPageTable[a4].bytesPerRow;
	v6 = (unsigned char *)(gYLookup[a2] + gPageTable[a4].begin + ((unsigned int)a3 >> 2));
	v7 = a1 - a2 + 1;
	v8 = gColor;

	do
	{
		*v6 = v8;
		v6 += bytesPerRow;
		--v7;
	}
	while (v7);
}

void ModeXLine(int a1, int a2, int a3, int a4, int a5)
{
	return;
}

void ModeXHLineROP(int a1, int a2, int a3, int a4)
{
	return;
}

void ModeXVLineROP(int a1, int a2, int a3, int a4)
{
	return;
}

void ModeXBlitRLE2V(int a1, int a2, int a3, int a4)
{
	return;
}

void ModeWSetMode(void)
{
	unsigned char v0; // al
	unsigned short *v1; // esi
	int v2; // ecx
	unsigned short v3; // ax

	setVideoMode(0x03);
	outpw(0x3C4, 0x604);
	outpw(0x3C4, 0x100);
	outp(0x3C2, 0xE7);
	outpw(0x3C4, 0x300);
	outp(0x3D4, 0x11);
	v0 = inp(0x3D5);
	outp(0x3D5, v0 & 0x7F);
	v1 = word_106F9;
	v2 = sizeof(word_106F9) / sizeof(unsigned short);

	do
	{
		v3 = *v1++;
		outpw(0x3D4, v3);
		--v2;
	}
	while (v2);

	outpw(0x3C4, 0xF02);
	memset((void *)0xA0000, 0, 0x10000);
	ModeXSetPage(1, 0, 0, 0);
}

void ModeXInit(void)
{
	int *v0; // ebx
	int v1; // ecx
	int v2; // eax
	unsigned int bytesPerRow; // edx

	gPages = sizeof(ModeXPageTable) / sizeof(PAGE_STRUCT);
	memcpy(gPageTable, ModeXPageTable, sizeof(ModeXPageTable));
	v0 = gYLookup;
	v1 = sizeof(gYLookup) / sizeof(unsigned int);;
	v2 = 0;
	bytesPerRow = ModeXPageTable[0].bytesPerRow;

	do
	{
		*v0++ = v2;
		v2 += bytesPerRow;
		--v1;
	}
	while (v1);
}

void ModeXSetMode(void)
{
	unsigned char v0; // al
	unsigned short *v1; // esi
	int v2; // ecx
	unsigned short v3; // ax

	setVideoMode(0x03);
	outpw(0x3C4, 0x604);
	outpw(0x3C4, 0x100);
	outp(0x3C2, 0xE3);
	outpw(0x3C4, 0x300);
	outp(0x3D4, 0x11);
	v0 = inp(0x3D5);
	outp(0x3D5, v0 & 0x7F);
	v1 = word_106F9;
	v2 = 10;

	do
	{
		v3 = *v1++;
		outpw(0x3D4u, v3);
		--v2;
	}
	while (v2);

	outpw(0x3C4, 0xF02);
	memset((void *)0xA0000, 0, 0x10000);
	ModeXSetPage(1, 0, 0, 0);
}

void ModeYInit(void)
{
	int *v0; // ebx
	int v1; // ecx
	int v2; // eax
	unsigned int bytesPerRow; // edx

	gPages = sizeof(ModeYPageTable) / sizeof(PAGE_STRUCT);
	memcpy(gPageTable, ModeYPageTable, sizeof(ModeYPageTable));
	v0 = gYLookup;
	v1 = sizeof(gYLookup) / sizeof(unsigned int);;
	v2 = 0;
	bytesPerRow = ModeYPageTable[0].bytesPerRow;

	do
	{
		*v0++ = v2;
		v2 += bytesPerRow;
		--v1;
	}
	while (v1);
}

void ModeYSetMode(void)
{
	unsigned short *v0; // esi
	int v1; // ecx
	unsigned short v2; // ax

	setVideoMode(0x03);
	outpw(0x3C4, 0x604);
	v0 = word_108E1;
	v1 = sizeof(word_108E1) / sizeof(unsigned short);

	do
	{
		v2 = *v0++;
		outpw(0x3D4, v2);
		--v1;
	}
	while (v1);

	outpw(0x3C4, 0xF02);
	memset((void *)0xA0000, 0, 0x10000);
	ModeXSetPage(1, 0, 0, 0);
}

void ModeZInit(void)
{
	int *v0; // ebx
	int v1; // ecx
	int v2; // eax
	unsigned int bytesPerRow; // edx

	gPages = sizeof(ModeZPageTable) / sizeof(PAGE_STRUCT);
	memcpy(gPageTable, ModeZPageTable, sizeof(ModeZPageTable));
	v0 = gYLookup;
	v1 = sizeof(gYLookup) / sizeof(unsigned int);;
	v2 = 0;
	bytesPerRow = ModeZPageTable[0].bytesPerRow;

	do
	{
		*v0++ = v2;
		v2 += bytesPerRow;
		--v1;
	}
	while (v1);
}

void ModeZSetMode(void)
{
	unsigned char v0; // al
	unsigned short *v1; // esi
	int v2; // ecx
	unsigned short v3; // ax

	setVideoMode(0x03);
	outpw(0x3C4, 0x604);
	outpw(0x3C4, 0x100);
	outp(0x3C2, 0xE7);
	outpw(0x3C4, 0x300);
	outp(0x3D4, 0x11);
	v0 = inp(0x3D5);
	outp(0x3D5, v0 & 0x7F);
	v1 = word_10990;
	v2 = sizeof(word_10990) / sizeof(unsigned short);

	do
	{
		v3 = *v1++;
		outpw(0x3D4, v3);
		--v2;
	}
	while (v2);

	outpw(0x3C4, 0xF02);
	memset((void *)0xA0000, 0, 0x10000);
	ModeXSetPage(1, 0, 0, 0);
}

void ModeXFillBox(int a1, int a2, int a3, int a4, int a5)
{
	unsigned char v5; // bh
	unsigned char v6; // bl
	unsigned char *v7; // edi
	unsigned int v8; // ecx
	unsigned char v9[2]; // ax
	unsigned char *v10; // edi
	unsigned int v11; // ecx
	unsigned int v12; // edx
	unsigned int v13; // ebx
	unsigned int v14; // edi
	unsigned int v15; // esi
	unsigned int v16; // eax
	unsigned int v17; // ecx
	unsigned int v18; // ecx
	unsigned char v19; // bl
	unsigned int *v20; // edi
	unsigned int v21; // ecx
	unsigned char v22; // cf
	unsigned int i; // ecx
	unsigned short v24[2]; // [esp-4h] [ebp-20h]
	unsigned int v25; // [esp-4h] [ebp-20h]
	unsigned int v26; // [esp+Ch] [ebp-10h]
	unsigned int v27; // [esp+10h] [ebp-Ch]
	unsigned int bytesPerRow; // [esp+18h] [ebp-4h]

	bytesPerRow = gPageTable[a5].bytesPerRow;
	v5 = byte_109B2[a4 & 3];
	v6 = byte_109B6[a2 & 3];
	v7 = (unsigned char *)(((unsigned int)a4 >> 2) + gYLookup[a3] + gPageTable[a5].begin);
	v26 = (unsigned int)v7;
	v27 = ((unsigned int)a2 >> 2) - ((unsigned int)a4 >> 2);
	v8 = a1 - a3 + 1;
	v9[1] = gColor;

	if ((unsigned int)a2 >> 2 == (unsigned int)a4 >> 2)
	{
		outp(0x3C5, v5 & v6);

		do
		{
			*v7 = v9[1];
			v7 += bytesPerRow;
			--v8;
		}
		while (v8);
	}
	else
	{
		outp(0x3C5, v5);

		do
		{
			*v7 = v9[1];
			v7 += bytesPerRow;
			--v8;
		}
		while (v8);

		outp(0x3C5, v6);
		v10 = (unsigned char *)(v27 + v26);
		v11 = a1 - a3 + 1;

		do
		{
			*v10 = v9[1];
			v10 += bytesPerRow;
			--v11;
		}
		while (v11);

		outp(0x3C5, 0xF);
		v12 = v27 - 1;
		v13 = bytesPerRow - (v27 - 1);
		v14 = v26 + 1;
		v9[0] = v9[1];
		v15 = a1 - a3 + 1;
		v24[1] = *(unsigned short *)v9;
		v24[0] = *(unsigned short *)v9;
		v16 = *(unsigned int *)v24;

		do
		{
			v17 = v27 - 1;

			if (v27 != 1)
			{
				if (v12 >= 0xC)
				{
					v25 = v13;
					v18 = -v14 & 3;
					v19 = v12 - v18;
					memset((void *)v14, v16, v18);
					v20 = (unsigned int *)(v14 + v18);
					v21 = (v12 - v18) >> 2;
					memset32(v20, v16, v21);
					v14 = (unsigned int)&v20[v21];
					v17 = v19 & 3;
					v13 = v25;
				}

				v22 = v17 & 1;

				for (i = v17 >> 1; i; --i)
				{
					*(unsigned short *)v14 = v16;
					v14 += 2;
				}

				if (v22)
					*(unsigned char *)v14++ = v16;
			}

			v14 += v13;
			--v15;
		}
		while (v15);
	}
}
#endif

