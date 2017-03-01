#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define PORTA 8888
#define PORTASEDE 7777
#define PORTA_CLIENT_SEDE 7548





ssize_t FullWrite(int fd, const void *buf, size_t count);
ssize_t FullRead(int fd, void *buf, size_t count);

int Socket(int family, int type, int prot)
{
    int n;
    if((n=socket(family, type, prot))<0)
    {
        printf("socket error \n");
        exit(1);
    }
    return(n);
}

int Connect(int sock, const struct sockaddr* sadd, int size)
{
    if(connect(sock, sadd, size)<0)
    {
        printf("connect error %d\n", errno);
        exit (1);
    }
}

int Bind(int listfd, const struct sockaddr* sadd, int size)
{
    int n;
    if((n=bind(listfd, sadd, size))<0)
    {
        printf("bind error %d \n", errno);
        exit(1);
    }
    return(n);
}

int Listen(int listfd, int size)
{
    if(listen(listfd, size)<0)
    {
        printf("listen error \n");
        exit(1);
    }
}

int Accept(int listfd, struct sockaddr* sadd, int *var)
{
    int n;
    if((n=accept(listfd, sadd, var))<0)
    {
        printf("accept error %d \n", errno);
        exit(1);
    }
    return(n);
}


ssize_t FullWrite(int fd, const void *buf, size_t count)
{
    size_t nleft;
    ssize_t nwritten;
    nleft = count;
    while (nleft > 0)               /* repeat until no left */
    {
        if ( (nwritten = write(fd, buf, nleft)) < 0)
        {
            if (errno == EINTR)     /* if interrupted by system call */
            {
                continue;           /* repeat the loop */
            }
            else
            {
                return(nwritten);   /* otherwise exit with error */
            }
        }
        nleft -= nwritten;          /* set left to write */
        buf +=nwritten;             /* set pointer */
    }
    return (nleft);
}

ssize_t FullRead(int fd, void *buf, size_t count)
{
    size_t nleft;
    ssize_t nread;
    nleft = count;
    while (nleft > 0)               /* repeat until no left */
    {
        if ( (nread = read(fd, buf, nleft)) < 0)
        {
            if (errno == EINTR)     /* if interrupted by system call */
            {
                continue;           /* repeat the loop */
            }
            else
            {
                return(nread);      /* otherwise exit */
            }
        }
        else if (nread == 0)        /* EOF */
        {
            break;                  /* break loop here */
        }
        nleft -= nread;             /* set left to read */
        buf +=nread;                /* set pointer */
    }
    return (nleft);
}

int pagamento(int soldi, int costo)
{
    int resto;
    
    if(soldi >= costo)
    {
        resto = soldi - costo;
        printf("Resto : %d \n", resto);
    }
	else
	{
		printf("Non hai pagato\n");
		exit(0);
	}
}
