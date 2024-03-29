#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void parent();
void child();

#define SHMKEY 859047
#define BUFF_SZ sizeof ( int )

typedef struct QNode
{
	struct QNode *prev, *next;
	unsigned pageNumber;
} QNode;

typedef struct Queue 
{
	unsigned count;
	unsigned numberOfFrames;
	QNode *front, *rear;
} Queue;

typedef struct Hash
{
	int capacity;
	QNode** array;
} Hash;

QNode* newQNode(unsigned pageNumber)
{
	QNode* temp = (QNode*)malloc(sizeof(QNode));
	temp->pageNumber = pageNumber;

	temp->prev = temp->next = NULL;
	
	return temp;
}

Queue* createQueue(int numberOfFrames)
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));

	queue->count = 0;
	queue->front = queue->rear = NULL;

	queue->numberOfFrames = numberOfFrames;

	return queue;
}

int main(){	
	const unsigned int maxTimeBetweenNewProcsNS = 1000000;
	const unsigned int maxTimeBetweenNewProcsSecs = 1;
	int procCount = 0;
	int lastProcNan = 0;
	int lastProcSec = 0;
	int frameTable = [][];	

	switch ( fork() )
	{
		case -1:
			printf("Failed to Fork");
			return ( 1 );

		case 0:
			child();
			break;
		
		default:
			parent();
			break;
	}
	
	return ( 0 );
}

void parent()
{
	int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 | IPC_CREAT );
	if ( shmid == -1 )
	{
		printf("Parent: ... Error in shmget ...");
		exit (1);
	}

	char * paddr = ( char * )( shmat ( shmid, 0, 0 ) );
	int * pint = ( int * )( paddr );

	int i = 0;
	for ( i = 0 ; i < 10; i++ )
	{
		sleep ( 2 );
		*pint = 10 *i ;
		printf("Parent: Written Val.: = "); //*pint;
	}
}

void child()
{
	sleep ( 5 );
	int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 );

	if ( shmid == -1 )
	{
		printf("Child: ... Error in shmget ...");
		exit ( 1 );
	}

	int * cint = (int * )( shmat ( shmid, 0, 0 ) );

	int i = 0;
	for ( i = 0; i < 10; i++ )
	{
		sleep ( 1 );
		printf("Child: Read Val. = "); //*cint << endl;
	}
}
