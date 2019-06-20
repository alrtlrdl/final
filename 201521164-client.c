#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/s1521164_socket"

struct math {
	char x[10];
	int a, b, c;
};

int main () {
	int s, t, len;
	struct sockaddr_un remote;
	struct math str;
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	printf("Trying to connect...\n");

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	
	if (connect(s, (struct sockaddr *)&remote, len) == -1) {
		perror("connect");
		exit(1);
	}

	printf("Connected.\n");
	while(1) {
		printf("\n첫 번째 변수 > ");
		scanf("%d", &str.a);
		printf("두 번째 변수 > ");
		scanf("%d", &str.b);
		printf("연산 부호\n > ");
		scanf("%s", str.x);
		if (send(s, &str, sizeof(str), 0) == -1) {
			perror("send");
			exit(1);
		}
		
		if (!strcmp(str.x, "q")||!strcmp(str.x, "qq")) {
			printf("종료\n");
			break;
		}	

		if ((t = recv(s, &str, sizeof(str), 0)) > 0 ) {
			printf("답> %d\n%d", str.c, str.b);
		}
		else {
			if (t < 0)
				perror("recv");
			else
				printf("Server closed connection\n");
			exit(1);
		}
	}
	close(s);

	return 0;
}
