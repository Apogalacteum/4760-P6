#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

void parent();
void child();

#define SHMKEY 859047
#define BUFF_SZ sizeof ( int )

using namespace std;

struct Semaphore {
	int value;
	Queue<process> q;
};

P(struct Semaphore s)
{
	s.value = s.value - 1;
	if (s.value < 0)
	{
		q.push(p);
		block();
	}
	else
		return;
};

V(struct Semaphore s)
{
	s.value = s.value + 1;
	if (s.value <= 0)
	{
		q.pop();
		wakeup(p);
	}
};

int main(){	
	const unsigned int maxTimeBetweenNewProcsNS = 1000000;
	const unsigned int maxTimeBetweenNewProcsSecs = 1;
	int procCount = 0;
	int lastProcNan = 0;
	int lastProcSec = 0;
	

	switch ( fork() )
	{
		case -1:
			cerr << "Failed to Fork" << endl;
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
		cerr << "Parent: ... Error in shmget ..." << endl;
		exit (1);
	}

	char * paddr = ( char * )( shmat ( shmid, 0, 0 ) );
	int * pint = ( int * )( paddr );

	for ( int i ( 0 ); i < 10; i++ )
	{
		sleep ( 2 );
		*pint = 10 *i ;
		cout << "Parent: Written Val.: = " << *pint << endl;
	}
}

void child()
{
	sleep ( 5 );
	int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 );

	if ( shmid == -1 )
	{
		cerr << "Child: ... Error in shmget ..." << endl;
		exit ( 1 );
	}

	int * cint = (int * )( shmat ( shmid, 0, 0 ) );

	for ( int i ( 0 ); i < 10; i++ )
	{
		sleep ( 1 );
		cout << "Child: Read Val. = " << *cint << endl;
	}
}
