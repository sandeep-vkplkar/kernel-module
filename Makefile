obj-m += kernelmodule.o
all: user_program kernelmodule.ko
user_program: user_program.c
gcc -o user_program user_program.c
kernelmodule.ko: kernelmodule.c
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
rm -f user_program kernelmodule.ko kernelmodule.o
kernelmodule.mod.o
