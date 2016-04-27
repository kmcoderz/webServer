#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>

#define PORT 9090
#define BACKLOG 10
#define MAX 1024
char PATH[] = "/home/user/www/";

char buff[MAX];
char req[MAX];
void response(int clntfd,char *req){
	int i,fd,error=200,j,len,flag=1,num;
	char filepath[200],*file,filetemp[200],dirpath[200],curpath[200];
	if((file = strstr(req,"GET"))!=NULL)
	{
		i=5;
		j=0;
		while(1)
		{
			if(file[i]!='\n')
			{
				if(file[i]=='H' && file[i+1]=='T' && file[i+2]=='T' && file[i+3]=='P')
				break;
				filetemp[j++] = file[i++];
			}
			else
			{
				break;
			}
		}
		filetemp[j-1]='\0';
		//Check
		flag=1; 
		if(strlen(filetemp)==0)
		{
			//No Extension
			sprintf(dirpath,"%s\0",PATH);
			sprintf(filepath,"%s%s\0",PATH,"index.html");
			if(access (filepath, F_OK))
			{
				//File Doesn't Exists
				flag=0;
			}
			
		}
		else
		{
			sprintf(filepath,"%s%s\0",PATH,filetemp);
			struct stat buff;
			stat(filepath,&buff);
			if(S_ISDIR(buff.st_mode))
			{
				//Directory
				//No Extension
				len = strlen(filepath);
				if(filepath[len-1]!='/')
				{
					filepath[len]='/';
					filepath[len+1]='\0';
				}
				sprintf(dirpath,"%s\0",filepath);	
				sprintf(filepath,"%s%s\0",filepath,"index.html");
				if(access (filepath, F_OK))
				{
					//File Doesn't Exists
					flag=0;
					
				}
			}
			else
			{
				//File
			}
		}
		if(flag==0)
		{
			write(clntfd,"HTTP/1.1 200 OK\r\n",17);
			write(clntfd,"Server: WebBoxServer\r\n",21);
			write(clntfd,"Content-Type: text/html; charset=UTF-8\r\n\r\n",42);
			write(clntfd,"<html><head><title>Index of</title></head><body><h1>Index of</h1><table>",73);
			write(clntfd,"<tr><th>Name</th><th>Type</th></tr><tr><th colspan='2'><hr/></th></tr>",70);
			DIR *dirp;
			struct dirent *dir;
			dirp=opendir(dirpath);
			if(dirp==NULL)
			{
				printf("Cannot Open Directory Entry..");
			
			}
			else
			{
				while(dir=readdir(dirp)) //Reads the directory entry
				{
					if(strcmp(dir->d_name,".") && strcmp(dir->d_name,"..") && (strstr(dir->d_name,"~")==NULL))
					{
						char *row;
						sprintf(curpath,"%s%s\0",dirpath,dir->d_name);
						struct stat buff;
						stat(curpath,&buff);
						if(S_ISDIR(buff.st_mode))//A directory
							sprintf(row,"<tr><td><a href='%s/'>%s</a></td><td>Directory</td></tr>\0",dir->d_name,dir->d_name);
						else//A file
							sprintf(row,"<tr><td><a href='%s'>%s</a></td><td>File</td></tr>\0",dir->d_name,dir->d_name);
						write(clntfd,row,strlen(row));	
					}
				}		

			}
			closedir(dirp);
			write(clntfd,"<tr><th colspan='2'><hr/></th></tr></table></body></html>",57);	
			
		}
		else
		{
		
			
		
			printf("\nFrom Client : %s",req);
			fd = open(filepath,O_RDONLY);
			if(fd == -1)
			{
				error = 404;
				write(clntfd,"HTTP/1.1 404 Not Found\r\n",24);	
				write(clntfd,"Server: WebBoxServer\r\n",21);
				write(clntfd,"Content-Type: text/html; charset=UTF-8\r\n\r\n",42);
				write(clntfd,"<html><head><title>404 not found error!!</title></head><body><h1>Error</h1><h3>404- File Not Found</h3></body></html>",118);
		
			}
	
			if(error == 200)
			{//ok
				write(clntfd,"HTTP/1.1 200 OK\r\n",17);
				write(clntfd,"Server: WebBoxServer\r\n",21);
				if(strstr(filepath,".png"))
					write(clntfd,"Content-Type: image/png; charset=UTF-8\r\n\r\n",42);
				else
				if(strstr(filepath,".html"))
					write(clntfd,"Content-Type: text/html; charset=UTF-8\r\n\r\n",42);
				else
				if(strstr(filepath,".htm"))
					write(clntfd,"Content-Type: text/html; charset=UTF-8\r\n\r\n",42);
				else
				if(strstr(filepath,".css"))
					write(clntfd,"Content-Type: text/plain; charset=UTF-8\r\n\r\n",43);
				else
				if(strstr(filepath,".gif"))
					write(clntfd,"Content-Type: image/gif; charset=UTF-8\r\n\r\n",42);
				else
				if(strstr(filepath,".txt"))
					write(clntfd,"Content-Type: text/plain; charset=UTF-8\r\n\r\n",43);
				else
				if(strstr(filepath,".jpg") || strstr(filepath,".JPG"))
					write(clntfd,"Content-Type: image/jpg; charset=UTF-8\r\n\r\n",42);
				else
				if(strstr(filepath,".jpeg"))
					write(clntfd,"Content-Type: image/jpeg; charset=UTF-8\r\n\r\n",43);
				else
				if(strstr(filepath,".ico"))
					write(clntfd,"Content-Type: image/ico; charset=UTF-8\r\n\r\n",42);
				else
				if(strstr(filepath,".pdf"))
					write(clntfd,"Content-Type: application/pdf; charset=UTF-8\r\n\r\n",48);
				else
				if(strstr(filepath,".zip"))
					write(clntfd,"Content-Type: application/octet-stream; charset=UTF-8\r\n\r\n",57);
				else
				if(strstr(filepath,".js"))
					write(clntfd,"Content-Type: application/x-javascript; charset=UTF-8\r\n\r\n",57);
				else
				if(strstr(filepath,".rar"))
					write(clntfd,"Content-Type: application/octet-stream; charset=UTF-8\r\n\r\n",57);
				else
				if(strstr(filepath,".gz"))
					write(clntfd,"Content-Type: application/octet-stream; charset=UTF-8\r\n\r\n",57);
				else
				if(strstr(filepath,".tar"))
					write(clntfd,"Content-Type: application/octet-stream; charset=UTF-8\r\n\r\n",57);
				else
					write(clntfd,"Content-Type: text/html; charset=UTF-8\r\n\r\n",42);
				
				
				
				memset(buff,'\0',MAX);
				while((num=read(fd,buff,MAX-1))>0)
				{
					write(clntfd,buff,num);	
					memset(buff,'\0',MAX);
				}
			}
			close(fd);
			write(clntfd, "\r\n", 2);
		}
	}	
}
 
int main()
{
	//Variable
	int sockfd,clntfd,num;
	struct sockaddr_in servaddr,clntaddr;
	socklen_t sin_len = sizeof(clntaddr);
	int one = 1;
	//Socket
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("Failed: Socket");
		exit(1);
	}
	//Check Bind
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)); //New Bind
	
	//Set Structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//Bind
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		close(sockfd);
		perror("Failed: Bind");
		exit(1);
	}
	printf("\nServer Started on Port: %d",PORT);
	//Listen
	listen(sockfd,BACKLOG);
	printf("\nWaiting for Clients\n");
	
	//Get Connected to Client
	while (1) {
		clntfd = accept(sockfd, (struct sockaddr *)&clntaddr, &sin_len);
		if (clntfd == -1) {
			perror("Can't accept");
			exit(1);
		}
		if(!fork()) // For each client new child process created
		{
			close(sockfd);
			printf("Received Connection !!\n");
			
			if((num = recv(clntfd,req,MAX-1,0))<0)
				exit(1);
			req[num] = '\0';
	
			response(clntfd,req);
			
			exit(0);
		}
		else
		{
			close(clntfd);
		}
	
	}
	close(sockfd);
	return 0;
}
