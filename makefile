all:
	gcc src/main.c -I include -o vprobe

clean:
	rm -f vprobe