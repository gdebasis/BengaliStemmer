#ifndef __RBS_H
#define __RBS_H

#include <stdio.h>
#include <locale.h>
#include <langinfo.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include "bn_unicode.h"

#define BUFFSIZE 1024

typedef enum { false, true } bool ;

int wstrlen(wchar_t* word);
bool isBengaliSwaraBarna(wchar_t a);
bool isBengaliByanjanBarna(wchar_t a);
void stripPluralSuffixes(wchar_t* word, int* len);
bool stripCommonSuffixes(wchar_t* word, int* len, bool i_removed);
bool suffixEndingByanjonBarna(wchar_t ch);
wchar_t* stemWord(wchar_t* word, bool);

#define strip(word, len, pos) 		\
	if (len - pos >= 1) { 			\
		word[len - pos] = 0; 		\
		len -= pos ;				\
	}

#endif
