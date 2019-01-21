#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<math.h>
#include <arpa/inet.h>
#include <unistd.h>



#define PORT_AND    22970
#define UDP_PORT_OR    21970
#define PORT_EDGE	24970
#define EDGE_TCP    23970



int main(void)
{
int nums[1001], nums1[1001],nums2[1001];
int yes =1;
int tcpfd, connfd;
int numbyte, numbytes, sentbytes;
int maxconn,c1= 0,c2 = 0;
	 
	 socklen_t addrlen;
			
	 struct sockaddr_in Clienaddr, address_edge;
	
	tcpfd = socket (AF_INET, SOCK_STREAM, 0);

        if (setsockopt(tcpfd , SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("Error: setsockopt");
            exit(1);
        }
	
	 address_edge.sin_family = AF_INET;
	 address_edge.sin_addr.s_addr = htonl(INADDR_ANY);
	 address_edge.sin_port = htons(EDGE_TCP);
	
	 bind (tcpfd, (struct sockaddr *) &address_edge, sizeof(address_edge));
	
	 maxconn = 10;
	 listen (tcpfd, maxconn);
	
	 printf("The EDGESERVER is up and running.\n");
	
	 for ( ; ; ) 
	{
		
	  addrlen = sizeof(Clienaddr);
	  connfd = accept (tcpfd, (struct sockaddr *) &Clienaddr, &addrlen);
	 	
//RECEIVING DATA FROM THE CLIENT 


  if ((numbytes = recv(connfd, &nums, sizeof nums, 0)) == -1) 
	    {
		perror("Error!!!");
		exit(1);
	    }
	


	int lc,port,size,oper;
	
	int num1[101],num2[101],op[101];
	
	lc = nums[201];
	int p = lc;
	printf("%d\n",p);



	for(int i=0; i<p; i++)
	{
	op[i] = nums[i];

	 }

	for(int i=0 ;i<(2*p); i++)
	{
	num1[i] = nums[i+p];
	}  
	
	for(int i =0;i<(3*p);i++)
	{
	num2[i] = nums[i+(2*p)];
	}

	for(int i =0;i<p;i++)
	{
	printf("%d\n",op[i]);
	}



for( int i =0;i<lc;i++){
	if(op[i] == 0){
		nums1[i] = num1[i];
		nums1[i+lc] = num2[i];
		c1++;}
	else {
		nums2[i] = num1[i];
		nums2[i+lc] = num2[i];
		c2++;}
}

for(int i =0 ;i<p;i++)
	{
	printf("%d\n",nums1[i]);
	}

for(int i =0 ;i<p;i++)
	{
	printf("%d\n",nums2[i]);
	}

nums1[101] = c1;nums2[101] = c2;nums1[102] = lc;nums2[102] = lc;

printf("%d,%d\n",nums1[101],nums2[101]);

/*for(int i =0;i<lc;i++)
	{
	printf("%d",nums[i]);
	}*/
	
	
port = ntohs(Clienaddr.sin_port);

	 printf("The EDGESERVER has received %d lines from the client using TCP over port %d \n", lc, PORT_EDGE);


// UDP SOCKET 


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
if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("Error: Setsockopt...");
            exit(1);
        }

memset(&server_AND, 0, sizeof(server_AND));
server_AND.sin_family = AF_INET;
server_AND.sin_addr.s_addr = htonl(INADDR_ANY);
server_AND.sin_port = htons(PORT_AND);

memset(&srcaddr, 0, sizeof(srcaddr));
srcaddr.sin_family = AF_INET;
srcaddr.sin_addr.s_addr = htonl(INADDR_ANY);
srcaddr.sin_port = htons(PORT_EDGE);

if (bind(fd, (struct sockaddr *) &srcaddr, sizeof(srcaddr)) < 0) {
        perror("Error: Binding...");
        exit(1);
    }

   
// SENDING DATA TO SERVER AND 



if ((numbyte=sendto(fd, nums1, sizeof(nums1), 0, (struct sockaddr *) &server_AND, sizeof(server_AND))) < 0) {
            perror("Error: Sendto...");
            exit(1);
        }

    printf("The EDGESERVER has successfully sent %d lines to Backend-Server AND \n", c1);


// SENDING DATA TO SERVER OR 
	
    if ((fd1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: Socket");
        exit(1);
    }

if (setsockopt(fd1, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("Error");
            exit(1);
        }
   
    memset(&server_OR, 0, sizeof(server_OR));
    server_OR.sin_family = AF_INET;
    server_OR.sin_addr.s_addr = htonl(INADDR_ANY);
    server_OR.sin_port = htons(PORT_OR);

if ((numbyte=sendto(fd1, nums2, sizeof(nums2), 0, (struct sockaddr *) &server_OR, sizeof(server_OR))) < 0) {
            perror("Error: Sendto...");
            exit(1);
        }

    printf("The EDGESERVER has successfully sent %d lines to Backend-Server OR \n", c2);





// RECEIVING RESULT FROM SERVER AND

 
int res1[101],cal1[101],test;   
addr_len = sizeof their_addr;
if ((numbytes = recvfrom(fd, &res1, sizeof res1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1 ) {
        perror("recvfrom");
        exit(1);
    }
printf("The edge server start receiving the computation results from Backend-Server AND Backend-Server using UDP over port %d \n", PORT_EDGE);
printf("The final computation results are:");


for (int i = 0;i<lc;i++)
{
cal1[i] = res1[i];
}


   
// RECEIVING RESULT FROM SERVER OR 

addr_len1 = sizeof their_addrB;
int res2[101],cal2[101];
if ((numbytes = recvfrom(fd, &res2, sizeof res2 , 0, (struct sockaddr *)&their_addrB, &addr_len1)) == -1 ) {
        perror("Error: recvfrom B");
        exit(1);
    }



// CALCULATING THE FINAL COMPUTATIONS 

for (int i=0;i<lc;i++)
{
cal2[i] = res2[i];
}

int result[1001];
for(int i =0;i<lc;i++){
result[i] = cal1[i] + cal2[i];
}

for (int i=0;i<lc;i++)
{
printf("%d\n",result[i]);
}






// SENDING THE FINAL COMPUTATIONS TO CLIENT 

if ((sentbytes=send(connfd, &result, sizeof result, 0)) == -1){
                perror("Error: Send to client ..");
            
            exit(0);
        }

	printf("The EDGESERVER has successfully finished sending the reduction value to client. \n");

     
 }

    return 0;
}




