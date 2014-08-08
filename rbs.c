#include "rbs.h"

/* Rule based stemmer. */
int wstrlen(wchar_t* word)
{
	/*
	wchar_t *p ;
	for (p = word; *p; ++p) ;
	return p - word - 1 ;
	*/
	return wcslen(word);
}

bool isBengaliSwaraBarna(wchar_t a)
{
	static wchar_t swaraBarnas[] = { bn_AA, bn_E, bn_I, bn_II, bn_U, bn_UU } ;
	int i ;

	for (i = 0; i < sizeof(swaraBarnas)/sizeof(*swaraBarnas); i++) {
		if ( a == *(swaraBarnas + i) )
			return true ;
	}

	return false ; 
}

/* A common suffix candidate is a sequence of vowels and bn_y. */
static bool isBnCommonSuffix(wchar_t a) {
	if (a >= bn_AA && a <= bn_AU)
		return true;
	if (a >= bn_aa && a <= bn_au)
		return true;
    return a == bn_y? true:false;
}

bool isBengaliByanjanBarna(wchar_t a)
{
	return a >= bn_k && a <= bn_y ;
}

// Strip off suffixes "gulo", "guli", "gulote" "gulite"
void stripPluralSuffixes(wchar_t* word, int* len)
{
	if (*len <= 6)
		return ;

	if ( word[*len-1] == bn_E && word[*len-2] == bn_t ) {
		word[*len - 2] = 0 ;
		*len -= 2 ;
	}

	if (*len <= 6)
		return ;

	if ( word[*len-4] == bn_g && word[*len-3] == bn_U && word[*len-2] == bn_l &&
			(word[*len-1] == bn_O || word[*len-1] == bn_I) ) {
		word[*len - 4] = 0 ;
		*len -= 4 ;
	}
}

// Strip off suffixes like "rai", "tuku", "shil" "ta" etc.
bool stripCommonSuffixes(wchar_t* word, int* len, bool i_removed)
{
	int newlen = *len ;

	do {
		if (*len <= 4)
			break ;

		// Remove 'tta' or 'ta' (only if it is not preceeded with a m or g)
		if ( word[*len-1] == bn_AA &&
			(word[*len-2] == bn_tt || (word[*len-2] == bn_t &&
			 word[*len-3] != bn_m && word[*len-3] != bn_g) ))   {
			word[*len - 2] = 0 ;
			*len -= 2 ;
		}

		if (*len <= 4)
			break ;

		// Remove 'ti' or 'tti'
		if ( word[*len-1] == bn_I &&
			word[*len-2] == bn_tt )  {
			word[*len - 2] = 0 ;
			*len -= 2 ;
		}

		if (*len <= 4)
			break ;

		// Remove "ra"  ("rai" has alreday been stemmed to "ra").
		if (word[*len-1] == bn_r) { 	
			word[*len - 1] = 0 ;
			*len -= 1 ;

			if (*len <= 4)
				break ;

			// Remove "-er"
			if (word[*len-1] == bn_E) {
				int pos = word[*len-2] == bn_d ? 2 : 1; // Remove '-der'
				word[*len - pos] = 0 ;
				*len -= pos ;
			}
		}

		if (*len <= 5)
			break ;

		// Remove ttai tai ttar or tar
		if ( (word[*len-1] == bn_y || word[*len-1] == bn_r) &&
			word[*len-2] == bn_AA &&
			(word[*len-3] == bn_tt || word[*len-3] == bn_t)) {
			word[*len - 3] = 0 ;
			*len -= 3 ;
		}
		else if ( (word[*len-1] == bn_r) && word[*len-2] == bn_I && word[*len-3] == bn_tt) {
			word[*len - 3] = 0 ;
			*len -= 3 ;
		}

		if (*len <= 5)
			break ;

		if ( word[*len-1] == bn_E && word[*len-2] == bn_k) {
			word[*len - 2] = 0 ;
			*len -= 2 ;
		}

		if (*len <= 5)
			break;

		// Remove 'shil'
		if (word[*len-1] == bn_l && word[*len-2] == bn_II && word[*len-3] == bn_sh) {
			word[*len - 3] = 0 ;
			*len -= 3 ;
		}

		if (*len <= 6)
			break;

		// Remove 'tuku'
		if (word[*len-1] == bn_U && word[*len-2] == bn_k && word[*len-3] == bn_U && word[*len-4] == bn_tt) {
			word[*len - 4] = 0 ;
			*len -= 4 ;
		}

		// Remove 'debi'
		if (*len <= 6)
			break;

		if (word[*len-1] == bn_II && word[*len-2] == bn_b && word[*len-3] == bn_E && word[*len-4] == bn_d) {
			word[*len - 4] = 0 ;
			*len -= 4 ;
		}

		// Remove 'babu'
		if (*len <= 6)
			break;

		if (word[*len-1] == bn_U && word[*len-2] == bn_b && word[*len-3] == bn_AA && word[*len-4] == bn_b) {
			word[*len - 4] = 0 ;
			*len -= 4 ;
		}

		// Remove 'bhai'
		if (*len <= 6 || !i_removed)
			break;

		if (word[*len-1] == bn_AA && word[*len-2] == bn_bh) {
			word[*len - 2] = 0 ;
			*len -= 2 ;
		}

		// Remove 'bhabe'
		if (*len <= 6)
			break;

		if (word[*len-1] == bn_b && word[*len-2] == bn_E && word[*len-3] == bn_AA && word[*len-4] == bn_bh) {
			word[*len - 4] = 0 ;
			*len -= 4 ;
		}


	}
	while (0) ;

	return newlen != *len;
}

bool suffixEndingByanjonBarna(wchar_t ch)
{
	return (ch == bn_d || ch == bn_k || ch == bn_tt || ch == bn_t || ch == bn_m);
}
 
wchar_t* stemWord(wchar_t* word, bool isAggressive)
{
	static wchar_t buff[BUFFSIZE] ;
	int len = wstrlen(word) ;
	int wordlen = len;
	bool i_removed ;
	wchar_t *p;

	wcscpy(buff, word) ;

	if (!isAggressive) {
		if (len <= 3)
			return buff;
	}

	// Remove trailing okhyor "i" and "o"
	if (buff[len-1] == bn_i || buff[len-1] == bn_o) {
		strip(buff, len, 1) ;
		i_removed = true;
	}

	while (stripCommonSuffixes(buff, &len, i_removed)) {
		i_removed = false;
	}

	stripPluralSuffixes(buff, &len);

	if (isAggressive) {
	    p = buff+len-1;
    	while (isBnCommonSuffix(*p)) {
	    	p--;
	    }
		if (p-word+1 >= 2)
			*(p+1) = 0;
	}

	if (buff[len-1] == bn_E)
		strip(buff, len, 1);

	return buff ;
}

