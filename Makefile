all: arm
arm: clean
	arm-linux-gnueabi-g++-4.7 *.cpp -o bin/tempsensor 
clean:
	rm -rf *.o bin/tempsensor 
