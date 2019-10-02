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

void permissionCheck(char s[],char c[]){
	write(1, c, len(c));
	write(1, " is created: ", len(" is created: "));
	struct stat fileStat;
	if(stat(s,&fileStat) == -1){
		write(1,"No\n\n",len("No\n\n"));
		return;
	}
	write(1,"Yes\n\n",len("Yes\n\n"));

	if(fileStat.st_mode & S_IRUSR){
		write(1,"User has read permissions on ",len("User has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n",len(": Yes\n"));
	}
	else{
		write(1,"User has read permissions on ",len("User has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n",len(": No\n"));
	}
	if(fileStat.st_mode & S_IWUSR){
		write(1,"User has read permissions on ",len("User has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n",len(": Yes\n"));
	}
	else{
		write(1,"User has read permissions on ",len("User has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n",len(": No\n"));
	}
	if(fileStat.st_mode & S_IXUSR){
		write(1,"User has read permissions on ",len("User has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n\n",len(": Yes\n\n"));
	}
	else{
		write(1,"User has read permissions on ",len("User has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n\n",len(": No\n\n"));
	}

	if(fileStat.st_mode & S_IRGRP){
		write(1,"Group has read permissions on ",len("Group has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n",len(": Yes\n"));
	}
	else{
		write(1,"Group has read permissions on ",len("Group has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n",len(": No\n"));
	}
	if(fileStat.st_mode & S_IWGRP){
		write(1,"Group has read permissions on ",len("Group has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n",len(": Yes\n"));
	}
	else{
		write(1,"Group has read permissions on ",len("Group has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n",len(": No\n"));
	}
	if(fileStat.st_mode & S_IXGRP){
		write(1,"Group has read permissions on ",len("Group has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n\n",len(": Yes\n\n"));
	}
	else{
		write(1,"Group has read permissions on ",len("Group has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n\n",len(": No\n\n"));
	}

	if(fileStat.st_mode & S_IROTH){
		write(1,"Others has read permissions on ",len("Others has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n",len(": Yes\n"));
	}
	else{
		write(1,"Others has read permissions on ",len("Others has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n",len(": No\n"));
	}
	if(fileStat.st_mode & S_IWOTH){
		write(1,"Others has read permissions on ",len("Others has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n",len(": Yes\n"));
	}
	else{
		write(1,"Others has read permissions on ",len("Others has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n",len(": No\n"));
	}
	if(fileStat.st_mode & S_IXOTH){
		write(1,"Others has read permissions on ",len("Others has read permissions on "));
		write(1,c,len(c));
		write(1,": Yes\n\n",len(": Yes\n\n"));
	}
	else{
		write(1,"Others has read permissions on ",len("Others has read permissions on "));
		write(1,c,len(c));
		write(1,": No\n\n",len(": No\n\n"));
	}	
}

ll isReversed(char a[], char b[]){
	struct stat fileStat;
	if(stat(a,&fileStat) == -1 || stat(b,&fileStat) == -1){
		return 0;
	}

	ll fd1 = open(a,O_RDONLY);
	if(fd1 < 0){
		perror(a);
		exit(1);
	}

	ll fd2 = open(b,O_RDONLY);
	if(fd2 < 0){
		perror(b);
		exit(1);
	}

	ll size = lseek(fd1,0,SEEK_END);
	ll size2 = lseek(fd2,0,SEEK_END);
	if(size != size2)
		return 0;

	lseek(fd2,0,SEEK_SET);
	ll buffer_size = 1e6;

	char str[15]="\b\b\b00%";
	ll prev = -1;
	ll done = 0;
	char c[1000002],d[1000002];
	for (ll k = size/buffer_size ; k >= 1; k--) {  
		lseek(fd1, -buffer_size, SEEK_CUR);
		read(fd1, &c, buffer_size);
		read(fd2, &d, buffer_size);

		for(ll i = 0, j = buffer_size - 1; j >= 0 ; i++, j--){
			if(c[i]!=d[j]){
				close(fd1);
				close(fd2);
				return 0;
			}
		}

		done = (size/buffer_size + 1 - k)*buffer_size*100/size;
		if(done>prev){
			prev=done;
			str[3]=done/10+'0';
			str[4]=done%10+'0';
			write(1,&str,len(str));
		}

		lseek(fd1, -buffer_size, SEEK_CUR);     
	}

	if(size%buffer_size!=0){
		lseek(fd1, 0, SEEK_SET);
		read(fd1, &c, size%buffer_size);
		read(fd2, &d, size%buffer_size); 

		for(ll i = 0, j = size%buffer_size - 1; j >= 0; i++, j--){
			if(c[i]!=d[j]){
				close(fd1);
				close(fd2);
				return 0;
			}
		}

	}
	write(1,"\b\b\b100%",len("\b\b\b100%"));

	close(fd1);
	close(fd2);
	
	return 1;
}

	int main(int argc, char *argv[]) {

		if(argc < 4){
			char s[] = "less arguments given\n";
			write(1, &s, len(s));
			exit(1);
		}

		if(argc > 4){
			char s[] = "Please give only these arguments i.e. ./a.out <newfilePath> <oldfilePath> <directoryPath>\n"
			write(1, &s, len(s));
			exit(1);
		}
		
		permissionCheck(argv[1],"newfile");
		permissionCheck(argv[2],"oldfile");
		permissionCheck(argv[3],"directory");


		write(1,"Whether file contents are reversed in newfile:    ",len("Whether file contents are reversed in newfile:    "));
		if(isReversed(argv[1],argv[2]))
			write(1,"\b\b\b\bYes \n\n",len("\b\b\b\bYes \n\n"));
		else
			write(1,"\b\b\bNo  \n\n",len("\b\b\bNo  \n\n"));

		return 0;
	}
