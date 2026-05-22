* Return: full path if found, NULL if not
 */
char *get_path(char *command)
{
        char *path, *path_copy, *token, *file_path;
        struct stat st;

        if (!command)
                return (NULL);
        /* If command contains '/', it's a direct path. Check it directly. */
        /* CHANGED: Used custom _strchr instead of standard strchr */
        if (_strchr(command, '/'))
        {
                if (stat(command, &st) == 0)
                        /* CHANGED: Used custom _strdup instead of standard strdup */
                        return (_strdup(command));
                return (NULL);
        }
        path = _getenv("PATH");
        if (!path || !*path)
                return (NULL);

        /* CHANGED: Used custom _strdup instead of standard strdup */
        path_copy = _strdup(path);
        if (!path_copy)
                return (NULL);

        token = strtok(path_copy, ":");
        while (token)
        {
                file_path = build_path(token, command);
                if (stat(file_path, &st) == 0)
                {
                        free(path_copy);
                        return (file_path);
                }
                free(file_path);
                token = strtok(NULL, ":");
        }
        free(path_copy);
        return (NULL);
}
