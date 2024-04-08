#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CHARACTERS 20

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

    char buffer[MAX_CHARACTERS];
    int num_characters = read(STDIN_FILENO, buffer, sizeof(buffer));
    buffer[--num_characters] = '\0';
    
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

    num_characters = read(STDIN_FILENO, buffer, sizeof(buffer));
    buffer[--num_characters] = '\0';
    
    int current_brightness = atoi(buffer);

    printf("%d\n", (current_brightness * 100) / max_brightness);

    waitpid(id_1, NULL, 0);
    waitpid(id_2, NULL, 0);

    return 0;
}
