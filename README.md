# SoalShift_modul4_C06

Soal Shift Modul 4

Suatu ketika King of YouTube ASEAN, Atta Halilintar, mengunjungi laboratorium AJK karena urusan yang benar-benar genting. Prestasi dan ketenarannya ternyata menarik perhatian hacker yang ingin mencuri video-video berkualitas milik Atta, bahkan ingin mencuri password akun YouTube miliknya. Menurut intel Atta, pemimpin hacker yang berusaha menyerangnya adalah:

Hacker LAPTOP_RUSAK<br>
Hacker tersebut menyusup ke dalam file system yang ada di dalam PC Atta dengan mudah karena ternyata file system tersebut sama sekali tidak memiliki sistem pengamanan. Maka dari itu maksud Atta ke laboratorium AJK karena dia ingin kalian untuk mengembangkan file system yang aman.<br>
Atta menginginkan nama file system yang akan kalian buat adalah “AFSHiaAP”, yang nama kepanjangannya adalah AHHA’s File System: Hierarchical and Advanced Protection. Berikut ini kebutuhan yang harus kalian penuhi:

1. Semua nama file dan folder harus terenkripsi<br>
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:
```
qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0
```
Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .<br>
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan<br>

Jawab :<br>
- membuat fungsi enkripsi<br>
```c
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
```
- membuat fungsi dekripsi<br>
```c
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
```


2. Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”<br>
Urutan operasi dari kebutuhan ini adalah:<br>
Tepat saat sebelum file system di-mount<br>
Secara otomatis folder “Videos” terbuat di root directory file system<br>
Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”<br>
Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse<br>
Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut<br>
Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”<br>
Jangan tampilkan file pecahan di direktori manapun<br>
Tepat saat file system akan di-unmount<br>
Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system<br>
Hapus folder “Videos” <br>

Jawab :<br>
ndak paham buat nge joinnya mbak :(

3. Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
```
Owner Name     : ‘chipset’ atau ‘ic_controller’
Group Name    : ‘rusak’
Tidak dapat dibaca
```
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

Jawab:<br>
- Pada fungsi readdir, mengubah bagian while
```c
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
```

4. Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

Jawab:<br>
- Pada fungsi mkdir diubah menjadi
```c
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
```
- Pada fungsi mknod diubah menjadi
```c
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
```
- Pada fungsi chmod ditambahi
```c
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
			}
			return 0;
		}
	}
```

5. Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss

jawab : <br>
maaf mbak ndak bisa :( kesulitan di nomor2 sebelumnya, jadi yg ini ndak dikerjakan

Catatan: 
- Semua nama file dan folder yang terdapat pada soal harus memenuhi syarat soal no. 1 (terenkripsi saat belum di-mount dan terdekripsi saat di-mount)
- Tidak diperkenankan menggunakan system() dan exec*(), kecuali ada pengecualian di butir soal.
- Untuk soal nomor 3 diharapkan secara manual membuat user dan group-nya. Pengecekan file dilakukan setiap membuka direktori.
- Untuk soal nomor 4 dan 5 diperbolehkan menggunakan exec*().
- Pengerjaan hanya dilakukan dalam 1 file program C dengan format nama AFSHiaAP_[Kelompok].c . 
- File System AFSHiaAP mengarah ke folder /home/[user]/shift4.
