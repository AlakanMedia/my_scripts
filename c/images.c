#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int df_1[2];
    int df_2[2];
    if (pipe(df_1) == -1) { return 1; }
    if (pipe(df_2) == -1) { return 1; }

    int id_1 = fork();
    if (id_1 == -1) { return 1; }

    char *home = getenv("HOME");
    char images_path[80];
    int home_length = strlen(home);

    images_path[0] = '\0';
    strncat(images_path, home, home_length + 1);
    strncat(images_path, "/images/", 9);

    if (id_1 == 0)
    {
        close(df_2[0]);
        close(df_2[1]);
        close(df_1[0]);
        dup2(df_1[1], STDOUT_FILENO);
        close(df_1[1]);

        execlp("ls", "ls", images_path, NULL);
    }

    int id_2 = fork();
    if (id_2 == -1) { return 1; }

    if (id_2 == 0)
    {
        close(df_1[1]);
        close(df_2[0]);
        dup2(df_1[0], STDIN_FILENO);
        close(df_1[0]);
        dup2(df_2[1], STDOUT_FILENO);
        close(df_2[1]);

        execlp("dmenu", "dmenu", "-p", "Select directory", "-i", NULL);
    }

    close(df_1[0]);
    close(df_1[1]);
    close(df_2[1]);
    dup2(df_2[0], STDIN_FILENO);
    close(df_2[0]);

    char buffer[20];
    read(STDIN_FILENO, buffer, sizeof(buffer));

    for (int i = 0; i < 20; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            break;
        }

    waitpid(id_1, NULL, 0);
    waitpid(id_2, NULL, 0);

    strncat(images_path, buffer, strlen(buffer) + 1);

    int id_3 = fork();
    if (id_3 == -1) { return 1; }

    if (id_3 == 0)
        execlp("nsxiv", "nsxiv", images_path, NULL);

    waitpid(id_3, NULL, 0);
    
    return 0;
}
