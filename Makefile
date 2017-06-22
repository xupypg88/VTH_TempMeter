all: arm

arm: clean
	arm-linux-gnueabi-g++-4.7 -W main.cpp -o temp
clean:
	rm -rf temp *.o