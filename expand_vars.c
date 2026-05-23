#include "shell.h"

/**
 * expand_vars - Replaces $?, $$, $VAR in a token
 * @token: the token to expand
 * @exit_status: last exit status
 * @pid: shell PID
 *
 * Return: newly allocated expanded string
 */
char *expand_vars(char *token, int exit_status, pid_t pid)
{
	char result[1024];
	char tmp[32];
	int i = 0, j = 0;
	char *val;
	int vlen, k;

	if (!token)
		return (token);

	while (token[i] && j < 1023)
	{
		if (token[i] == '$' && token[i + 1])
		{
			if (token[i + 1] == '?')
			{
				sprintf(tmp, "%d", exit_status);
				k = 0;
				while (tmp[k] && j < 1023)
					result[j++] = tmp[k++];
				i += 2;
			}
			else if (token[i + 1] == '$')
			{
				sprintf(tmp, "%d", pid);
				k = 0;
				while (tmp[k] && j < 1023)
					result[j++] = tmp[k++];
				i += 2;
			}
			else if (token[i + 1] == ' ' || token[i + 1] == '\0')
			{
				result[j++] = token[i++];
			}
			else
			{
				int name_start = i + 1;
				int name_len = 0;
				char varname[256];

				while (token[name_start + name_len] &&
					token[name_start + name_len] != ' ' &&
					token[name_start + name_len] != '$')
					name_len++;
				if (name_len > 255)
					name_len = 255;
				k = 0;
				while (k < name_len)
				{
					varname[k] = token[name_start + k];
					k++;
				}
				varname[name_len] = '\0';
				val = _getenv(varname);
				if (val)
				{
					vlen = 0;
					while (val[vlen] && j < 1023)
						result[j++] = val[vlen++];
				}
				i += 1 + name_len;
			}
		}
		else
		{
			result[j++] = token[i++];
		}
	}
	result[j] = '\0';
	return (_strdup(result));
}
