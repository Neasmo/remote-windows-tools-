je veuc ce code en python . redige le 


#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>

using namespace std;

int main()
{
    // Création des étiquettes et champs de saisie
    cout << "Nom de l'ordinateur : ";
    string computerName;
    cin >> computerName;
    cout << "Nom d'utilisateur : ";
    string username;
    cin >> username;
    cout << "Mot de passe : ";
    string password;
    cin >> password;
}
// Exécution de la commande "wmic useraccount get name" pour récupérer la liste des utilisateurs sur l'ordinateur distant
vector<string> command = { "runas", "/user:" + (computerName + "\\" + username), password, "wmic", "useraccount", "get", "name" };
pid_t pid1 = fork();
if (pid1 == 0)
{
    // Exécution de la commande dans le processus fils
    char* argv[command.size() + 1];
    for (int i = 0; i < command.size(); i++)
    {
        argv[i] = &command[i][0];
    }
    argv[command.size()] = NULL;
    execvp("runas", argv);
}
else
{
    // Attente de la fin de l'exécution de la commande dans le processus père
    int status;
    waitpid(pid1, &status, 0);
    if (status == 0)
    {
        // Lecture de la sortie de la commande
        vector<string> users;
        string line;
        while (getline(cin, line))
        {
            // Si l'utilisateur est "Public", "DefaultAccount" ou "DefaultAppPool", on passe à l'itération suivante
            if (line == "Public" || line == "DefaultAccount" || line == "DefaultAppPool")
            {
                continue;
            }
            users.push_back(line);
        }


    }
    // Pour chaque utilisateur, exécution de la commande "del /q /f /s %USERPROFILE%\AppData\Local\Temp\*.*" pour supprimer les fichiers temporaires de l'utilisateur
    for (string user : users)
    {
        vector<string> command = { "runas", "/user:" + computerName + "\\" + username, password, "cmd.exe", "/c", "del", "/q", "/f", "/s", "%USERPROFILE%\\AppData\\Local\\Temp\\*.*" };
        pid_t pid2 = fork();
        if (pid2 == 0)
        {
            // Exécution de la commande dans le processus fils
            char* argv[command.size() + 1];
            for (int i = 0; i < command.size(); i++)
            {
                argv[i] = &command[i][0];
            }
            argv[command.size()] = NULL;
            execvp("runas", argv);
        }
        else
        {
            // Attente de la fin de l'exécution de la commande dans le processus père
            int status;
            waitpid(pid2, &status, 0);
        }
    }