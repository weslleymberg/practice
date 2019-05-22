#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* This code was reproduced from https://learnto.computer/screencasts/bsd-cat as an exercise to undestand the "cat" command. */

void cat(int rfd) {
  
  int wfd;
  static char *buf;
  static size_t bsize;
  struct stat sbuf;
  
  wfd = fileno(stdout);
  
  if(fstat(wfd, &sbuf)){
    err(1, "stdout");
  }
  
  if (buf == NULL){
    bsize = sbuf.st_blksize;
    if ((buf = malloc(bsize)) == NULL){
      err(1, NULL);
    } 
  }
  
  ssize_t nr, nw;
  
  nr = read(rfd, buf, bsize);
  while (nr > 0){
    for(int offset = 0; nr > 0; nr -= nw, offset += nw){
      nw = write(wfd, buf+offset, nr);
      if (nw < 0) {
        err(1, "stdout");
      }
    }
    nr = read(rfd, buf, bsize);
  }
}

int main (int argc, char **argv) {
  
  int fd;

  ++argv;
  fd = fileno(stdin);
  do {
    if (*argv) {
      if (strcmp(*argv, "-") == 0) {
        fd = fileno(stdin);
      } else {
        fd = open(*argv, O_RDONLY);
      }
    
      if (fd < 0) {
        err(1, "%s", *argv);
      }
      ++argv;
    }
    cat(fd);
  } while (*argv);
  
  if (fd != fileno(stdin)){
    close(fd);
  }
  
  exit(1);
}
