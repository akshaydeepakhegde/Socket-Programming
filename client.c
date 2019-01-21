
//CLIENT 

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<math.h>
#define AND 1
#define OR 0

#define EDGE_TCP 23970


int main(int argc, char **argv) 
{
 int sockfd,connfd, n, numbytes, sentbytes;
 struct sockaddr_in address_edge;
	
	
 //Creating the socket from Client to EDGE
 
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Error in creating the Socket");
  exit(2);
 }
	
 memset(&address_edge, 0, sizeof(address_edge));
 address_edge.sin_family = AF_INET;
 address_edge.sin_addr.s_addr= inet_addr("127.0.0.1");
 address_edge.sin_port =  htons(EDGE_TCP);

 if (connect(sockfd, (struct sockaddr *) &address_edge, sizeof(address_edge))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
printf("The client is up and running");


// Reading the file
FILE *fp = fopen("job.txt", "r");
int number1[100],number2[100];
int op[101];
int nums[500];  
    int j=0;

char *p;
int i, count, counter=0;
if(fp != NULL){
char each_line[100];
while(fgets(each_line, sizeof each_line, fp) != NULL)
{
count = 0;
p = strtok(each_line, ",");
while(p != NULL)
{
count++;
if(count == 1) {op[counter] = (strcmp(p,"and")) ? AND : OR;}
		if(count == 2) number1[counter] = atoi(p);
		if(count == 3) number2[counter] = atoi(p);
                token = strtok(NULL,",");
}
counter++;
}
fclose(fp);
} 
else {
        perror("job.txt");
    }
printf("\n"); 
for(int i = 0; i < (3*counter); i++){
    	nums[i] = op[i];

    }  
for(int i = 0; i < (3*counter); i++){
    	nums[5+i] = number1[i];
}

for(int i = 0; i < (3*counter); i++){
    	nums[10+i] = number2[i];}

nums[201] = counter-1;







//sending data to EDGE server

if ((sentbytes=send(sockfd, nums, sizeof nums, 0)) == -1){
                perror("Send:");
            
            exit(0);}

	printf("The client has successfully finished sending all %d lines to the edge server\n", counter-1);


//Receiving Result from EDGESERVER 
int result[1001];
if ((numbytes = recv(sockfd, &result, sizeof result, 0)) == -1) 
	    {
		perror("recv");
		exit(1);
	    }
printf("The client has successfully finished receiving all computation results from the edge server reduction type\n");

Printf("The final computation result are:\n");

for (int i=0;i<counter-1;i++)
{
printf("%d\n",result[i]);
}
close(sockfd);
return 0;
}

