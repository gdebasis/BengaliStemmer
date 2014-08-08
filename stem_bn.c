#include "rbs.h"

int main(int argc, char** argv)
{
	FILE   *fp, *ofile ;
	char* token;
	static wchar_t buff[BUFFSIZE];
	wchar_t *stem;
	const char* delims = " \t\n";
	static char line[BUFFSIZE<<1];
	int    len;
	int    aggr = 0;

	if (argc < 3) {
		fprintf(stderr, "usage: rbs <input query file> <output query file> [aggressive (default: 0)]\n") ;
		return 1 ;
	}
	if (argc >= 4) {
		if (!sscanf(argv[3], "%d", &aggr)) {
			fprintf(stderr, "usage: rbs <input query file> <output query file> [aggressive (default: 0)]\n") ;
			return 1 ;
		}
	}
	
	if (NULL == setlocale(LC_ALL, "bn_IN.utf8")) {
		printf("Couldn't set locale\n");
		setlocale(LC_ALL, "en_US.utf8");
	}

	fp = fopen(argv[1], "r") ;
	if (!fp) {
		fprintf(stderr, "Couldn't open file %s for reading\n", argv[1]) ;
		return 1 ;
	}

	ofile = fopen(argv[2], "w") ;
	if (!ofile) {
		fprintf(stderr, "Couldn't open file %s for writing\n", argv[2]) ;
		return 1 ;
	}

	while ( fgetws(buff, sizeof(buff) / sizeof(buff[0]), fp) ) {
		len = wcslen(buff);
		buff[len-1] = 0;
		stem = stemWord(buff, aggr);
		fprintf(ofile, "%ls\t%ls\n", buff, stem);
	}

	fclose(ofile);
	fclose(fp);

	return 0;
}
