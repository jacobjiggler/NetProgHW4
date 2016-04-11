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

int main(int argc , char *argv[])
{
  int children[1000];
  int child_size = 0;
  std::map<int,std::string> sockets;
  std::map<int,std::string>::iterator it;
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
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    int portno = atoi(port.c_str());
    serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              std::cout << "ERROR on binding" << std::endl;
     listen(sockfd,5);
     //add to map
     sockets[portno] = command;
  }
  //while loop forever through map
  it = sockets.begin();
  while(true) {
    //select or poll to accept connection
      //accept
      //fork
      //if child
        //dup2(socket, STDIN_FILENO);
        //dup2(socket, STDOUT_FILENO);

        //execl command
        //kill myself
      //else
        //add to children array
      it++;
      if (it == sockets.end())
        it = sockets.begin();
  }
  //eventually wait on children






}
