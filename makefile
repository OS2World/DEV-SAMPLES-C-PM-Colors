# nmake makefile
# 
# Tools used:
#  Compile::Resource Compiler
#  Compile::GNU C
#  Make: nmake
all : colors.exe

colors.exe: colors.obj colors.def colors.res 
	gcc -Zomf colors.obj colors.def colors.res -o colors.exe
	wrc colors.res

colors.obj : colors.c colors.h
	gcc -Wall -Zomf -c -O2 colors.c -o colors.obj

colors.res : colors.rc colors.ico 
	wrc -r colors.rc

clean :
	rm -rf *exe *res *obj