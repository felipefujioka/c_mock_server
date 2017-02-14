#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int n;
  int sockfd, newsockfd, portno;

  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  char buffer[BUFFER_SIZE];
  char line[100];
  char fix_logon_answer[100];
  char fix_subscription_answer[100];

  char* filename;

  FILE * file;
  int messages_sent = 0;

  clock_t cpu_start;
  clock_t cpu_end;
  double elapsed_time;

  double elapsed_wallclock_time;
  struct timeval wallclock_start, wallclock_end;

  if (argc < 3) {
    fprintf(stderr,"ERROR, no port or filename provided\n");
    exit(1);
  }

  filename = argv[2];
  file = fopen(filename, "r");

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

  /* Wait for first message */
  bzero(buffer, BUFFER_SIZE);
  n = read(newsockfd, buffer, BUFFER_SIZE);
  if (n < 0) {
    error("ERROR reading from socket");
  }
  printf("Received: %s\n", buffer);

  /* Send first message            */
  gettimeofday(&wallclock_start, NULL);
  cpu_start = clock();
  fscanf(file, "%s\n", fix_logon_answer);
  n = write(newsockfd, fix_logon_answer, strlen(fix_logon_answer));
  messages_sent++;
  if (n < 0) {
    error("ERROR writing to socket");
  }

  /* Do not wait the second message */
  /* bzero(buffer, BUFFER_SIZE); */
  /* n = read(newsockfd, buffer, BUFFER_SIZE); */
  /* if (n < 0) { */
  /*   error("ERROR reading from socket"); */
  /* } */
  /* printf("Received: %s\n", buffer); */

  /* Send second message */
  fscanf(file, "%s\n", fix_subscription_answer);
  n = write(newsockfd, fix_subscription_answer, strlen(fix_subscription_answer));
  messages_sent++;
  if (n < 0) {
    error("ERROR writing to socket");
  }

  /* Send remaining messages */
  while ((n = fscanf(file, "%s\n", line)) != EOF) {
    n = write(newsockfd, line, strlen(line));
    messages_sent++;
    if (n < 0) {
      error("ERROR writing to socket");
    }
  }

  cpu_end = clock();
  gettimeofday(&wallclock_end, NULL);

  elapsed_time = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
  elapsed_wallclock_time = (double) (((long) (wallclock_end.tv_sec * 1000000 + wallclock_end.tv_usec)
                                      - (wallclock_start.tv_sec * 1000000 + wallclock_start.tv_usec))) / 1000;

  printf("Sent %d messages in %fs (CPU) %fms (real)\n", messages_sent,  elapsed_time, elapsed_wallclock_time);

  close(newsockfd);
  close(sockfd);
  fclose(file);

  return 0;
}
