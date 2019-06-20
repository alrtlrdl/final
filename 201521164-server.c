#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>
#define SOCK_PATH "/tmp/s1521164_socket"

struct math {
	char x[10];
	int a, b, c;
};

struct math str;

int main () {
	int s, s2, len;
	unsigned t;
	struct sockaddr_un local, remote;
	pid_t pid;

	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);

	if (bind(s, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(s, 5) == -1) {
		perror("listen");
		exit(1);
	}

	for (;;) {
		int done, n;
		printf("Waiting for a connection...\n");
		t = sizeof(remote);

		if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
			perror("accept");
			exit(1);
		}

		printf("Connected.\n");

		done = 0;

		do {
			n = recv(s2, &str, sizeof(str), 0);
			printf("값 받은 %d\t%d\t%s\n",str.a,str.b,str.x);
			if (n <= 0) {
				if (n < 0)
					perror("recv");
				done = 1;
			}
			if ((pid = fork()) < 0) 
					perror("fork");
			else if (pid == 0) {
//				sem_wait(&sem);
				printf("자식프로세스");
				if (!strcmp(str.x,"+")) 
					str.c = str.a + str.b;
				if (!strcmp(str.x,"-"))
					str.c = str.a - str.b;
				if (!strcmp(str.x,"*"))
					str.c = str.a * str.b;
				if (!strcmp(str.x,"/"))
					str.c = str.a / str.b;
				printf("자식 값 : %d\t%d\t%d\t%s", str.a,str.b,str.c,str.x);
//				sem_post(&sem);
			}
			else {
				sleep(5);
				printf("값 가져오는 중...\n");
//				kill(pid,SIGKILL);
				if (!done) {
					if(send(s2, &str, sizeof(str), 0) <0) {
						perror("send");
						done = 1;
					}
					else printf("값 보내는 중...\n");
				}
			}

			if (!strcmp(str.x,"q")||!strcmp(str.x,"qq")) {
				printf("클라이언트 연결 종료\n");
				break;
			}

/*			if (!done)
				if(send(s2, &str, sizeof(str), 0) < 0) {
					perror("send");
					done = 1;
				}
*/			
		}while (!done);
		
		if (!strcmp(str.x, "qq")){
			printf("서버종료\n");
			break;
		}

		close(s2);

	}

	return 0;
}
