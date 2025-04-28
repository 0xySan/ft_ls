#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
		return (1);
	}
	ft_ls_fake_locale(argv[1]);
	return (0);
}