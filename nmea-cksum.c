#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMEA_EPILOGUE_LENGTH	5 /* -> strlen("*xx\r\n") */

int main(int ac, char **av)
{
	uint8_t cksum = 0;
	char *s;
	char *res;
	size_t cur = 0;
	uint8_t nodollar = 1;

	if (ac < 2 || strlen(av[1]) == 0) {
		fprintf(stderr, "usage: %s NMEA_STRING\nexiting...", av[0]);
		return EXIT_FAILURE;
	}

	s = av[1];

	if (s[cur] == '$') {
		cur++; /* discards the '$' */
		nodollar = 0;
	}

	for (; s[cur]; cur++) {
		if (s[cur] == '*') {
			s[cur] = '\0';
			break;
		}
		cksum ^= s[cur];
	}

	res = (char *) malloc((cur + 1 + NMEA_EPILOGUE_LENGTH) * sizeof(char));
	if (!res) {
		fprintf(stderr, "memory exhausted\nexiting...\n");
		return EXIT_FAILURE;
	}

	sprintf(res, "%s*%02X", s, cksum);

	if (nodollar)
		putchar('$');

	printf("%s\n", res);
	free(res);

	return EXIT_SUCCESS;
}
