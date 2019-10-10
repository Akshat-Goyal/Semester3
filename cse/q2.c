#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Chef{
	int id, vessels, p, *studentsToBeServed, isready;                 // Each vessel has the capacity to feed p students
	pthread_t tid;
	pthread_mutex_t lock;
}Chef;

typedef struct Table{
	int id, slots, p, *studentsToBeServed, M;
	Chef *chefs; 
	pthread_t tid;
	pthread_mutex_t lock;
}Table;

typedef struct Student{
	int id, tableid, slot, *studentsToBeServed, N;
	Table *tables;
	pthread_t tid;
}Student;


int biryani_ready(Chef *chef){
	pthread_mutex_t *lock = &(chef->lock);
	pthread_mutex_lock(lock);
	chef->isready = 1;
	printf("Chef %d is ready to serve %d vessels of capacity %d\n", chef->id+1, chef->vessels, chef->p);
	pthread_mutex_unlock(lock);
	while(*(chef->studentsToBeServed) && chef->vessels);
	pthread_mutex_lock(lock);
	chef->isready = 0;
	pthread_mutex_unlock(lock);
	return *(chef->studentsToBeServed);
}

void* cook_biryani(void* arg){
	int status = 1;
	Chef *chef = (Chef*) arg;
	pthread_mutex_t * lock = &(chef->lock);
	while(status){
		int w = rand()%4 + 2;
		pthread_mutex_lock(lock);
		chef->vessels = rand()%10 + 1;
		chef->p = rand()%26 + 25;
		pthread_mutex_unlock(lock);
		sleep(w);
		status = biryani_ready(chef);
	}
}

int load_biryani(Table *table){
	int status = 1;
	pthread_mutex_t *lock1, *lock2;
	while(status && *(table->studentsToBeServed)){
		for(int i=0; i<table->M; i++){
			if(table->chefs[i].isready){
				lock1 = &(table->lock);
				lock2 = &(table->chefs[i].lock);
				pthread_mutex_lock(lock2);
				pthread_mutex_lock(lock1);
				table->p = table->chefs[i].p;
				table->chefs[i].vessels--;
				pthread_mutex_unlock(lock1);
				pthread_mutex_unlock(lock2);
				status = 0;
				break;
			}
		}
	}
	return *(table->studentsToBeServed);
}

int ready_to_serve_table(Table *table){
	while(*(table->studentsToBeServed) && table->slots);
	return *(table->studentsToBeServed);
}

void* serve_biryani(void* arg){
	int status = 1;
	Table *table = (Table*) arg;
	pthread_mutex_t * lock = &(table->lock);
	while(status){
		status = load_biryani(table);
		printf("Serving Table %d is ready to serve biryani of capacity %d\n", table->id+1, table->p);
		while(status && table->p){
			pthread_mutex_lock(lock);
			table->slots = rand()%10%table->p + 1;
			table->p -= table->slots;
			printf("Serving Table %d has %d slots\n", table->id, table->slots);
			pthread_mutex_unlock(lock);
			status = ready_to_serve_table(table);
		}
	}
}

void student_in_slot(Student* student){
	printf("Student %d is allocated %dth slot of Serving Table %d\n", student->id+1, student->slot, student->tableid+1);
	*(student->studentsToBeServed) -= 1;
}

void *wait_for_slot(void* arg){
	int arrivalTime = rand()%10;
	sleep(arrivalTime);
	int status = 1;
	Student *student = (Student*) arg;
	pthread_mutex_t *lock;
	while(status){
		for(int i=0; i<student->N; i++){
			if(student->tables[i].slots != 0){
				lock = &(student->tables[i].lock);
				pthread_mutex_lock(lock);
				student->tableid = student->tables[i].id;
				student->slot = student->tables[i].slots;
				student->tables[i].slots--;
				pthread_mutex_unlock(lock);
				status = 0;
				break;
			}
		}
	}
	student_in_slot(student);
}

int main(){

	int M, N, K;
	printf("Enter the number of Robot Chefs\n");
	scanf("%d", &M);
	printf("Enter the number of Serving Tables\n");
	scanf("%d", &N);
	printf("Enter the number of Registered Students\n");
	scanf("%d", &K);

	int studentsToBeServed = K;
	Chef *chefs = (Chef*) malloc(sizeof(Chef)*M);
	for(int i=0, status; i<M; i++){
		chefs[i].id = i;
		chefs[i].isready = 0;
		chefs[i].studentsToBeServed = &studentsToBeServed;
		pthread_mutex_init(&(chefs[i].lock), NULL);
		status = pthread_create(&(chefs[i].tid), NULL, cook_biryani, &chefs[i]);
		if(status){
			perror("Error in creating Chef thread");
		}
	}

	Table *tables = (Table*) malloc(sizeof(Table)*N);
	for(int i=0, status; i<N; i++){
		tables[i].id = i;
		tables[i].slots = 0;
		tables[i].chefs = chefs;
		tables[i].M = M;
		tables[i].studentsToBeServed = &studentsToBeServed;
		pthread_mutex_init(&(tables[i].lock), NULL);
		status = pthread_create(&(tables[i].tid), NULL, serve_biryani, &tables[i]);
		if(status){
			perror("Error in creating Serving table thread");
		}
	}

	Student *students = (Student*) malloc(sizeof(Student)*K);
	for(int i=0, status; i<K; i++){
		students[i].id = i;
		students[i].tableid = -1;
		students[i].tables = tables;
		students[i].N = N;
		students[i].studentsToBeServed = &studentsToBeServed;
		status = pthread_create(&(students[i].tid), NULL, wait_for_slot, &students[i]);
		if(status){
			perror("Error in creating Student thread");
		}
	}

	for(int i=0; i<M; i++){
		pthread_join(chefs[i].tid, NULL);
	}
	for(int i=0; i<N; i++){
		pthread_join(tables[i].tid, NULL);
	}
	for(int i=0; i<K; i++){
		pthread_join(students[i].tid, NULL);
	}
	free(chefs);
	free(tables);
	free(students);
	printf("All the students are served biryani\n");

	return 0;
}