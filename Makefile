make:
	python3 setup.py build

install:
	python3 setup.py install

csocks:
	gcc my_client.c -g -Wall -O2 -o client.run
	gcc my_server.c -g -Wall -O2 -o server.run
