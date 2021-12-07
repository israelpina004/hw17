all: write control

write: write.o
		gcc -o write write.o

write.o: write.c
			gcc -c write.c

control: control.o
	gcc -o control control.o

control.o: control.c
	gcc -c control.c

clean:
	rm control
	rm write
	rm *.o
	rm *.txt
