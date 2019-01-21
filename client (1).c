
//////////////// CLIENT ////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<math.h>
#define AND 1
#define OR 0

#define TCP_EDGE_PORT 23302


int main(int argc, char **argv) 
{
 int sockfd,connfd, n, numbytes, sentbytes;
 struct sockaddr_in EDGEaddr;
	
 /////basic check of the arguments
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server"); 
  exit(1);
}
	
 ////Creating the socket for the client & connecting it to EDGE/////
 
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }
	
 memset(&EDGEaddr, 0, sizeof(EDGEaddr));
 EDGEaddr.sin_family = AF_INET;
 EDGEaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
 EDGEaddr.sin_port =  htons(TCP_EDGE_PORT);

 if (connect(sockfd, (struct sockaddr *) &EDGEaddr, sizeof(EDGEaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }

////////////// Reading the file ////////////////////////////////////////

   FILE *fp = fopen("job.txt", "r");
    int num1[100],num2[100],operator[100],total[500];  
    int j;

    char *token;
    int i, count, linecount=0;
    if(fp != NULL)
    {
        char line[100];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            count = 0;
            token = strtok(line, ",");
	    while(token != NULL)
	    {
	        count++;
		if(count == 1) {operator[linecount] = (strcmp(token,"and")) ? AND : OR;}
		if(count == 2) num1[linecount] = atoi(token);
		if(count == 3) num2[linecount] = atoi(token);
                token = strtok(NULL,",");
	    }
            linecount++;
        }
        fclose(fp);
    } else {
        perror("job.txt");
    }
    printf("\n"); 
    for(i = 0; i < linecount-1; i++){
    	printf("%d : %d : %d\n",operator[i],num1[i], num2[i]);
    }  

 for(int i = 0; i < (3*linecount); i++){
    	total[i] = operator[i];

    }  
for(int i = 0; i < (3*linecount); i++){
    	total[5+i] = num1[i];
}

for(int i = 0; i < (3*linecount); i++){
    	total[10+i] = num2[i];}


 for(int i = 0; i < (3*linecount); i++){
    printf("%d",total[i]);
    }  

total[201] = linecount-1;







///////////////////sending data to EDGESERVER /////////////////


	 if ((sentbytes=send(sockfd, total, sizeof total, 0)) == -1){
                perror("Send:");
            
            exit(0);
        }

	printf("The client has sent %d lines to EDGESERVER\n", linecount-1);


//////////////////Receiving Result from EDGESERVER ///////////////////////////////////////////////

int result[1001];
    if ((numbytes = recv(sockfd, &result, sizeof result, 0)) == -1) 
	    {
		perror("recv");
		exit(1);
	    }

printf("The EDGESERVER has received reduction type\n");


for (int i=0;i<linecount-1;i++)
{
printf("%d\n",result[i]);
}


///////////close socket///////////////////


close(sockfd);

   return 0;
}

