#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    unsigned char *home = getenv("HOME");
    int home_length = strlen(home);
    unsigned char *screenshot_directory = "/images/screenshots/%d-%m-%Y-%T-ss.png";
    int directory_length = strlen(screenshot_directory);
    
    unsigned char screenshot_path[100];
    
    screenshot_path[0] = '\0';
    strncat(screenshot_path, home, home_length + 1);
    strncat(screenshot_path, screenshot_directory, directory_length + 1);

    int df_1[2];
    if (pipe(df_1) == -1) { return 1; }
    int df_2[2];
    if (pipe(df_2) == -1) { return 1; }
    
    int id_1 = fork();
    if (id_1 == -1) { return 1; }

    if (id_1 == 0)
    {
        close(df_2[0]);
        close(df_2[1]);
        close(df_1[0]);
        dup2(df_1[1], STDOUT_FILENO);
        close(df_1[1]);

        execlp("echo", "echo", "-e", "Full\nSelect\nWindow", NULL);
    }

    int id_2 = fork();
    if (id_2 == -1) { return 1; }

    if (id_2 == 0)
    {
        close(df_1[1]);
        dup2(df_1[0], STDIN_FILENO);
        close(df_1[0]);
        close(df_2[0]);
        dup2(df_2[1], STDOUT_FILENO);
        close(df_2[1]);

        execlp("dmenu", "dmenu", "-p", "Take screenshot" "-i", NULL);
    }

    int id_3 = fork();
    if (id_3 == -1) { return 1; }

    if (id_3 == 0)
    {
        close(df_1[0]);
        close(df_1[1]);
        close(df_2[1]);
        dup2(df_2[0], STDIN_FILENO);
        close(df_2[0]);

        unsigned char buffer[20];
        read(STDIN_FILENO, buffer, sizeof(buffer));

        for (int i = 0; i < 20; i++)
            if (buffer[i] == '\n')
            {
                buffer[i] = '\0';
                break;
            }

        if (strncmp(buffer, "Full", 1) == 0)
            execlp("scrot", "scrot", screenshot_path, NULL);
        else if (strncmp(buffer, "Select", 1) == 0)
            execlp("scrot", "scrot", screenshot_path, "--select", "--line", "mode=edge", NULL);
        else
            execlp("scrot", "scrot", screenshot_path, "--focused", "--border", NULL);
    }

    close(df_1[0]);
    close(df_1[1]);
    close(df_2[0]);
    close(df_2[1]);

    waitpid(id_1, NULL, 0);
    waitpid(id_2, NULL, 0);
    waitpid(id_3, NULL, 0);
    
    return 0;
}
