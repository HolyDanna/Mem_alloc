#Created under licence GNU GPL since 22 november 2015, 21:23

MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
cC90=-ansi
cerror=-Wall -Werror
cflag=-pedantic -fpic
csoflag=-shared -o libBeMa.so

all:build-pre build-post

build-pre: Mem_Alloc.o Mem_Free.o
	gcc $(cC90) $(cerror) $(csoflag) $^

Mem_Alloc.o: ./lib/Mem_Alloc.c
	gcc $(cC90) $(cerror) $(cflag) -c $< -o $@

Mem_Free.o: ./lib/Mem_Free.c
	gcc $(cC90) $(cerror) $(cflag) -c $< -o $@


build-post: 
	rm -f *.o
	rm -f *~



testBeMa: ./c_file/testBeMa.c 
	gcc $(cC90) $(cerror) $(cflag) $< -L./ -I./ -lBeMa
	export LD_LIBRARY_PATH=./lib/:./&&./a.out
	rm -f a.out

testmem: ./c_file/testmem.c
	gcc $(cflag) $< -L./ -I./ -lBeMa
	@echo "Don't forget to use in first the command"
	@echo "export LD_LIBRARY_PATH=./lib/:./"	
		
testmem_exec: ./c_file/testmem.c
	gcc $(cflag) $< -L./ -I./ -lBeMa
	export LD_LIBRARY_PATH=./&&./a.out
	rm -f a.out

clean:
	rm -f *.so
	rm -f *.out
	rm -f ./lib/*.o
	rm -f ./lib/*~
	rm -f *~
	rm -f ./c_file/*~
