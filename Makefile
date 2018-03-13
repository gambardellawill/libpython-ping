make:
	gcc -fPIC -I/usr/include/python3.5m -shared spammodule.c -o spam.run
