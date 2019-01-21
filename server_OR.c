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


#define PORT_EDGE    24302
#define PORT_OR   21302
#define MAXBUFLEN 1001




int main(void) {
    struct sockaddr_in address_edge, server_OR;
    int fd, numbytes;
        int total2[1001],num1[101],num2[101],counter2 = 0,res2[101];
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

    memset(&server_OR, 0, sizeof(server_OR));
    server_OR.sin_family = AF_INET;
    server_OR.sin_addr.s_addr = htonl(INADDR_ANY);
    server_OR.sin_port = htons(PORT_OR);

    if (bind(fd, (struct sockaddr *) &server_OR, sizeof(server_OR)) < 0) {
        perror("Error: Bind...");
        exit(1);
    }

	 printf("The Server OR is up and running using UDP port %d \n", PORT_OR);
       
	addr_len = sizeof EDGE;

// RECEIVING DATA FRON EDGE SERVER 

   if ((numbytes = recvfrom(fd, total2, sizeof total2 , 0, (struct sockaddr *)&EDGE, &addr_len)) == -1){
        perror("Error: Receive from....");
        exit(1);
    }

counter2 = total2[101]; int k = total2[102];
printf("The Server OR has received %d lines \n", counter2);

for( int i =0; i<k;i++)
{
num1[i] = total2[i];
}

for( int i =0; i<k;i++)
{

num2[i+k] = total2[i+k];
}



int dec1[101],dec2[101],base1= 1, rem1[101],rem2[101],res[1001],base2=1,rs1[1001];

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
        num2[i+k]= num2[i+k] / 10;
        base2 = base2 * 2;
    }
rem2[i]=0;
base2=1;
}
for (int i=0;i<k;i++)
{
res[i]=dec1[i]|dec2[i];
rs1[i]=res[i];
}
    int remainder[101], x[101];
 for (int i=0;i<k;i++)
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


for (int i = 0;i<k;i++)
{
res2[i] = x[i];
}

for (int i=0;i<k;i++)
{
printf("%d\n",res2[i]);
}



	
	 printf("The Server OR has received %d numbers \n", counter2);
	
// SENDING DATA TO EDGE SERVER 
 if (sendto(fd, &res2, sizeof(res2), 0, (struct sockaddr *) &address_edge, sizeof(address_edge)) < 0) {
            perror("Error: Sending to aws server..");
            exit(1);
        }
    printf("The Server OR has seccessfully finished sending the reduction value to EDGE server \n");


    return 0;
}



