#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHMKEY 859047

using namespace std;

int main()
{
	struct processControlBlock p;
	p.state = 0;
	p.totalCPUTime = 0;
	p.totalSysTime = 0;
	p.lastBurstTime = 0;
	p.pPriority = 0;

	

	return 0;
}
