/* $Id$ */

/* 
 * Cookie support written by Dan McMahill borrowing heavily from 
 * Thomas Boutell's cgic 
 */


#ifndef __COOKIE_H__
#define __COOKIE_H__

#include <stdio.h>

#include "cgic.h"


/* Possible return codes from the cgiCookie family of functions (see below). */

typedef enum {
	cgiCookieSuccess,
	cgiCookieTruncated,
	cgiCookieBadType,
	cgiCookieEmpty,
	cgiCookieNotFound,
	cgiCookieConstrained,
	cgiCookieNoSuchChoice,
	cgiCookieMemory
} cgiCookieResultType;

void cookieFreeResources(void);
cgiParseResultType cgiParseCookieInput(void);

/* 
 * These functions are used to retrieve form data.  They closely
 * mirror the cgiForm*() functions of similar names. See
 * cgic.html for documentation. 
 */

cgiCookieResultType cgiCookieString(char *name, char *result, int max);


cgiCookieResultType cgiCookieStringNoNewlines(char *name, 
					      char *result, 
					      int max);


cgiCookieResultType cgiCookieStringSpaceNeeded(char *name, int *length);


cgiCookieResultType cgiCookieStringMultiple(char *name, 
					    char ***ptrToStringArray);

cgiCookieResultType cgiCookieInteger(char *name,
				     int *result,
				     int defaultV);

cgiCookieResultType cgiCookieIntegerBounded(char *name, 
					    int *result,
					    int min, 
					    int max,
					    int defaultV);

cgiCookieResultType cgiCookieDouble(char *name,
				    double *result,
				    double defaultV);


cgiCookieResultType cgiCookieDoubleBounded(char *name,
					   double *result,
					   double min,
					   double max,
					   double defaultV);

typedef struct {
  char *NAME;
  char *VALUE;

  char *Comment;
  char *Domain;
  char *MaxAge;
  char *Path;
  char *Secure;
  char *Version;
} cgiCookieType;

cgiCookieType *cgiCookie_new(const char *NAME, const char *VALUE);
void cgiCookie_free(cgiCookieType *cookie);
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

void cgiHeaderSetCookie(cgiCookieType *cookie);


#endif /* __COOKIE_H__ */

