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

static const char *dirpath = "/home/maya/shift4";
static const char *youtuber = "/home/maya/shift4/YOUTUBER";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char fpath[1000];
	if(strcmp(path, "/") == 0)
	{
		path=dirpath;
		sprintf(fpath, "%s", path);
	}
	else sprintf(fpath, "%s%s", dirpath, path);
	
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
	else sprintf(fpath, "%s%s", dirpath, path);

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
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
			char* argv[] = {"zenity", "--error", "--text=File ekstensi iz1 tidak boleh diubah permissionnya.", NULL};
			execv("/usr/bin/zenity", argv);
			return 0;
			//system("zenity --error --text=File ekstensi iz1 tidak boleh diubah permissionnya.");
		}
	}
	else{
		int res = 0;

		res = chmod(fpath, mode);
		if (res == -1)
			return -errno;

		return 0;
	}
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