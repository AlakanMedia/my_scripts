#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int df_1[2];
    if (pipe(df_1) == -1) { return 1; }

    int id_1 = fork();
    if (id_1 == -1) { return 1; }

    if (id_1 == 0)
    {
        close(df_1[0]);
        dup2(df_1[1], STDOUT_FILENO);
        close(df_1[1]);

        execlp("brightnessctl", "brightnessctl", "m", NULL);
    }

    dup2(df_1[0], STDIN_FILENO);
    close(df_1[0]);

    char buffer[20];
    read(STDIN_FILENO, buffer, sizeof(buffer));
    
    for (int i = 0; i < 20; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            break;
        }
    
    int max_brightness = atoi(buffer);

    int id_2 = fork();
    if (id_2 == -1) { return 1; }

    if (id_2 == 0)
    {
        close(df_1[0]);
        dup2(df_1[1], STDOUT_FILENO);
        close(df_1[1]);

        execlp("brightnessctl", "brightnessctl", "g", NULL);
    }

    close(df_1[1]);

    read(STDIN_FILENO, buffer, sizeof(buffer));
    
    for (int i = 0; i < 20; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            break;
        }

    int current_brightness = atoi(buffer);

    printf("%d\n", (current_brightness * 100) / max_brightness);

    waitpid(id_1, NULL, 0);
    waitpid(id_2, NULL, 0);

    return 0;
}
