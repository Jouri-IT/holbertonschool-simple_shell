#include "main.h"

/**
 * _strcmp - Compares two strings.
 * @s1: First string
 * @s2: Second string
 * Return: 0 if identical, or difference between first mismatching chars
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

/**
 * _strchr - Locates a character in a string.
 * @s: The string to search
 * @c: The character to locate
 * Return: Pointer to first occurrence, or NULL if not found
 */
char *_strchr(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	if (c == '\0')
		return (s);
	return (NULL);
}

/**
 * _strdup - Duplicates a string in dynamically allocated memory.
 * @str: The string to duplicate
 * Return: Pointer to the allocated copy, or NULL on failure
 */
char *_strdup(char *str)
{
	char *dup;
	int len = 0, i;

	if (str == NULL)
		return (NULL);

	while (str[len])
		len++;

	dup = malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);

	for (i = 0; i <= len; i++)
		dup[i] = str[i];

	return (dup);
}

/**
 * _memcpy - Copies n bytes from src to dest.
 * @dest: Destination buffer
 * @src: Source buffer
 * @n: Number of bytes to copy
 * Return: Pointer to dest
 */
char *_memcpy(char *dest, char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		dest[i] = src[i];
	return (dest);
}
