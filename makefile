CPPFLAGS = /4r /d2 /otexan

obj_dir           = obj
lib_dir           = lib
src_dir           = src

.h   :              $(src_dir)
.cpp :              $(src_dir)
.c   :              $(src_dir)
.asm :              $(src_dir)
.obj :              $(obj_dir)
.lib :              $(lib_dir)

OBJS = &
hkernel.obj &
palette.obj &
vmode.obj &
vgt.obj &
hgd13.obj &
hgdx.obj

all : helix.lib

clean : .symbolic
    del /f /q $(lib_dir)\*.lib
    rem > $(lib_dir)\blank
    del /f /q $(obj_dir)\*.obj
    rem > $(obj_dir)\blank
    del /f /q *.err
    del /f /q *.lnk
    del /f /q *.log

helix.lib: $(OBJS) .symbolic
    %create helix.lnk
    for %i in ($(OBJS)) do %append helix.lnk -+$(obj_dir)\%i
    wlib $(LIB_DIR)\$^@ /n /b @helix.lnk


.cpp.obj: .AUTODEPEND
    wpp386 $(CPPFLAGS) $[* -fo=$(obj_dir)\$^&

