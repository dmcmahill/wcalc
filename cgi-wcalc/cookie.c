/* $Id$ */

/* 
 * Cookie support written by Dan McMahill borrowing heavily from 
 * Thomas Boutell's cgic 
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#ifndef NO_UNISTD
#include <unistd.h>
#endif /* NO_UNISTD */

#include "cgic.h"
#include "cookie.h"

#define cgiStrEq(a, b) (!strcmp((a), (b)))

/* these are in cgic.c */
extern char *cgiCookie;
extern FILE *cgiIn;
extern FILE *cgiOut;

/* One form entry, consisting of an attribute-value pair. */

typedef struct cgiCookieEntryStruct {
  char *attr;
  char *value;
  struct cgiCookieEntryStruct *next;
} cgiCookieEntry;

/* The first form entry. */
static cgiCookieEntry *cgiCookieEntryFirst;

static void cgiSetupConstants();
static int cgiStrEqNc(char *s1, char *s2);


typedef enum {
  cgiEscapeRest,
  cgiEscapeFirst,
  cgiEscapeSecond
} cgiEscapeState;

typedef enum {
  cgiUnescapeSuccess,
  cgiUnescapeMemory
} cgiUnescapeResultType;

static cgiUnescapeResultType cgiUnescapeChars(char **sp, char *cp, int len);

cgiParseResultType cgiParseCookieInput(void) 
{
  /* Scan for pairs, unescaping and storing them as they are
     found. */
  int length;
  int pos = 0;
  cgiCookieEntry *n;
  cgiCookieEntry *l = 0;

  length = strlen(cgiCookie);

  while (pos != length) {
    int foundEq = 0;
    int foundAmp = 0;
    int start = pos;
    int len = 0;
    char *attr;
    char *value;
    while (pos != length) {
      if (cgiCookie[pos] == '=') {
	foundEq = 1;
	pos++;
	break;
      }
      pos++;
      len++;
    }
    if (!foundEq) {
      break;
    }
    if (cgiUnescapeChars(&attr, cgiCookie+start, len)
	!= cgiUnescapeSuccess) {
      return cgiParseMemory;
    }	
    start = pos;
    len = 0;
    while (pos != length) {
      if (cgiCookie[pos] == '&') {
	foundAmp = 1;
	pos++;
	break;
      }
      pos++;
      len++;
    }
    /* The last pair probably won't be followed by a &, but
       that's fine, so check for that after accepting it */
    if (cgiUnescapeChars(&value, cgiCookie+start, len)
	!= cgiUnescapeSuccess) {
      return cgiParseMemory;
    }	
    /* OK, we have a new pair, add it to the list. */
    n = (cgiCookieEntry *) malloc(sizeof(cgiCookieEntry));	
    if (!n) {
      return cgiParseMemory;
    }
    n->attr = attr;
    n->value = value;
    n->next = 0;
    if (!l) {
      cgiCookieEntryFirst = n;
    } else {
      l->next = n;
    }
    l = n;
    if (!foundAmp) {
      break;
    }			
  }
  return cgiParseSuccess;
}

static int cgiHexValue[256];

cgiUnescapeResultType cgiUnescapeChars(char **sp, char *cp, int len) {
  char *s;
  cgiEscapeState escapeState = cgiEscapeRest;
  int escapedValue = 0;
  int srcPos = 0;
  int dstPos = 0;
  s = (char *) malloc(len + 1);
  if (!s) {
    return cgiUnescapeMemory;
  }
  while (srcPos < len) {
    int ch = cp[srcPos];
    switch (escapeState) {
    case cgiEscapeRest:
      if (ch == '%') {
	escapeState = cgiEscapeFirst;
      } else if (ch == '+') {
	s[dstPos++] = ' ';
      } else {
	s[dstPos++] = ch;	
      }
      break;
    case cgiEscapeFirst:
      escapedValue = cgiHexValue[ch] << 4;	
      escapeState = cgiEscapeSecond;
      break;
    case cgiEscapeSecond:
      escapedValue += cgiHexValue[ch];
      s[dstPos++] = escapedValue;
      escapeState = cgiEscapeRest;
      break;
    }
    srcPos++;
  }
  s[dstPos] = '\0';
  *sp = s;
  return cgiUnescapeSuccess;
}		
	
static void cgiSetupConstants() {
  int i;
  for (i=0; (i < 256); i++) {
    cgiHexValue[i] = 0;
  }
  cgiHexValue['0'] = 0;	
  cgiHexValue['1'] = 1;	
  cgiHexValue['2'] = 2;	
  cgiHexValue['3'] = 3;	
  cgiHexValue['4'] = 4;	
  cgiHexValue['5'] = 5;	
  cgiHexValue['6'] = 6;	
  cgiHexValue['7'] = 7;	
  cgiHexValue['8'] = 8;	
  cgiHexValue['9'] = 9;
  cgiHexValue['A'] = 10;
  cgiHexValue['B'] = 11;
  cgiHexValue['C'] = 12;
  cgiHexValue['D'] = 13;
  cgiHexValue['E'] = 14;
  cgiHexValue['F'] = 15;
  cgiHexValue['a'] = 10;
  cgiHexValue['b'] = 11;
  cgiHexValue['c'] = 12;
  cgiHexValue['d'] = 13;
  cgiHexValue['e'] = 14;
  cgiHexValue['f'] = 15;
}

void cookieFreeResources(void) 
{
  cgiCookieEntry *c = cgiCookieEntryFirst;
  cgiCookieEntry *n;
  while (c) {
    n = c->next;
    free(c->attr);
    free(c->value);
    free(c);
    c = n;
  }
}

static cgiCookieResultType cgiCookieEntryString(
						cgiCookieEntry *e, char *result, int max, int newlines);

static cgiCookieEntry *cgiCookieEntryFindFirst(char *name);
static cgiCookieEntry *cgiCookieEntryFindNext();

cgiCookieResultType cgiCookieString(
				    char *name, char *result, int max) {
  cgiCookieEntry *e;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    strcpy(result, "");
    return cgiCookieNotFound;
  }
  return cgiCookieEntryString(e, result, max, 1);
}

cgiCookieResultType cgiCookieStringNoNewlines(
					      char *name, char *result, int max) {
  cgiCookieEntry *e;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    strcpy(result, "");
    return cgiCookieNotFound;
  }
  return cgiCookieEntryString(e, result, max, 0);
}

cgiCookieResultType cgiCookieStringMultiple(
					    char *name, char ***result) {
  char **stringArray;
  cgiCookieEntry *e;
  int i;
  int total = 0;
  /* Make two passes. One would be more efficient, but this
		function is not commonly used. The select menu and
		radio box functions are faster. */
  e = cgiCookieEntryFindFirst(name);
  if (e != 0) {
    do {
      total++;
    } while ((e = cgiCookieEntryFindNext()) != 0); 
  }
  stringArray = (char **) malloc(sizeof(char *) * (total + 1));
  if (!stringArray) {
    *result = 0;
    return cgiCookieMemory;
  }
  /* initialize all entries to null; the last will stay that way */
  for (i=0; (i <= total); i++) {
    stringArray[i] = 0;
  }
  /* Now go get the entries */
  e = cgiCookieEntryFindFirst(name);
#ifdef CGICDEBUG
  CGICDEBUGSTART
    fprintf(dout, "StringMultiple Beginning\n");
  CGICDEBUGEND
#endif /* CGICDEBUG */
    if (e) {
      i = 0;
      do {
	int max = strlen(e->value) + 1;
	stringArray[i] = (char *) malloc(max);
	if (stringArray[i] == 0) {
	  /* Memory problems */
	  cgiStringArrayFree(stringArray);
	  *result = 0;
	  return cgiCookieMemory;
	}	
	strcpy(stringArray[i], e->value);
	cgiCookieEntryString(e, stringArray[i], max, 1);
	i++;
      } while ((e = cgiCookieEntryFindNext()) != 0); 
      *result = stringArray;
#ifdef CGICDEBUG
      CGICDEBUGSTART
	fprintf(dout, "StringMultiple Succeeding\n");
      CGICDEBUGEND
#endif /* CGICDEBUG */
	return cgiCookieSuccess;
    } else {
      *result = stringArray;
#ifdef CGICDEBUG
      CGICDEBUGSTART
	fprintf(dout, "StringMultiple found nothing\n");
      CGICDEBUGEND
#endif /* CGICDEBUG */
	return cgiCookieNotFound;
    }	
}

cgiCookieResultType cgiCookieStringSpaceNeeded(
					       char *name, int *result) {
  cgiCookieEntry *e;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    *result = 1;
    return cgiCookieNotFound; 
  }
  *result = strlen(e->value) + 1;
  return cgiCookieSuccess;
}

static cgiCookieResultType cgiCookieEntryString(
						cgiCookieEntry *e, char *result, int max, int newlines) {
  char *dp, *sp;
  int truncated = 0;
  int len = 0;
  int avail = max-1;
  int crCount = 0;
  int lfCount = 0;	
  dp = result;
  sp = e->value;	
  while (1) {
    int ch;
    /* 1.07: don't check for available space now.
       We check for it immediately before adding
       an actual character. 1.06 handled the
       trailing null of the source string improperly,
       resulting in a cgiCookieTruncated error. */
    ch = *sp;
    /* Fix the CR/LF, LF, CR nightmare: watch for
       consecutive bursts of CRs and LFs in whatever
       pattern, then actually output the larger number 
       of LFs. Consistently sane, yet it still allows
       consecutive blank lines when the user
       actually intends them. */
    if ((ch == 13) || (ch == 10)) {
      if (ch == 13) {
	crCount++;
      } else {
	lfCount++;
      }	
    } else {
      if (crCount || lfCount) {
	int lfsAdd = crCount;
	if (lfCount > crCount) {
	  lfsAdd = lfCount;
	}
	/* Stomp all newlines if desired */
	if (!newlines) {
	  lfsAdd = 0;
	}
	while (lfsAdd) {
	  if (len >= avail) {
	    truncated = 1;
	    break;
	  }
	  *dp = 10;
	  dp++;
	  lfsAdd--;
	  len++;		
	}
	crCount = 0;
	lfCount = 0;
      }
      if (ch == '\0') {
	/* The end of the source string */
	break;				
      }	
      /* 1.06: check available space before adding
	 the character, because a previously added
	 LF may have brought us to the limit */
      if (len >= avail) {
	truncated = 1;
	break;
      }
      *dp = ch;
      dp++;
      len++;
    }
    sp++;	
  }	
  *dp = '\0';
  if (truncated) {
    return cgiCookieTruncated;
  } else if (!len) {
    return cgiCookieEmpty;
  } else {
    return cgiCookieSuccess;
  }
}

static int cgiFirstNonspaceChar(char *s);

cgiCookieResultType cgiCookieInteger(
				     char *name, int *result, int defaultV) {
  cgiCookieEntry *e;
  int ch;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    *result = defaultV;
    return cgiCookieNotFound; 
  }	
  if (!strlen(e->value)) {
    *result = defaultV;
    return cgiCookieEmpty;
  }
  ch = cgiFirstNonspaceChar(e->value);
  if (!(isdigit(ch)) && (ch != '-') && (ch != '+')) {
    *result = defaultV;
    return cgiCookieBadType;
  } else {
    *result = atoi(e->value);
    return cgiCookieSuccess;
  }
}

cgiCookieResultType cgiCookieIntegerBounded(
					    char *name, int *result, int min, int max, int defaultV) {
  cgiCookieResultType error = cgiCookieInteger(name, result, defaultV);
  if (error != cgiCookieSuccess) {
    return error;
  }
  if (*result < min) {
    *result = min;
    return cgiCookieConstrained;
  } 
  if (*result > max) {
    *result = max;
    return cgiCookieConstrained;
  } 
  return cgiCookieSuccess;
}

cgiCookieResultType cgiCookieDouble(
				    char *name, double *result, double defaultV) {
  cgiCookieEntry *e;
  int ch;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    *result = defaultV;
    return cgiCookieNotFound; 
  }	
  if (!strlen(e->value)) {
    *result = defaultV;
    return cgiCookieEmpty;
  } 
  ch = cgiFirstNonspaceChar(e->value);
  if (!(isdigit(ch)) && (ch != '.') && (ch != '-') && (ch != '+')) {
    *result = defaultV;
    return cgiCookieBadType;
  } else {
    *result = atof(e->value);
    return cgiCookieSuccess;
  }
}

cgiCookieResultType cgiCookieDoubleBounded(
					   char *name, double *result, double min, double max, double defaultV) {
  cgiCookieResultType error = cgiCookieDouble(name, result, defaultV);
  if (error != cgiCookieSuccess) {
    return error;
  }
  if (*result < min) {
    *result = min;
    return cgiCookieConstrained;
  } 
  if (*result > max) {
    *result = max;
    return cgiCookieConstrained;
  } 
  return cgiCookieSuccess;
}


static void cgiCookieEscape(FILE *fp,char *str) {
  long int i;
  
  for (i=0; i<strlen(str); i++) {
    if ( (('0' <= str[i]) && ( str[i] <= '9')) ||
	 (('a' <= str[i]) && ( str[i] <= 'z')) ||
	 (('A' <= str[i]) && ( str[i] <= 'Z')) ||
	 str[i] == '-') {
      fprintf(fp,"%c",str[i]);
    }
    else {
      /* 
       * XXX is this portable?  Problems on non-ascii systems?
       * are there such things?  I guess for some non-english apps
       * there must be...
       */
      fprintf(fp,"%%%2X",str[i]);
    }
    
  }
}
 
static void cgiCookieUnEscape(char *str) {
  long int i;
  char tmp[3];
  char ch;

  tmp[2] = '\0';
  for (i=0; i<strlen(str); i++) {
    if (str[i] != '%')
      printf("%c",str[i]);
    else {
      if (strlen(&str[i]) < 3 )
	return;

      i++;
      tmp[0] = tolower(str[i]);
      i++;
      tmp[1] = tolower(str[i]);

      if ( ('0' <= tmp[0]) && (tmp[0] <= '9') ) {
	ch = 16*(tmp[0]-'0');
      }
      else if ( ('a' <= tmp[0]) && (tmp[0] <= 'f') ) {
	ch = 16*(tmp[0]-'a' + 10);
      }
      else {
	return;
      }

      if ( ('0' <= tmp[1]) && (tmp[1] <= '9') ) {
	ch = ch + (tmp[1]-'0');
      }
      else if ( ('a' <= tmp[1]) && (tmp[1] <= 'f') ) {
	ch = ch + (tmp[1]-'a' + 10);
      }
      else {
	return;
      }
      printf("%c",ch);
    }
    
  }
}
 
void cgiHeaderSetCookie(cgiCookieType *cookie) {

  if (cookie->NAME == NULL) {
    return;
  }
  cgiCookieUnEscape("coax%2Ecgi=0%2E1%20Farads%2Fm; ");
  printf("\n");
  fprintf(cgiOut, "Set-Cookie: ");
  cgiCookieEscape(cgiOut,cookie->NAME);
  fprintf(cgiOut,"=");
  cgiCookieEscape(cgiOut,cookie->VALUE);
  
  if (cookie->Comment != NULL) {
    fprintf(cgiOut, "; Comment=");
    cgiCookieEscape(cgiOut,cookie->Comment);
  }
  if (cookie->Domain != NULL) {
    fprintf(cgiOut, "; Domain=");
    cgiCookieEscape(cgiOut,cookie->Domain);
  }
  if (cookie->MaxAge != NULL) {
    fprintf(cgiOut, "; Max-Age=");
    cgiCookieEscape(cgiOut,cookie->MaxAge);
  }
  if (cookie->Path != NULL) {
    fprintf(cgiOut, "; Path=");
    cgiCookieEscape(cgiOut,cookie->Path);
  }
  if (cookie->Secure != NULL) {
    fprintf(cgiOut, "; Secure=");
    cgiCookieEscape(cgiOut,cookie->Secure);
  }
  if (cookie->Version != NULL) {
    fprintf(cgiOut, "; Version=");
    cgiCookieEscape(cgiOut,cookie->Version);
  }
  
  fprintf(cgiOut,"%c",10);
}


static int cgiStrEqNc(char *s1, char *s2) {
  while(1) {
    if (!(*s1)) {
      if (!(*s2)) {
	return 1;
      } else {
	return 0;
      }
    } else if (!(*s2)) {
      return 0;
    }
    if (isalpha((int) *s1)) {
      if (tolower(*s1) != tolower(*s2)) {
	return 0;
      }
    } else if ((*s1) != (*s2)) {
      return 0;
    }
    s1++;
    s2++;
  }
}

static char *cgiFindTarget = 0;
static cgiCookieEntry *cgiFindPos = 0;

static cgiCookieEntry *cgiCookieEntryFindFirst(char *name) {
  cgiFindTarget = name;
  cgiFindPos = cgiCookieEntryFirst;
  return cgiCookieEntryFindNext();
}

static cgiCookieEntry *cgiCookieEntryFindNext() {
  while (cgiFindPos) {
    cgiCookieEntry *c = cgiFindPos;
    cgiFindPos = c->next;
    if (!strcmp(c -> attr, cgiFindTarget)) {
      return c;
    }
  }
  return 0;
}

static int cgiFirstNonspaceChar(char *s) {
  int len = strspn(s, " \n\r\t");
  return s[len];
}


/* Functions for the manipulation of cgiCookieType's */
cgiCookieType *cgiCookie_new(const char *NAME, const char *VALUE) {
  cgiCookieType *c;

  if ( (c = malloc(sizeof(cgiCookieType))) == NULL ) {
    exit(1);
  }

  c->NAME  = strdup(NAME);
  c->VALUE = strdup(VALUE);

  c->Comment = NULL;
  c->Domain  = NULL;
  c->MaxAge  = NULL;
  c->Path    = NULL;
  c->Secure  = NULL;
  c->Version = strdup("1");

  return c;
}

void cgiCookie_free(cgiCookieType *cookie) {

  if (cookie->NAME == NULL)
    free(cookie->NAME);
  
  if (cookie->VALUE == NULL)
    free(cookie->VALUE);
  
  if (cookie->Comment != NULL)
    free(cookie->Comment);

  if (cookie->Domain != NULL)
    free(cookie->Domain);

  if (cookie->MaxAge != NULL)
    free(cookie->MaxAge);

  if (cookie->Path != NULL)
    free(cookie->Path);

  if (cookie->Secure != NULL)
    free(cookie->Secure);

  if (cookie->Version != NULL)
    free(cookie->Version);
  
}

char *cgiCookie_Comment_get(cgiCookieType *cookie);
void cgiCookie_Comment_set(cgiCookieType *cookie, char *val);
char *cgiCookie_Domain_get(cgiCookieType *cookie);
void cgiCookie_Domain_set(cgiCookieType *cookie, char *val);
char *cgiCookie_MaxAge_get(cgiCookieType *cookie);
void cgiCookie_MaxAge_set(cgiCookieType *cookie, char *val);
char *cgiCookie_Path_get(cgiCookieType *cookie);
void cgiCookie_Path_set(cgiCookieType *cookie, char *val);
char *cgiCookie_Secure_get(cgiCookieType *cookie);
void cgiCookie_Secure_set(cgiCookieType *cookie, char *val);
char *cgiCookie_Version_get(cgiCookieType *cookie);
void cgiCookie_Version_set(cgiCookieType *cookie, char *val);


