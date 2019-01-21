#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<math.h>
#include <arpa/inet.h>
#include <unistd.h>



#define UDP_PORT_AND    22302
#define UDP_PORT_OR    21302
#define UDP_EDGE_PORT    24302
#define TCP_EDGE_PORT    23302



int main(void)
{
////////////////////// TCP SOCKET //////////////////////////
////////////////////// TCP SOCKET //////////////////////////
	int total[1001], total1[1001],total2[1001];
	int yes =1;
	int tcpfd, connfd, numbyte, numbytes, sentbytes, maxconn,counter1 = 0,counter2 = 0;
	 
	 socklen_t addrlen;
			
	 struct sockaddr_in Clienaddr, EDGEaddr;
	
	tcpfd = socket (AF_INET, SOCK_STREAM, 0);

        if (setsockopt(tcpfd , SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("Error: setsockopt");
            exit(1);
        }
	
	 EDGEaddr.sin_family = AF_INET;
	 EDGEaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 EDGEaddr.sin_port = htons(TCP_EDGE_PORT);
	
	 bind (tcpfd, (struct sockaddr *) &EDGEaddr, sizeof(EDGEaddr));
	
	 maxconn = 10;
	 listen (tcpfd, maxconn);
	
	 printf("The EDGESERVER is up and running.\n");
	
	 for ( ; ; ) 
	{
		
	  addrlen = sizeof(Clienaddr);
	  connfd = accept (tcpfd, (struct sockaddr *) &Clienaddr, &addrlen);
	 	
///////////////////////// RECEIVING DATA FROM THE CLIENT ////////////////////////////////////


  if ((numbytes = recv(connfd, &total, sizeof total, 0)) == -1) 
	    {
		perror("Error: numbers are not received from client");
		exit(1);
	    }
	


	int linecount,port,size,oper;
	
	int num1[101],num2[101],operator[101];
	
	linecount = total[201];
	int p = linecount;
	printf("%d\n",p);



	for(int i=0; i<p; i++)
	{
	operator[i] = total[i];

	 }

	for(int i=0 ;i<(2*p); i++)
	{
	num1[i] = total[i+p];
	}  
	
	for(int i =0;i<(3*p);i++)
	{
	num2[i] = total[i+(2*p)];
	}

	for(int i =0;i<p;i++)
	{
	printf("%d\n",operator[i]);
	}



for( int i =0;i<linecount;i++){
	if(operator[i] == 0){
		total1[i] = num1[i];
		total1[i+linecount] = num2[i];
		counter1++;}
	else {
		total2[i] = num1[i];
		total2[i+linecount] = num2[i];
		counter2++;}
}

for(int i =0 ;i<p;i++)
	{
	printf("%d\n",total1[i]);
	}

for(int i =0 ;i<p;i++)
	{
	printf("%d\n",total2[i]);
	}

total1[101] = counter1;
total2[101] = counter2;
total1[102] = linecount;
total2[102] = linecount;
printf("%d,%d\n",total1[101],total2[101]);

/*for(int i =0;i<linecount;i++)
	{
	printf("%d",total[i]);
	}*/
	
	
port = ntohs(Clienaddr.sin_port);
int port1 = 23302;
	 printf("The EDGESERVER has received %d lines from the client using TCP over port %d \n", linecount, port1);


////////////////////// UDP SOCKET //////////////////////////
////////////////////// UDP SOCKET //////////////////////////
    struct sockaddr_in server_AND, server_OR,srcaddr;
    int fd, fd1;
    int numbytes;
    socklen_t addr_len, addr_len1, addr_len2;
    int yes=1;
    struct sockaddr_storage their_addr, their_addrB;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Error: Socket...");
        exit(1);
    }
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
	{
            perror("Error: Setsockopt...");
            exit(1);
        }

    memset(&server_AND, 0, sizeof(server_AND));
    server_AND.sin_family = AF_INET;
    server_AND.sin_addr.s_addr = htonl(INADDR_ANY);
    server_AND.sin_port = htons(UDP_PORT_AND);

    memset(&srcaddr, 0, sizeof(srcaddr));
    srcaddr.sin_family = AF_INET;
    srcaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srcaddr.sin_port = htons(UDP_EDGE_PORT);

    if (bind(fd, (struct sockaddr *) &srcaddr, sizeof(srcaddr)) < 0) {
        perror("Error: Binding...");
        exit(1);
    }

   
///////////////////////// SENDING DATA TO SERVER AND ////////////////////////////////////

        if ((numbyte=sendto(fd, total1, sizeof(total1), 0, (struct sockaddr *) &server_AND, sizeof(server_AND))) < 0) {
            perror("Error: Sendto...");
            exit(1);
        }

    printf("The EDGESERVER has sent %d numbers to Backend-Server AND \n", counter1);


///////////////////////// SENDING DATA TO SERVER OR ////////////////////////////////////
	
    if ((fd1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: Socket");
        exit(1);
    }

    if (setsockopt(fd1, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("Error: Setsockopt");
            exit(1);
        }
   
    memset(&server_OR, 0, sizeof(server_OR));
    server_OR.sin_family = AF_INET;
    server_OR.sin_addr.s_addr = htonl(INADDR_ANY);
    server_OR.sin_port = htons(UDP_PORT_OR);


        if ((numbyte=sendto(fd1, total2, sizeof(total2), 0, (struct sockaddr *) &server_OR, sizeof(server_OR))) < 0) {
            perror("Error: Sendto...");
            exit(1);
        }

    printf("The EDGESERVER has sent %d numbers to Backend-Server OR \n", counter2);





///////////////////////// RECEIVING RESULT FROM SERVER AND ////////////////////////////////////
int res1[101],cal1[101],test;   
addr_len = sizeof their_addr;
if ((numbytes = recvfrom(fd, &res1, sizeof res1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1 ) {
        perror("recvfrom");
        exit(1);
    }
printf("The EDGESERVER received reduction result of from Backend-Server AND  using UDP over port %d and it is \n", UDP_EDGE_PORT);


for (int i = 0;i<linecount;i++)
{
cal1[i] = res1[i];
}

/*for (int i=0;i<5;i++)
{
printf("%d\n",cal1[i]);
}*/

   
///////////////////////// RECEIVING RESULT FROM SERVER OR ////////////////////////////////////

addr_len1 = sizeof their_addrB;
int res2[101],cal2[101];
  if ((numbytes = recvfrom(fd, &res2, sizeof res2 , 0, (struct sockaddr *)&their_addrB, &addr_len1)) == -1 ) {
        perror("Error: recvfrom B");
        exit(1);
    }
printf("The EDGESERVER received reduction result of from Backend-Server B  using UDP over port %d and it is  \n",UDP_EDGE_PORT);


//////////////// CALCULATING THE FINAL ANSWER //////////////////////////

for (int i=0;i<linecount;i++)
{
cal2[i] = res2[i];
}

int result[1001];
for(int i =0;i<linecount;i++){
result[i] = cal1[i] + cal2[i];
}

for (int i=0;i<linecount;i++)
{
printf("%d\n",result[i]);
}






///////////////// SENDING THE FINAL RESULT TO CLIENT /////////////////////////////////////////////////////
if ((sentbytes=send(connfd, &result, sizeof result, 0)) == -1){
                perror("Error: Send to client ..");
            
            exit(0);
        }

	printf("The EDGESERVER has successfully finished sending the reduction value to client. \n");

     
 }

    return 0;
}

//+++++++++++++++++++++++++++
//+++++++++++++++++++++++++++

////////////////////////////////////////////////
////////////////THE END/////////////////////////
////////////////////////////////////////////////




