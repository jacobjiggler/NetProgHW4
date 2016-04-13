#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <map>





//todo array of multiple serv_addr's
//use this http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/



struct socket_info {
  struct sockaddr_in serv_addr;
  std::string command;
};



int main(int argc , char *argv[])
{
  int children[1000];
  int child_size = 0;
  std::map<int,struct socket_info> sockets;
  std::map<int,struct socket_info>::iterator it;
  fd_set rfds; int maxfd = 0;
  int opt = true;
  /* initialize rfds to the empty set */
  FD_ZERO(&rfds);

  //parse config
  //for each line
  std::ifstream file("config.txt");
  std::string str;
  while (std::getline(file, str))
  {
    //create listening tcp socket
    int mid_index = str.find(" ");
    std::string port = str.substr(0,mid_index);
    std::string command = str.substr(mid_index+1, str.length() - mid_index);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "sockfd " << sockfd << std::endl;
    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    int portno = atoi(port.c_str());
    serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              std::cout << "ERROR on binding" << std::endl;
     if (listen(sockfd,3  ) == -1){
     std::cout<< "Server-listen() error lol!" << std::endl;
     exit(1);
}
    if (sockfd > 0)
      FD_SET(sockfd, &rfds);

     if (sockfd > maxfd)
      maxfd = sockfd;

    struct socket_info temp_sockinfo;
    temp_sockinfo.serv_addr = serv_addr;
    temp_sockinfo.command = command;
     //add to map
     sockets[sockfd] = temp_sockinfo;
  }



  //while loop forever
  while(true) {
    //select or poll to accept connections
    if (select(maxfd + 1, &rfds, NULL, NULL, NULL) < 0){
      perror("Server-select() error lol!");
      exit(1);
    }
    it = sockets.begin();
    /*run through the existing connections looking for data to be read*/
    for(it = sockets.begin(); it!=sockets.end(); it++)
    {
        if(FD_ISSET(it->first, &rfds))
        { /* we got one... */
             /* handle new connections */
          socklen_t addrlen = sizeof(it->second.serv_addr);
          //accept
          int newfd = accept(it->first, (struct sockaddr *)&(it->second.serv_addr), &addrlen);
          if (!(newfd>0))
          {
              printf("Server-accept() error lol!\n");
          }
          else
          {
              printf("Server-accept() is OK...\n");
              //fork
              pid_t pid = fork();
              //if child
              if (pid == 0)
              {
                // child process

                dup2(newfd, STDIN_FILENO);
                dup2(newfd, STDOUT_FILENO);

                //execl command
                execl("/bin/sh", "/bin/sh", "-c", it->second.command.c_str(), NULL);
                //kill myself
                exit(1);
              }
              else if (pid > 0){
              //else
                //add to children array
              }
          }



  }

}
}
  //eventually wait on children






}
