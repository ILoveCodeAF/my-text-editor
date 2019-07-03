/* my libs */
#include "te.h"
#include "x.h"
#include "queue.h"

//#include <unistd.h> //sleep
#include <pthread.h>
#include <time.h>

typedef struct _BUFFER_THREAD_MUTEX BTM;

/* globals */
BTM btm_global;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//queue buffer;

void*
xAPI(void* param)
{
	printf("\nRunning thread xAPI...\n");
	xinit();
	xrun(param);
	xfree();
	xclose();
	printf("\nClosing thread xAPI...\n");
}

void*
ioAPI(void* param)
{
	printf("\nRunning thread ioAPI...\n");
	BTM* btm = (BTM*) param;
	
	clock_t t1 = clock(), t2;
	int period = 200;// 200ms
	int div = CLOCKS_PER_SEC/1000;//ms
	
	int  ksym;
	int not_break = 1;
	while(not_break){
		t2 = clock();
		if( (t2-t1)/div > 200){
			t1 = clock();
			pthread_mutex_lock(btm->mutex);
			while(!queue_empty(&btm->q)){
				ksym = queue_pop(&btm->q);
				if(ksym>31 && ksym<127){	
					printf("%c", (char)ksym);
				}
				if(ksym == XK_Escape){ //esc pressed
					not_break = 0; //break
					break;
				}
			}
			pthread_mutex_unlock(btm->mutex);
		}
	}
	printf("\nclosing thread ioAPI...\n");
}


int
main()
{
	btm_global.mutex = &mutex;
	queue_init(&btm_global.q, 50);

	pthread_t thread_xapi_id;
	pthread_t thread_ioapi_id;

	printf("\nCreating thread xAPI...\n");	
	pthread_create(&thread_xapi_id, NULL, xAPI, &btm_global);

	printf("\nCreating thread ioAPI..\n");
	pthread_create(&thread_ioapi_id, NULL, ioAPI, &btm_global);
	pthread_join(thread_ioapi_id, NULL);

	
	return 0;
}
