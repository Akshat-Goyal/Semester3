#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#define ll long long int

ll len(char *s){
	ll n=0;
	while(s[n]!='\0')
		n++;
	return n;
}

void cat(char *s,char *c){
	ll n=len(s),n2=len(c);
	ll k = n2-1;
	while(k>=0 && c[k]!='/')
		k--;
	k++;
	for(ll i=n;c[i-n+k]!='\0';i++)
		s[i]=c[i-n+k];
}


	int main(int argc, char *argv[]) {

		if(argc < 2){
			char s[] = "Please give the name of file to be read!\n";
			write(1, &s, len(s));
			exit(1);
		}	

		if(argc >= 3){
			char s[] = "Please give only the name of file to be read!\n";
			write(1, &s, len(s));
			exit(1);
		}

		ll fd1=0, fd2=0;

		fd1 = open(argv[1], O_RDONLY);

		if (fd1 < 0) {
			perror(argv[1]);
			exit(1);
		}

		struct stat fileStat;
		if(stat("Assignment",&fileStat)==-1){
			fd2 = mkdir("Assignment", 0700);
			if(fd2 == -1){
				perror("Assignment folder not created");
				exit(1);
			}
		}
		else{
			chmod("Assignment", 0700);
		}


		char s[150]="./Assignment/";
		cat(s,argv[1]);
		if(stat(s,&fileStat)!=-1)
			remove(s);
		fd2 = open(s, O_WRONLY | O_CREAT, 0600);

		if(fd2 < 0) {
			perror(s);
			exit(1);
		}

 
		write(1,"Progress Status\n",len("Progress Status\n"));

		ll size = lseek(fd1, 0, SEEK_END);
		ll buffer_size = 1e6;

		char str[8]="\r00%\0";
		char c[1000002];
		ll prev = -1;
		ll done = 0;
		for (ll k = size/buffer_size ; k >= 1; k--) {  
			lseek(fd1, -buffer_size, SEEK_CUR);
			read(fd1, &c, buffer_size);

			for(ll i = 0, j = buffer_size - 1; i < j ; i++, j--){
				char temp = c[i];c[i]=c[j];c[j]=temp;
			}

			write(fd2, &c, buffer_size);

			done = (size/buffer_size + 1 - k)*buffer_size*100/size;
			if(done>prev){
				prev=done;
				str[1]=done/10+'0';
				str[2]=done%10+'0';
				write(1,&str,len(str));
			}

			lseek(fd1, -buffer_size, SEEK_CUR);     
		}

		if(size%buffer_size!=0){
			lseek(fd1, 0, SEEK_SET);
			read(fd1, &c, size%buffer_size); 

			for(ll i = 0, j = size%buffer_size - 1; i < j ; i++, j--){
				char temp = c[i];c[i]=c[j];c[j]=temp;
			}

			write(fd2, &c, size%buffer_size);
			
		}
		write(1,"\r100%\n",len("\r100%\n"));
		
		close(fd1);
		close(fd2);

		return 0;
	}