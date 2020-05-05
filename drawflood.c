#include <stdio.h>
#include <getopt.h>

#include "drawflood.h"

#define LISTEN_ADDRESS_DEFAULT "::"

int main(int argc, char *argv[]) {
	printf("Starting drawflood\n");
	char* listen_address = LISTEN_ADDRESS_DEFAULT;

	int opt;
	while((opt = getopt(argc, argv, "l:")) != -1) {
		switch(opt) {
			case('l'):
				listen_address = optarg;
				break;
		}
	}


	net_listen(listen_address);
}

void net_listen(char* listen_address) {
	printf("Listening on %s\n", listen_address);
}
