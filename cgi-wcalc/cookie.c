/* $Id: cookie.c,v 1.6 2004/08/13 04:43:55 dan Exp $ */

/* 
 * Cookie support written by Dan McMahill borrowing heavily from 
 * Thomas Boutell's cgic 
 */

/*
 * See http://www.netscape.com/newsref/std/cookie_spec.html
 * for infomation on cookies.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <ctype.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <time.h>
#ifndef NO_UNISTD
#include <unistd.h>
#endif /* NO_UNISTD */

#include "cgic.h"
#include "cookie.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define cgiStrEq(a, b) (!strcmp((a), (b)))

/* these are in cgic.c */
extern char *cgiCookie;
extern FILE *cgiIn;
extern FILE *cgiOut;
extern int cgiHexValue[256];


/* The first form entry. */
cgiCookieEntry *cgiCookieEntryFirst;

/*
static int cgiStrEqNc(char *s1, char *s2);
*/

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
     found.  Note: cookies are separated  by ';'*/
  int length;
  int pos = 0;
  cgiCookieEntry *n;
  cgiCookieEntry *l = 0;

  length = strlen(cgiCookie);

  while (pos != length) {
    int foundEq = 0;
    int foundSem = 0;
    int start = pos;
    int len = 0;
    char *attr;
    char *value;

    /* skip over leading whitespace in the attribute name */
    while (pos != length) {
      if (cgiCookie[pos] != ' ') {
	break;
      }
      pos++;
      start++;
    }
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
      if (cgiCookie[pos] == ';') {
	foundSem = 1;
	pos++;
	break;
      }
      pos++;
      len++;
    }
    /* The last pair probably won't be followed by a ;, but
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
    n->next = NULL;
    if (l==NULL) {
      cgiCookieEntryFirst = n;
    } else {
      l->next = n;
    }
    l = n;
    if (!foundSem) {
      break;
    }			
  }

  return cgiParseSuccess;
}


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

static cgiCookieResultType cgiCookieEntryString(cgiCookieEntry *e, 
						char *result, 
						int max, 
						int newlines);

static cgiCookieEntry *cgiCookieEntryFindFirst(const char *name);
static cgiCookieEntry *cgiCookieEntryFindNext();

cgiCookieResultType cgiCookieString(const char *name, char *result, int max) 
{
  cgiCookieEntry *e;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    strcpy(result, "");
    return cgiCookieNotFound;
  }
  return cgiCookieEntryString(e, result, max, 1);
}

cgiCookieResultType cgiCookieStringNoNewlines(const char *name, 
					      char *result, 
					      int max) 
{
  cgiCookieEntry *e;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    strcpy(result, "");
    return cgiCookieNotFound;
  }
  return cgiCookieEntryString(e, result, max, 0);
}

cgiCookieResultType cgiCookieStringMultiple(char *name, 
					    char ***result) 
{
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
    fprintf(dout, "CookieStringMultiple Beginning\n");
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
	fprintf(dout, "CookieStringMultiple Succeeding\n");
      CGICDEBUGEND
#endif /* CGICDEBUG */
	return cgiCookieSuccess;
    } else {
      *result = stringArray;
#ifdef CGICDEBUG
      CGICDEBUGSTART
	fprintf(dout, "CookieStringMultiple found nothing\n");
      CGICDEBUGEND
#endif /* CGICDEBUG */
	return cgiCookieNotFound;
    }	
}

cgiCookieResultType cgiCookieqStringSpaceNeeded(char *name, 
						int *result) 
{
  cgiCookieEntry *e;
  e = cgiCookieEntryFindFirst(name);
  if (!e) {
    *result = 1;
    return cgiCookieNotFound; 
  }
  *result = strlen(e->value) + 1;
  return cgiCookieSuccess;
}

static cgiCookieResultType cgiCookieEntryString(cgiCookieEntry *e, 
						char *result, 
						int max, 
						int newlines) 
{
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
    /* don't check for available space now.
       We check for it immediately before adding
       an actual character. */
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
      /* check available space before adding
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

cgiCookieResultType cgiCookieInteger(char *name, 
				     int *result, 
				     int defaultV) 
{
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

cgiCookieResultType cgiCookieIntegerBounded(char *name, 
					    int *result, 
					    int min, 
					    int max, 
					    int defaultV) 
{
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

cgiCookieResultType cgiCookieDouble(char *name, 
				    double *result, 
				    double defaultV) 
{
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

cgiCookieResultType cgiCookieDoubleBounded(char *name, 
					   double *result, 
					   double min, 
					   double max, 
					   double defaultV) 
{
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


static void cgiCookieEscape(FILE *fp,char *str) 
{
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

#ifdef notdef
static void cgiCookieUnEscape(char *str) {
  long int i;
  int tmp[3];
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

      ch = (cgiHexValue[tmp[0]] << 4) + 
	cgiHexValue[tmp[1]];

      printf("%c",ch);
    }
    
  }
}
#endif

void cgiHeaderSetCookie(cgiCookieType *cookie) 
{

  if (cookie->NAME == NULL) {
    return;
  }

  fprintf(cgiOut, "Set-Cookie: ");
  cgiCookieEscape(cgiOut,cookie->NAME);
  fprintf(cgiOut,"=");
  cgiCookieEscape(cgiOut,cookie->VALUE);
  
  if (cookie->Domain != NULL) {
    fprintf(cgiOut, "; domain=");
    cgiCookieEscape(cgiOut,cookie->Domain);
  }
  if (cookie->Expires != NULL) {
    fprintf(cgiOut, "; expires=%s",cookie->Expires);
  }
  if (cookie->Path != NULL) {
    fprintf(cgiOut, "; path=");
    cgiCookieEscape(cgiOut,cookie->Path);
  }
  if (cookie->Secure == cgiCookieSecure) {
    fprintf(cgiOut, "; secure");
  }
  
  fprintf(cgiOut,"%c",10);
}

#ifdef notdef
static int cgiStrEqNc(char *s1, char *s2) 
{
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
#endif

static const char *cgiFindTarget = 0;
static cgiCookieEntry *cgiFindPos = 0;

static cgiCookieEntry *cgiCookieEntryFindFirst(const char *name) {
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

  c->Domain  = NULL;
  c->Expires = NULL;
  c->Path    = NULL;
  c->Secure  = cgiCookieInsecure;

  return c;
}

void cgiCookie_free(cgiCookieType *cookie) {

  if (cookie->NAME == NULL)
    free(cookie->NAME);
  
  if (cookie->VALUE == NULL)
    free(cookie->VALUE);
  
  if (cookie->Domain != NULL)
    free(cookie->Domain);

  if (cookie->Expires != NULL)
    free(cookie->Expires);

  if (cookie->Path != NULL)
    free(cookie->Path);


}

void cgiCookie_Domain_set(cgiCookieType *cookie, char *val)
{
  if (cookie == NULL)
    return ;

  cookie->Domain=strdup(val);
}

void cgiCookie_Expires_set(cgiCookieType *cookie, char *val)
{
  if (cookie == NULL)
    return ;

  cookie->Expires=strdup(val);
}

#define EXPIRES_LEN 29
/* set Expires to be 'secs' seconds in the future */
void cgiCookie_MaxAge_set(cgiCookieType *cookie, long int secs)
{
  time_t when;
  char *str;
  struct tm *t;
  int day, month;

  /*
   * these are the allowed days.  Note:  we want these, not the
   * versions set by the users current locale 
   */
  char *days[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  char *months[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
  "Aug", "Sep", "Oct", "Nov", "Dec"};

  if (cookie == NULL)
    return ;

  if ( (str=malloc((EXPIRES_LEN+1)*sizeof(char))) == NULL) {
    exit(1);
  }

  
  when = time(NULL) + secs;
  

  t=gmtime(&when);
  strftime(str, EXPIRES_LEN, "%w",t);
  day = atoi(str);

  strftime(str, EXPIRES_LEN, "%m",t);
  month = atoi(str) - 1;

  strftime(str, EXPIRES_LEN,"   , %d-%m -%Y %H:%M:%S GMT",t);
  str[0]=days[day][0];
  str[1]=days[day][1];
  str[2]=days[day][2];

  str[8]=months[month][0];
  str[9]=months[month][1];
  str[10]=months[month][2];

  cookie->Expires=str;

}

void cgiCookie_Path_set(cgiCookieType *cookie, char *val)
{
  if (cookie == NULL)
    return ;

  cookie->Path=strdup(val);
}

void cgiCookie_Secure_set(cgiCookieType *cookie, 
			  cgiCookieSecureType secure)
{
  if (cookie == NULL)
    return ;

  cookie->Secure=secure;

}

void cgiCookiePrintAll(FILE *fp)
{
    cgiCookieEntry *c = cgiCookieEntryFirst;

    while(c != NULL) {
      fprintf(fp,"cookie:  attr  = \"%s\"\n",c->attr);
      fprintf(fp,"         value = \"%s\"\n",c->value);
      c=c->next;
    }

}
