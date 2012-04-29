emulator : src/emulator.c
	gcc -pthread -include src/config1.h -o bin/emulator1 src/emulator.c
	gcc -pthread -include src/config2.h -o bin/emulator2 src/emulator.c
	gcc -pthread -include src/config3.h -o bin/emulator3 src/emulator.c
	gcc -pthread -include src/config4.h -o bin/emulator4 src/emulator.c
	gcc -pthread -include src/config5.h -o bin/emulator5 src/emulator.c

clean : 
	rm -rf bin/*