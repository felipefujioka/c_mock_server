#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2048

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  int i;

  char fix_logon_answer[] = "8=FIX.4.2""\x1""9=79""\x1""35=A""\x1""34=1""\x1""49=ACCEPTOR""\x1""52=20170213-20:56:07.810""\x1""56=INITIATOR""\x1""98=0""\x1""108=600""\x1""141=Y""\x1""10=101""\x1";

  char fix_subscription_answer[] = "8=FIX.4.2""\x1""9=87""\x1""35=W""\x1""34=2""\x1""49=ACCEPTOR""\x1""52=20170213-20:56:07.868""\x1""56=INITIATOR""\x1""22=8""\x1""48=""\x1""55=""\x1""207=BVMF""\x1""262=""\x1""10=021""\x1";

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("ERROR opening socket");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("ERROR on binding");
  }

  printf("Server listening on port %d\n", portno);

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) {
    error("ERROR on accept");
  }

  bzero(buffer, BUFFER_SIZE);
  n = read(newsockfd, buffer, BUFFER_SIZE);
  if (n < 0) {
    error("ERROR reading from socket");
  }

  printf("Received: %s\n", buffer);
  printf("Sending back: %s\n", fix_logon_answer);

  n = write(newsockfd, fix_logon_answer, strlen(fix_logon_answer));
  if (n < 0) {
    error("ERROR writing to socket");
  }
  /* Established FIX session, messages will now be exchanged via newsockfd */

  bzero(buffer, BUFFER_SIZE);
  n = read(newsockfd, buffer, BUFFER_SIZE);
  if (n < 0) {
    error("ERROR reading from socket");
  }

  printf("Received: %s\n", buffer);
  printf("Sending back: %s\n", fix_subscription_answer);

  n = write(newsockfd, fix_subscription_answer, strlen(fix_subscription_answer));
  if (n < 0) {
    error("ERROR writing to socket");
  }

  /* for (i = 0; i < 100; i++) { */
  /*   n = write(newsockfd, fix_update_message, strlen(fix_update_message)); */
  /*   if (n < 0) { */
  /*     error("ERROR writing to socket"); */
  /*   }     */
  /*   sleep(1); */
  /* } */

  sleep(2);

  close(newsockfd);
  close(sockfd);

  return 0;
}
