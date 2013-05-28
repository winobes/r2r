objects = r2r.o windows.o callbacks.o file_handles.o

r2r : $(objects)
	gcc -Wall -o r2r $(objects) `pkg-config --libs gtk+-3.0` -lcsv

r2r.o : r2r.c r2r.h
	gcc `pkg-config --cflags gtk+-3.0` -c -o r2r.o r2r.c

windows.o : windows.c windows.h r2r.h
	gcc `pkg-config --cflags gtk+-3.0` -c -o windows.o windows.c

callbacks.o : callbacks.c callbacks.h r2r.h
	gcc `pkg-config --cflags gtk+-3.0` -c -o callbacks.o callbacks.c

file_handles.o : file_handles.c file_handles.h r2r.h 
	gcc `pkg-config --cflags gtk+-3.0` -c -o file_handles.o file_handles.c

.PHONY : clean
clean : 
	rm -f $(objects) r2r
