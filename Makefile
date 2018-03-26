CC=gcc
CFLAGS=-g -Wall -std=c90 -pedantic -O2

make:
	python3 setup.py build

install:
	python3 setup.py install

csocks:
	$(CC) $(CFLAGS) my_client.c -o client.run
	$(CC) $(CFLAGS) my_server.c -o server.run

cping:
	$(CC) $(CFLAGS) ping_functions.c -o
