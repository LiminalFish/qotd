#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// UDP or TCP
#define PORT 17

// max message size 512 octets
#define MSG_SIZE 512

#define WISDOM_FILE "wisdom.txt"

// TODO:
// - use the current date to grab a random entry from wisdom file
// - try an break some of this code up into functions so it is more declarative

struct sockaddr_in myaddr;

int main(int argc, char **argv) {
  char *wisdom = "once a fish, always a fish\n<><\n";

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int inbound;

  if (sock == -1) {
    fprintf(stderr, "Something is fucky with the socket\n");
    exit(1);
  }

  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(PORT);
  myaddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

  int opt = 1;

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) == -1) {
    fprintf(stderr, "I don't even fucking know\n");
    exit(1);
  }

  if (bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
    fprintf(stderr, "Oh shittings! I have not been bound :\(\n");
    exit(1);
  }

  if (listen(sock, 10) == -1) {
    fprintf(stderr, "Linda didn't listen\n");
    exit(1);
  }

  printf("WE LISTENEN\n");

  socklen_t addr_size = sizeof(myaddr);

  while (1) {
    if ((inbound = accept(sock, (struct sockaddr *)&myaddr, &addr_size)) ==
        -1) {
      fprintf(stderr, "They will never accept me for who I am\n");
      perror("accept");
      continue;
    }

    send(inbound, wisdom, strlen(wisdom), 0);
    close(inbound);
  }

  close(sock);
  close(inbound);
}
