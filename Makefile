all: barcode_reader
barcode_reader: get_event.c
	gcc -Wall get_event.c -o get_event -lm
install: get_event
	mkdir -p ./usr/bin
	cp rikos-tracker ./usr/bin/
