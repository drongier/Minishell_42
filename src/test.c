#include "minishell.h"

void    test(void)
{
    printf("TEST\n");
}

int main() 
{
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Erreur à l'ouverture du fichier");
        return 1;
    }

    // Redirige la sortie standard vers le fichier
    dup(fd);
    test();
    printf("Ceci sera écrit dans le fichier au lieu du terminal.\n");

    close(fd);
    return 0;
}