#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
using namespace std;
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
int main()
{
  //Creating a file descriptor for a network socket that uses IPv4
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  
  //Enabling a socket option that allows it to bind to a local address that is already in use
  int val = 1;
  setsockopt(fd, SOL_Socket, SO_REUSEDADDR,  &val, sizeof(val));
  
  // Binding to the wildcard address 0.0.0.0:1234
  struct sockaddr_in addr = {}; // sockaddr_in holds an IPv4:Port pair as big-endian numbers
  addr.sin_family = AF_NET;
  addr.sin_port = htons(1234)     //port
  addr.sin_addr.s_addr = hton(0); //wildcard IP 0.0.0.0
  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv)
    die("bind()");
    
    // Listen 
    rv = listen(fd, SOMAXCONN)
  
  
  
  return 0;
}

