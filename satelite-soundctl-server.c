#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>



#include <alsa/asoundlib.h>
#include <alsa/mixer.h>



void set_vol(long volume)
{
  long min, max;
  snd_mixer_t *handle;
  snd_mixer_selem_id_t *sid;
  const char *card = "default";
  const char *selem_name = "Digital";

  snd_mixer_open(&handle, 0);
  snd_mixer_attach(handle, card);
  snd_mixer_selem_register(handle, NULL, NULL);
  snd_mixer_load(handle);

  snd_mixer_selem_id_alloca(&sid);
  snd_mixer_selem_id_set_index(sid, 0);
  snd_mixer_selem_id_set_name(sid, selem_name);
  snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

  snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
  snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

  snd_mixer_close(handle);
}




int main(int argc, char *argv[])
{

  FILE *fp= NULL;
#if 1


  pid_t process_id = 0;
  pid_t sid = 0;
  // Create child process
  process_id = fork();
  // Indication of fork() failure
  if (process_id < 0)
  {
    printf("fork failed!\n");
    // Return failure in exit status
    exit(1);
  }
  // PARENT PROCESS. Need to kill it.
  if (process_id > 0)
  {
    printf("process_id of child process %d \n", process_id);
    // return success in exit status
    exit(0);
  }
  //unmask the file mode
  umask(0);
  //set new session
  sid = setsid();
  if(sid < 0)
  {
    // Return failure
    exit(1);
  }
  // Change the current working directory to root.
  chdir("/");
  // Close stdin. stdout and stderr
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  // Open a log file in write mode.


  openlog("satelite-server-log", LOG_PID|LOG_CONS, LOG_USER);


  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr; 
  int n = 0;
  uint32_t vol = 0;
  uint32_t volume = 0;
  char recvBuff[1024];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(recvBuff, '0', sizeof(recvBuff)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(8083); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 


  while(1)
  {
    syslog(LOG_INFO, "----- Listening for incoming sound controls at port %d \n",ntohs(serv_addr.sin_port));
    listen(listenfd, 10); 
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
    n = recv(connfd,&vol, sizeof(vol),0);
    volume = ntohl(vol);
    syslog(LOG_INFO,"----- received %ld\n",volume);
    set_vol(volume);
    close(connfd);
  }

}
