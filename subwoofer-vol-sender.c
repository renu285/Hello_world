#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "packet_format.h" 

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>






int main(int argc, char *argv[])
{
  int sockfd = 0, n = 0;
  char sendBuff[1024];
  struct sockaddr_in serv_addr; 

  uint32_t volume = 0;


  if(argc != 3)
  {
    printf("\n Usage: %s <ip of server> \n",argv[0]);
    return 1;
  } 

  memset(sendBuff, '0',sizeof(sendBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Error : Could not create socket \n");
    return 1;
  } 

  memset(&serv_addr, '0', sizeof(serv_addr)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8083); 

  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
  {
    printf("\n inet_pton error occured\n");
    return 1;
  } 

  if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\n Error : Connect Failed \n");
    return 1;
  } 
  //struct packet snd_pkt;
  //snd_pkt.id = 1;
  //snd_pkt.volume = atoi(argv[2]);

  //memcpy(sendBuff,argv[2],sizeof(argv[2]));
  volume = htonl(atoi(argv[2]));

  //n = send(sockfd,&snd_pkt, sizeof(struct packet),0);
 // write(sockfd, &un, sizeof(un));
  //write(sockfd,&snd_pkt,sizeof(struct packet));
  write(sockfd,&volume,sizeof(volume));
    
  //n = send(sockfd,&un, sizeof(un),0);
  //printf("sending id: %d\n",snd_pkt.id);
  printf("sending volume: %s\n",argv[2]);
  if ( n <= 0 )
  {
    //printf("Either Connection Closed or Error\n");
  }  
  return 0;
}
