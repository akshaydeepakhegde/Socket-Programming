////////////////// SERVER AND /////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<math.h>


#define PORT_EDGE    24970
#define PORT_AND    22970
#define MAXBUFLEN 1001



int main(void) {
    struct sockaddr_in address_edge, server_AND;
    int fd, numbytes;
        int total1[1001];
    socklen_t addr_len;


	char s[INET_ADDRSTRLEN];
	int yes=1;
    struct sockaddr_storage EDGE;


    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: creating socket..");
        exit(1);
    }


        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("Error: Setsockopt..");
            exit(1);
        }


    memset(&address_edge, 0, sizeof(address_edge));
    address_edge.sin_family = AF_INET;
    address_edge.sin_addr.s_addr = htonl(INADDR_ANY);
    address_edge.sin_port = htons(PORT_EDGE);

    memset(&server_AND, 0, sizeof(server_AND));
    server_AND.sin_family = AF_INET;
    server_AND.sin_addr.s_addr = htonl(INADDR_ANY);
    server_AND.sin_port = htons(PORT_AND);

    if (bind(fd, (struct sockaddr *) &server_AND, sizeof(server_AND)) < 0) {
        perror("Error: Bind...");
        exit(1);
    }

	 printf("The Server AND is up and running using UDP port %d \n", PORT_AND);
       
	addr_len = sizeof EDGE;


//RECEIVING DATA FRON EDGE SERVER 

    if ((numbytes = recvfrom(fd, total1, sizeof total1 , 0, (struct sockaddr *)&EDGE, &addr_len)) == -1 ) {
        perror("Error: Receive from....");
        exit(1);
    }

int num1[101],num2[101],res1[101];

int counter1 = total1[101];
int k = total1[102];
	
	 printf("The Server AND has received %d lines \n", counter1);

for( int i =0; i<k;i++)
{
num1[i] = total1[i];
}

for( int i =0; i<k;i++)
{

num2[i+k] = total1[i+k];
}



int dec1[101],dec2[101],base1= 1, rem1[101],rem2[101],res[101],base2=1,rs1[101];

for (int i=0;i<k;i++)
{
dec1[i]=0;
dec2[i]=0;
}
  for (int i=0;i<k;i++)
{
 while (num1[i] >0)
    {
        rem1[i]= num1[i] % 10;
        dec1[i] = dec1[i] + (rem1[i] * base1);
        num1[i]= num1[i] / 10;
        base1 = base1 * 2;
    }
rem1[i]=0;
base1=1;
}
for (int i=0;i<k;i++)
{
 while (num2[i+k] >0)
    {
        rem2[i]= num2[i+k] % 10;
        dec2[i] = dec2[i] + (rem2[i] * base2);
        num2[i+5]= num2[i+k] / 10;
        base2 = base2 * 2;
    }
rem2[i]=0;
base2=1;
}
for (int i=0;i<k;i++)
{
res[i]=dec1[i]&dec2[i];
rs1[i]=res[i];
}
    int remainder[1001],x[1001];
 for (int i=0;i<12;i++)
{
x[i]=0;
}
 int j = 1;
 for (int i=0;i<k;i++)
{
    while(rs1[i]> 0) {
        remainder[i] =rs1[i]%2;
	  x[i]= x[i] + (remainder[i]*j);
        rs1[i] = rs1[i]/2;
        j = j*10;
    }
j=1;
}


for (int i=0;i<k;i++)
{
result1[i] = x[i];
}

for (int i=0;i<k;i++)
{
printf("%d\n",result1[i]);
}


	 

// SENDING DATA TO EDGE SERVER 


 if (sendto(fd, &res1, sizeof(res1), 0, (struct sockaddr *) &address_edge, sizeof(address_edge)) < 0) {
            perror("Error: Sending to EDGE server..");
            exit(1);
        }
    printf("The Server AND has successfully finished sending the reduction value to AWS server \n");


    return 0;
}


