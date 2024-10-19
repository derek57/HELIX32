/*
 * Copyright (C) 2018, 2022 nukeykt
 *
 * This file is part of Blood-RE.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _HELIX_H_
#define _HELIX_H_
#include "typedefs.h"

struct QBITMAP {
    uchar bitModel;
    uchar tcolor;
    ushort cols;
    ushort rows;
    ushort stride;
    ushort xOrg;
    ushort yOrg;
    BYTE data[1];
};

#ifdef __cplusplus
extern "C" {
#endif
extern int gColor;
#ifdef __cplusplus
}
#endif

struct VGT {
    int (pascal *Valid)(void);
    char *name;
    int xRes;
    int yRes;
    int cRes;
    int model;
    void (pascal *Init)(void);
    void (pascal *SetMode)(void);
    void (pascal *SetPage)(int, int, int, int);
    int (pascal *GetPage)(void);
    void (pascal *Clear)(int);
    void (pascal *SetPixel)(int, int, int);
    int (pascal *GetPixel)(void);
    void (pascal *HLine)(int, int, int, int);
    void (pascal *VLine)(int, int, int, int);
    void (pascal *Line)(int, int, int, int, int);
    void (pascal *FillBox)(int, int, int, int, int);
    void (pascal *HLineROP)(int, int, int, int);
    void (pascal *VLineROP)(int, int, int, int);
    void (pascal *BlitRLE2V)(int, int, int, int);
    void (pascal *BlitV2M)(int, int, int, int, int, int, int, int);
    void (pascal *BlitM2V)(int, int, int, int, unsigned int, int, unsigned char *);
    void (pascal *BlitMT2V)(int, int, int, int, int, int, unsigned char, unsigned char *);
    void (pascal *BlitMono)(int, int, int, int, int, int, int, unsigned char *);

    static void SetColor(int color);
};

struct PAGE_STRUCT {
    unsigned int flags;
    unsigned int begin;
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned int bytesPerRow;
    unsigned int pad[2];
};


#ifdef __cplusplus
extern "C" {
#endif
extern struct VGT Video;
extern int gROP;
extern int gError;
extern int gPages;
extern struct PAGE_STRUCT gPageTable[4];
extern int gYLookup[1200];
extern struct VGT *VGTBegin;
extern struct VGT *VGTEnd;

void cdecl gRestoreMode(void);
int cdecl gFindMode(int, int, int, int);
void gEnumDrivers(void);
void cdecl gSetDACRange(int, int, unsigned char *);
void cdecl gSetDAC(int, int, int, int);

int pascal ModeXValid(void);
int pascal MCGAValid(void);
void pascal MCGAInit(void);
void pascal MCGASetMode(void);
void pascal MCGASetPage(int, int, int, int);
int pascal MCGAGetPage(void);
void pascal MCGAClear(int);
void pascal MCGASetPixel(int, int, int);
int pascal MCGAGetPixel(void);
void pascal MCGAHLine(int, int, int, int);
void pascal MCGAVLine(int, int, int, int);
void pascal MCGAFillBox(int, int, int, int, int);
void pascal MCGALine(int, int, int, int, int);
void pascal MCGAHLineROP(int, int, int, int);
void pascal MCGAVLineROP(int, int, int, int);
void pascal MCGABlitRLE2V(int, int, int, int);
void pascal MCGABlitV2M(int, int, int, int, int, int, int, int);
void pascal MCGABlitM2V(int, int, int, int, unsigned int, int, unsigned char *);
void pascal MCGABlitMT2V(int, int, int, int, int, int, unsigned char, unsigned char *);
void pascal MCGABlitMono(int, int, int, int, int, int, int, unsigned char *);

void pascal ModeWInit(void);
void pascal ModeXInit(void);
void pascal ModeYInit(void);
void pascal ModeZInit(void);

void pascal ModeWSetMode(void);
void pascal ModeXSetMode(void);
void pascal ModeYSetMode(void);
void pascal ModeZSetMode(void);

void pascal ModeXSetPage(int, int, int, int);
int pascal ModeXGetPage(void);
void pascal ModeXClear(int);
void pascal ModeXSetPixel(int, int, int);
int pascal ModeXGetPixel(void);
void pascal ModeXHLine(int, int, int, int);
void pascal ModeXVLine(int, int, int, int);
void pascal ModeXFillBox(int, int, int, int, int);
void pascal ModeXLine(int, int, int, int, int);
void pascal ModeXHLineROP(int, int, int, int);
void pascal ModeXVLineROP(int, int, int, int);
void pascal ModeXBlitRLE2V(int, int, int, int);

void setVideoMode(unsigned char mode);
void memset32(void *dest, unsigned int value, unsigned int count);
#ifdef __cplusplus
}
#endif

int cdecl gGetMode(void);
void cdecl gSetMode(int);
static inline void SetColor(int);

#endif
