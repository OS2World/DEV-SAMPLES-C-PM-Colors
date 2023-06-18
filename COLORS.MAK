colors.exe: colors.l colors.obj colors.def colors.res
        ilink.exe -NOL -O:colors.exe -BAS:0x10000 -PM:PM -MAP -DE -DB @colors.l
        dllrname.exe colors.exe CPPOM30=OS2OM30
        rc -n -p -x2 colors.res

colors.obj: colors.c colors.h
        icc.exe -C -Q -Ti -Ge+ -Gd+ -Gm+ -G5 -Ss colors.c

colors.res: colors.rc colors.dlg colors.ico colors.h
        rc -n -r colors.rc

