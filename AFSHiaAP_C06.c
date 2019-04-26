#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#define key 17

static const char *dirpath = "/home/maya/shift4";
static const char *youtuber = "/home/maya/shift4/YOUTUBER";
static const char *mirisPath = "/home/maya/shift4/filemiris.txt";
char x[] ="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

void encr(char inp[]){
	int idx; 
	int i, j;
	for(i = 0 ; i< strlen(inp) ;i++){
		if(inp[i]=='/')
			inp[i] = inp[i];
		
		else{
			for(j = 0; j<strlen(x) ; j++){
				if(inp[i] == x[j]){
					idx = j - key ;
				break;
			}
		}
		if(idx < 0 )
			idx = idx + 94;
		
		inp[i] = x[idx];}
	}
}

void decr(char inp[]){
	int idx; 
	int i, j;
	for(i = 0 ; i< strlen(inp) ;i++){
		if(inp[i]=='/')
			inp[i] = inp[i];
		
		else{
			for(j = 0; j<strlen(x) ; j++){
				if(inp[i] == x[j]){
					idx = j + key;
				break;
			}
		}
		if(idx > strlen(x))
			idx = idx - 94;
		
		inp[i] = x[idx];}
	}
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char fpath[1000];
	if(strcmp(path, "/") == 0)
	{
		path=dirpath;
		sprintf(fpath, "%s", path);
	}
	else 
	{	char temp[1000];
		strcpy(temp,path);
		encr(temp);
		sprintf(fpath, "%s%s",dirpath,temp);
	 }
	int res;

	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	if(strcmp(path, "/") == 0)
	{
		path=dirpath;
		sprintf(fpath, "%s", path);
	}
	else
	{
		char temp[1000];
		strcpy(temp,path);
		encr(temp);

		sprintf(fpath, "%s%s",dirpath,temp);	
	}
	int res = 0;
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		char fname[1000];
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if(strcmp(de->d_name, ".") && strcmp(de->d_name,".."))
      		{	
			strcpy(fname, de->d_name);
            		decr(fname);
    			res = (filler(buf,fname, &st, 0));
 	       	  }
        	else
        	{
            		res = (filler(buf,de->d_name, &st, 0));   
        	}
		if(res!=0) break;
		
		struct stat theFile;

		char fileName[100];
		strcpy(fileName, de->d_name);
		char filePath[1000];
		sprintf(filePath, "%s/%s", fpath, fileName);
		stat(filePath, &theFile);

		struct passwd *owner = getpwuid(theFile.st_uid);
		struct group *group = getgrgid(theFile.st_gid);
		struct tm *theTime = localtime(&theFile.st_atime);

		char owname[100];
		strcpy(owname, owner->pw_name);
		char grname[100];
		strcpy(grname, group->gr_name);

		if(strcmp(owname, "chipset")==0 || strcmp(owname, "ic_controller")==0 || strcmp(grname, "rusak")==0 || access(filePath, R_OK)!=0)
		{
			FILE *filemiris = fopen(mirisPath, "a");
			fprintf(filemiris, "File Name : %s\nGroup ID : %d\nOwner ID : %d\nRecently Accessed : %d-%d-%d %d:%d:%d\n", fileName, group->gr_gid, owner->pw_gid, theTime->tm_mday, theTime->tm_mon+1, theTime->tm_year+1900, theTime->tm_hour, theTime->tm_min, theTime->tm_sec);
			fclose(filemiris);
			remove(filePath);
		}
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	int res = 0;
	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",dirpath);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	int res = 0;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */

	char newname[1000];
	sprintf(newname, "%s.iz1", fpath);

	if (S_ISREG(mode)) {
		if(strstr(fpath, youtuber)){
			mode = 33184;
			res = open(newname, O_CREAT | O_EXCL | O_WRONLY, mode);
		}
		else{
			res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		}
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode)){
		if(strstr(fpath, youtuber)){
			mode = 33184;
			res = mkfifo(newname, mode);
		}
		else{
			res = mkfifo(fpath, mode);
		}
	}
	else{
		if(strstr(fpath, youtuber)){
			mode = 33184;
			res = mknod(newname, mode, rdev);
		}
		else{
			res = mknod(fpath, mode, rdev);
		}
	}
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	int res = 0;

	if(strstr(fpath, youtuber)){
		mode = 488;
	}
	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	int res = 0;

	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	if(strstr(fpath, youtuber))
	{
		if(strstr(fpath,".iz1"))
		{
			pid_t child_id;

			child_id = fork();
			
			if (child_id < 0) {
				exit(EXIT_FAILURE);
			}

			if (child_id == 0) {
				// this is child
				char* argv[] = {"zenity", "--error", "--text=File ekstensi iz1 tidak boleh diubah permissionnya.", NULL};
				execv("/usr/bin/zenity", argv);
				return 0;
			}
			//system("zenity --error --text=File ekstensi iz1 tidak boleh diubah permissionnya.");
		}
	}
	int res = 0;

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
    .mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.chmod		= xmp_chmod,
	.utimens	= xmp_utimens,
	.write		= xmp_write,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
