bit99midi: macos_midi.c main.c bit99_midi.c bit99.c
	clang main.c macos_midi.c bit99_midi.c bit99.c -framework CoreMIDI -framework CoreFoundation -o bit99midi

clean:
	rm *.o bit99midi

