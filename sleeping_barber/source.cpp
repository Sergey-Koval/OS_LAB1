// Copyright 2019 koval
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

void *client(void *param);
void *barber(void *param);

sem_t sem_chairs;
sem_t sem_client;
sem_t sem_barber;
int chairs = 5;
int clientWait = 2;


int main(int argc, char *argv[]) {
	pthread_t barberid;
	pthread_t clientid;
   sem_init(&sem_chairs,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
   pthread_create(&barberid, NULL, barber, NULL);
   std::cout << "Creating barber thread with id: " << barberid << std::endl;
   while(1){
	   pthread_create(&clientid, NULL, client, NULL);
	   sleep(rand()%8 + 1);
   }
   return 0;
}

void *barber(void *param) {
   int worktime;

   while(1) {
	  sem_wait(&sem_client);
	  sem_wait(&sem_chairs);
	  chairs += 1;
	  sem_post(&sem_chairs);
	  std::cout << "\e[01;34m";
	  std::cout << "Barber: started cutting..." << std::endl << "Chairs availiable: " << chairs << std::endl;
	  sem_post(&sem_barber);
	  worktime = rand() % 5 + 3;
	  std::cout << "\e[01;34m";
	  std::cout << "Barber: working for " << worktime << " seconds" << std::endl;
	  sleep(worktime); // 3 - 7 sec
    } 
}

void *client(void *param) {
   int waittime;
   while(1) {
	  sem_wait(&sem_chairs);
	  if(chairs <= 0){
		   sem_post(&sem_chairs);
		   std::cout << "\e[01;32m";
		   std::cout << "Client: leaving" << std::endl;
		   return param;
	  }
	  else{
		   chairs -= 1;
		   sem_post(&sem_chairs);
		   std::cout << "\e[01;32m";
		   std::cout << "Client: " << (unsigned int)pthread_self() << " waiting... chairs: " << chairs << std::endl;
		   sem_post(&sem_client);
		   sem_wait(&sem_barber);
		   std::cout << "\e[01;32m";
		   std::cout << "Client: " << (unsigned int)pthread_self() << " getting a haircut" << std::endl;
		   return param;
	  }
	  waittime = (rand() % clientWait) + 1;
	  std::cout << "Client: " << (unsigned int)pthread_self() << " waiting for " << waittime << " seconds" << std::endl;
	  std::cout << "\e[01;32m";
	  sleep(waittime);
     }
}
