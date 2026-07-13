#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
/*
// Structs 
struct in_addr
{
  uint32_t s_addr; //IPv4 in big-endian
};
struct sockaddr_in
{
  uint16_t sin_family;      //AF_INET
  uint16_t sin_port;       // port in big-endian
  struct in_addr sin_addr;// IPv4
};
*/
// Function prototypes
static void msg(const char *msg);
static void die(const char *msg);
static void do_something(int connfd);
int main()
{
  //Creating a file descriptor for a network socket that uses IPv4
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  
  //Enabling a socket option that allows it to bind to a local address that is already in use
  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,  &val, sizeof(val));
  
  // Binding to the wildcard address 0.0.0.0:1234
  struct sockaddr_in addr = {}; // sockaddr_in holds an IPv4:Port pair as big-endian numbers
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1234);    //port
  addr.sin_addr.s_addr = htons(0); //wildcard IP 0.0.0.0
  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv)
  {
    die("bind()");
    }
    
    // Listen 
    rv =  listen(fd, SOMAXCONN);
    if (rv)
      die("listen()");
      
    while (true)
    {
      // Accept
      struct sockaddr_in client_addr = {};
      socklen_t addr_len = sizeof(client_addr);
      int connfd = accept(fd, (struct sockaddr *)&client_addr, &addr_len);
      if (connfd < 0)
        continue; // Error
    
    do_something(connfd);
    close(connfd);
  }

  
  
  
  return 0;
}

static void do_something(int connfd) 
{
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if (n < 0)
  {
    msg("read() error");
    return;
  }
  printf("client says: %s\n", rbuf);
  char wbuf[] = "world";
  write(connfd, wbuf, strlen(wbuf));
}
static void msg(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg)
{
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}
