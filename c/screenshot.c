#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CHARACTERS 100

int main()
{
    unsigned char *home_path = getenv("HOME");
    unsigned char *save_as = "/images/screenshots/%d-%m-%Y-%T-ss.png";
    unsigned char screenshot_path[MAX_CHARACTERS];
    
    screenshot_path[0] = '\0';
    strncat(screenshot_path, home_path, strlen(home_path) + 1);
    strncat(screenshot_path, save_as, strlen(save_as) + 1);

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

        execlp("dmenu", "dmenu", "-p", "Take screenshot", "-i", NULL);
    }

    close(df_1[0]);
    dprintf(df_1[1], "%s", "full\nselect\nwindow");
    close(df_1[1]);

    close(df_2[1]);
    dup2(df_2[0], STDIN_FILENO);
    close(df_2[0]);

    char buffer[MAX_CHARACTERS];
    int num_characters = read(STDIN_FILENO, buffer, sizeof(buffer));
    buffer[--num_characters] = '\0';
    
    int id_2 = fork();
    if (id_2 == -1) { return 1; }

    if (id_2 == 0)
    {
        if (strncmp(buffer, "full", 1) == 0)
            execlp("scrot", "scrot", screenshot_path, NULL);
        else if (strncmp(buffer, "select", 1) == 0)
            execlp("scrot", "scrot", screenshot_path, "--select", "--line", "mode=edge", NULL);
        else if (strncmp(buffer, "window", 1) == 0)
            execlp("scrot", "scrot", screenshot_path, "--focused", "--border", NULL);
        else
            return 0;
    }

    waitpid(id_1, NULL, 0);
    waitpid(id_2, NULL, 0);

    return 0;
}
