//실행시 리시브할 데이터 타입 argv로 입력받기
//추가로 msgq 상태정보 가져오기
//리시버 마지막 파라미터 바꾸기
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFF_SIZE 1024

typedef struct {
	long data_type;
	int data_num;
	char data_buff[BUFF_SIZE];
}msg_t;

void main () {
	int id;
	int ndx = 0;
	int i = 0;
	msg_t data1;
	struct msqid_ds msqstat;

	if ((id = msgget((key_t)11608, IPC_CREAT | 0666)) == -1) {
		perror("msgget1");
		exit(1);
	}
	
	printf("recieving2\n");

	while(1) {
		if (msgrcv(id, &data1, sizeof(msg_t) - sizeof(long), i, IPC_NOWAIT) == -1 ) {
			perror("msgrcv1");
			exit(1);
		}
		
		i = (i++)%3;
		printf("\nrcv2:\t%d\t%s", data1.data_num, data1.data_buff);
		sleep(1);
	}
}

