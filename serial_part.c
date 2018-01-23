#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include<stdbool.h>
#include <sys/time.h>
#include<math.h>

struct node {
    int data;
    struct node* next;
};

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
   
    n = (int) strtol(argv[1], (char **)NULL, 10);
    m = (int) strtol(argv[2], (char **)NULL, 10);
     
    m_percentage_of_Member = (float) atof(argv[3]);
    m_percentage_of_Insert = (float) atof(argv[4]);
    m_percentage_of_Delete = (float) atof(argv[5]);
} 
    
double CalculateTime(struct timeval start, struct timeval end) {
    return (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec);
}

float linked_list_serial_program(int argc, char* argv[]){

	struct node* head = NULL; 
	struct timeval start, end;
	int i;
	
	GetArguments(argc, argv);

	for(;i<n;i++){   
        int r = rand()%65536;
		if(!Insert(r,&head)){
		    i--;
		}
    }

	gettimeofday(&start, NULL);
	for(i=0;i<m;i++){
        	float temp = (rand()%10000/10000.0);           
        	int r = rand()%65536;

		if(temp<m_percentage_of_Member){
		    Member(r,head);
		}else if(temp < m_percentage_of_Member + m_percentage_of_Insert){
		    Insert(r,&head);
		}else{           
		    Delete(r,&head);
        } 
	}
    
	gettimeofday(&end, NULL);
	
	return CalculateTime(start, end);
}

void  Display(struct node** head_pp)
{
    struct node* r=*head_pp;
    if(r==NULL)
    {
    	return;
    }
    while(r!=NULL)
    {
   	printf("%d ",r->data);
    	r=r->next;
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    int samples = 100; 
    float sum_of_time = 0; 
    float sum_of_time_square = 0;
	for (int j = 0; j< samples ; j++){
		float time = linked_list_serial_program(argc,argv);
		sum_of_time = sum_of_time + time;
		sum_of_time_square = sum_of_time_square + time*time ;
	}
	
	float mean = sum_of_time /samples ; 
	float sdtemp = (sum_of_time_square/samples) - mean*mean ; 
	float sd = sqrt(sdtemp);
        printf("Serial Linked List Time Spent : \n mean: %.6f secs\n sd: %.6f secs \n", mean,sd);
    
    /*struct node* head = NULL; 
    for(int i;i<4;i++){ 
        Insert(i,&head);
    }
    Display(&head);
    Delete(1,&head);
    Display(&head);*/
    return 0;
}