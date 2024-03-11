CC=gcc
CFLAGS=-Wall

all: tcp_server tcp_client udp_server udp_client

udp_server: udp_server.c
	$(CC) $(CFLAGS) -o udp_server udp_server.c

udp_client: udp_client.c
	$(CC) $(CFLAGS) -o udp_client udp_client.c

tcp_server: tcp_server.c
	$(CC) $(CFLAGS) -o tcp_server tcp_server.c

tcp_client: tcp_client.c
	$(CC) $(CFLAGS) -o tcp_client tcp_client.c

clean:
	rm -f tcp_server tcp_client udp_server udp_client

