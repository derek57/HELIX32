#include "helix.h"


// Custom   table is at 0x3DFC6C
// Original table is at 0x0F0000 - holding 5 offsets

// Define and initialize an instance of VGT
#if 1
struct VGT MCGADriver = {
    MCGAValid,                     // callback: Initialize with the address of sampleFunction
    "MCGA 320x200 256 Color",      // name: Initialize with "Sample Name"
    320,                           // id: Initialize with 1
    200,
    8,
    3,
    MCGAInit,
    MCGASetMode,
    MCGASetPage,
    MCGAGetPage,
    MCGAClear,
    MCGASetPixel,
    MCGAGetPixel,
    MCGAHLine,
    MCGAVLine,
    MCGALine,
    MCGAFillBox,
    MCGAHLineROP,
    MCGAVLineROP,
    MCGABlitRLE2V,
    MCGABlitV2M,
    MCGABlitM2V,
    MCGABlitMT2V,
    MCGABlitMono
};

struct VGT *VGTBegin = &MCGADriver;
#else
struct VGT MODEWDriver = {
    ModeXValid,                     // callback: Initialize with the address of sampleFunction
    "Mode-W 360x270 256 Color",      // name: Initialize with "Sample Name"
    360,                           // id: Initialize with 1
    270,
    8,
    4,
    ModeWInit,
    ModeWSetMode,
    ModeXSetPage,
    ModeXGetPage,
    ModeXClear,
    ModeXSetPixel,
    ModeXGetPixel,
    ModeXHLine,
    ModeXVLine,
    ModeXLine,
    ModeXFillBox,
    ModeXHLineROP,
    ModeXVLineROP,
    ModeXBlitRLE2V
};

struct VGT ModeXDriver = {
    ModeXValid,                     // callback: Initialize with the address of sampleFunction
    "Mode-X 320x240 256 Color",      // name: Initialize with "Sample Name"
    320,                           // id: Initialize with 1
    240,
    8,
    4,
    ModeXInit,
    ModeXSetMode,
    ModeXSetPage,
    ModeXGetPage,
    ModeXClear,
    ModeXSetPixel,
    ModeXGetPixel,
    ModeXHLine,
    ModeXVLine,
    ModeXLine,
    ModeXFillBox,
    ModeXHLineROP,
    ModeXVLineROP,
    ModeXBlitRLE2V
};

struct VGT ModeYDriver = {
    ModeXValid,                     // callback: Initialize with the address of sampleFunction
    "Mode-Y 320x400 256 Color",      // name: Initialize with "Sample Name"
    320,                           // id: Initialize with 1
    400,
    8,
    4,
    ModeYInit,
    ModeYSetMode,
    ModeXSetPage,
    ModeXGetPage,
    ModeXClear,
    ModeXSetPixel,
    ModeXGetPixel,
    ModeXHLine,
    ModeXVLine,
    ModeXLine,
    ModeXFillBox,
    ModeXHLineROP,
    ModeXVLineROP,
    ModeXBlitRLE2V
};

struct VGT ModeZDriver = {
    ModeXValid,                     // callback: Initialize with the address of sampleFunction
    "Mode-Z 360x480 256 Color",      // name: Initialize with "Sample Name"
    360,                           // id: Initialize with 1
    480,
    8,
    4,
    ModeZInit,
    ModeZSetMode,
    ModeXSetPage,
    ModeXGetPage,
    ModeXClear,
    ModeXSetPixel,
    ModeXGetPixel,
    ModeXHLine,
    ModeXVLine,
    ModeXLine,
    ModeXFillBox,
    ModeXHLineROP,
    ModeXVLineROP,
    ModeXBlitRLE2V
};

struct VGT *VGTBegin = &ModeWDriver;
#endif

struct VGT *VGTEnd = { 0 };


// Define the static member function outside of the header
void VGT::SetColor(int color)
{
	gColor = color;
}

