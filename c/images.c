#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_CHARACTERS 80

int main()
{
    char dir_names[MAX_CHARACTERS];
    dir_names[0] = '\0';
    char images_path[MAX_CHARACTERS];
    images_path[0] = '\0';

    char *home = getenv("HOME");
    DIR *images_directory;

    strncat(images_path, home, strlen(home) + 1);
    strncat(images_path, "/images/", 9);

    if (!(images_directory = opendir(images_path))) { return 1; }

    struct dirent *entry;

    while ((entry = readdir(images_directory)) != NULL)
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
        {
            int length_dir_name = strlen(entry->d_name);
            char new_name[length_dir_name + 2];
            new_name[0] = '\0';

            strncat(new_name, entry->d_name, length_dir_name);
            new_name[length_dir_name] = '\n';
            new_name[length_dir_name + 1] = '\0';

            strncat(dir_names, new_name, length_dir_name + 2);
        }

    if (closedir(images_directory) == -1) { return 1; }
    
    char *usb_images_path = "/mnt/usb/wallpapers/";
    DIR *usb_directory; 

    if (usb_directory = opendir(usb_images_path))
    {
        if (closedir(images_directory) == -1) { return 1; }
        strncat(dir_names, "usb", 4);
    }

    int df_1[2];
    int df_2[2];
    if (pipe(df_1) == -1 || pipe(df_2) == -1) { return 1; }

    int id_1 = fork();
    if (id_1 == -1) { return 1; }

    if (id_1 == 0)
    {
        close(df_1[1]);
        dup2(df_1[0], STDIN_FILENO);
        close(df_1[0]);

        close(df_2[0]);
        dup2(df_2[1], STDOUT_FILENO);
        close(df_2[1]);

        execlp("dmenu", "dmenu", "-p", "Directories", "-i", NULL);
    }

    char buffer[MAX_CHARACTERS];

    close(df_1[0]);
    dprintf(df_1[1], "%s", dir_names);
    close(df_1[1]);

    close(df_2[1]);
    dup2(df_2[0], STDIN_FILENO);
    close(df_2[0]);

    int num_characters = read(STDIN_FILENO, buffer, sizeof(buffer));
    buffer[--num_characters] = '\0';

    int id_2 = fork();
    if (id_2 == -1) { return 1; }

    if (id_2 == 0)
    {
        if (strlen(buffer) <= 1)
            return 0;
        else if (strncmp(buffer, "usb", 3) == 0)
            execlp("nsxiv", "nsxiv", usb_images_path, NULL); 
        else
        {
            strncat(images_path, buffer, num_characters + 1);
            execlp("nsxiv", "nsxiv", images_path, NULL); 
        }
    }

    waitpid(id_1, NULL, 0);
    waitpid(id_2, NULL, 0);

    return 0;
}
