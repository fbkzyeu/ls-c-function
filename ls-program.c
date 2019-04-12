
#include <stdio.h>		
#include <sys/types.h>		
#include <dirent.h>		
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int fileInfo(const char *dir);

int main(int argc, char *argv[])
{
	if (argc == 1)
    {
		fileInfo(".");	//without arguments, opens current directory where ls-program is executed by default
    }
	else if( argc > 1)
    {
		for (int i = 1; i < argc; i++)
        {
            fileInfo(argv[i]); //opens all directories put into the arguments 
        }
    }
    else
    {
        return -1;
    }

    return 0;
}



int fileInfo(const char *dir)
{
	//struct that will receive data from scanndir()
	struct dirent **entries;
	
    //scans the directory passed as argument, sorting using alphasort the values in d_name of dirent struct of files and saving it to entries
	int n_entries = scandir(dir, &entries, NULL, alphasort); //scandir returns the number of entries
    if (n_entries == -1) 
    {
        perror("scandir");
    }

    //display formatting header
    printf("%s  %-25s %-8s %-5s%-5s %-12s %-13s %-24s %-20s\n","Permissions", "Name", "Inode", "UID","GID", "Hard Links", "Size (B)", "Last Access", "Last Modification");
    
	for (int i = 0; i < n_entries; i++) 
    {   
	//struct to receive data from stat()
        struct stat file_info;
        //retrieves information about a file in the buffer pointed by pathname, returns a struct with multiple information entries about a file/directory 
        stat(entries[i]->d_name, &file_info);
        
        // Printing File/Directories Permissions
        // use of S_ISDIR() to check if it's a file or directory
        // use of flags from st_mode to check if bits are activated or not in USR/GRP/OTH permissions to RWX
        printf( (S_ISDIR(file_info.st_mode) ? "d": "-"));
        printf( (file_info.st_mode & S_IRUSR) ? "r" : "-");
        printf( (file_info.st_mode & S_IWUSR) ? "w" : "-");
        printf( (file_info.st_mode & S_IXUSR) ? "x" : "-");
        printf( (file_info.st_mode & S_IRGRP) ? "r" : "-");
        printf( (file_info.st_mode & S_IWGRP) ? "w" : "-");
        printf( (file_info.st_mode & S_IXGRP) ? "x" : "-");
        printf( (file_info.st_mode & S_IROTH) ? "r" : "-");
        printf( (file_info.st_mode & S_IWOTH) ? "w" : "-");
        printf( (file_info.st_mode & S_IXOTH) ? "x" : "-");
        
        // time converter - number of seconds since the unix Epoch to readable human format (ex: 19:49:20 - 27 Apr 19)
        char t_access[70];
        char t_modification[70]; 
        struct tm time;
        localtime_r(&file_info.st_atime, &time);
        strftime(t_access, sizeof t_access, "%T - %d %h %y", &time);
        localtime_r(&file_info.st_mtime, &time);
        strftime(t_modification, sizeof t_modification, "%T - %d %h %y", &time);
        
        //printing file information formatted
		printf("%2s %-25s %-8ld %-4u %-5u %-12lu %-13ld %-24s %-20s\n","", entries[i]->d_name, entries[i]->d_ino, file_info.st_uid, file_info.st_gid, file_info.st_nlink, file_info.st_size, t_access, t_modification);
        
		free(entries[i]);
	}
    printf("\n");

	free(entries);

	return 0;
}




