
all: tcpclient tcpserver

tcpserver: tcpserver.c
	gcc -o tcpserver -O0 -ggdb tcpserver.c

tcpclient: tcpclient.c
	gcc -o tcpclient -O0 -ggdb tcpclient.c

clean:
	rm -rf tcpclient tcpserver
