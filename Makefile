obj-m := mycall.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

ins:
	sudo insmod mycall.ko

rm:
	sudo rmmod mycall

.PHONY:test
test:test.c
	gcc -g test.c -o test.exe
	./test.exe

.PHONY:clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm *.exe