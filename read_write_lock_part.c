#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

struct node {
    int data;
    struct node* next;
};

//const int MAX_THREADS = 1024;
long thread_count=0;
pthread_rwlock_t rwlock;
struct node* head = NULL;

int n,m;
float m_percentage_of_Member, m_percentage_of_Insert, m_percentage_of_Delete;

int Member(int value, struct node* head){
    struct node* current = head;
    while(current != NULL && current-> data < value)
        current = current-> next;
    
    if (current == NULL || current-> data > value) {
        return 0;
    } else {
        return 1;
    }
 
}

int Insert(int value, struct node** head){
    struct node* current = *head;
    struct node* pred = NULL;
    struct node* temp;
    while(current != NULL && current->data < value){
        pred = current;
        current = current-> next;
    }
    if(current == NULL || current->data > value){
        temp = malloc(sizeof(struct node));
        temp-> data = value;
        temp-> next = current;
        if(pred == NULL)
            *head = temp;
        else
            pred-> next = temp;
        return 1;
    } else {
        return 0;
    }
}

int Delete (int value, struct node** head) {
	struct node* current = *head;
	struct node* pred = NULL;
	while (current != NULL && current->data<value) {
		pred = current;
		current = current->next;	
	}
	if (current != NULL && current->data == value){
		if(pred == NULL){ 
			*head = current->next;
			free(current);
		}else{
			pred->next = current->next;
			free(current);
		}
		return 1;
	}else {
		return 0;
	}
} 

void GetArguments(int argc, char* argv[]) {
    
    thread_count = strtol(argv[1], NULL, 10);
    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);
     
    m_percentage_of_Member = (float) atof(argv[4]);
    m_percentage_of_Insert = (float) atof(argv[5]);
    m_percentage_of_Delete = (float) atof(argv[6]);
} 

void* pthread_on_linked_list(void* rank){
    long long i;
    long long my_m = m/thread_count;

    for( i=0; i < my_m; i++ ){
 
        float prob = (rand()%10000/10000.0);
        
     
        int r = rand()%65536;

        if(prob<m_percentage_of_Member){
            pthread_rwlock_rdlock(&rwlock);
            Member(r,head);
            //number_of_member++;
            pthread_rwlock_unlock(&rwlock);

        }else if(prob < m_percentage_of_Member + m_percentage_of_Insert ){
            pthread_rwlock_wrlock(&rwlock);
            Insert(r,&head);
            //number_of_insert++;
            pthread_rwlock_unlock(&rwlock);
        
        }else{           
            pthread_rwlock_wrlock(&rwlock);
            Delete(r,&head);
            //number_of_delete++;
            pthread_rwlock_unlock(&rwlock);
        }   
    }  
 
   return NULL;
} 

double CalculateTime(struct timeval start, struct timeval end) {
    return (double) (end.tv_usec - start.tv_usec) / 1000000.0 + (double) (end.tv_sec - start.tv_sec);
}

float linkedList_rw_program(int argc, char* argv[]){
	int i=0;
	long thread; 
    pthread_t* thread_handles;
	struct timeval time_begin, time_end;
	GetArguments(argc, argv);

	for(;i<n;i++){   
        int r = rand()%65536;
		if(!Insert(r,&head)){
		    i--;
		}
    	}
	
	thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));	

	gettimeofday(&time_begin, NULL);
    pthread_rwlock_init(&rwlock, NULL);
 
    for (thread = 0; thread < thread_count; thread++){
		pthread_create(&thread_handles[thread], NULL,pthread_on_linked_list, (void*)thread);  
    }
     
    for (thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handles[thread], NULL); 
    }
     
    pthread_rwlock_destroy(&rwlock);
	gettimeofday(&time_end, NULL);

	return CalculateTime(time_begin, time_end);
	
}

int main(int argc, char* argv[]){
	
	for (int j = 0; j< 10 ; j++){
		float xi = linkedList_rw_program(argc,argv);
		printf("%.6f  \n",xi);
	}
    return 0;
	
}