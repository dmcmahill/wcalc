/* $Id: cookie.h,v 1.2 2002/02/20 01:10:10 dan Exp $ */

/* 
 * Cookie support written by Dan McMahill borrowing heavily from 
 * Thomas Boutell's cgic 
 */


#ifndef __COOKIE_H__
#define __COOKIE_H__

#include <stdio.h>

#include "cgic.h"

/* One cookie, consisting of an attribute-value pair. */

typedef struct cgiCookieEntryStruct {
  char *attr;
  char *value;
  struct cgiCookieEntryStruct *next;
} cgiCookieEntry;

/* The first form entry. */
extern cgiCookieEntry *cgiCookieEntryFirst;


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

cgiCookieResultType cgiCookieString(const char *name, char *result, int max);


cgiCookieResultType cgiCookieStringNoNewlines(const char *name, 
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

typedef enum {
	cgiCookieInsecure,
	cgiCookieSecure,
} cgiCookieSecureType;

typedef struct {
  char *NAME;
  char *VALUE;

  char *Domain;
  char *Expires;
  char *Path;
  cgiCookieSecureType   Secure;

} cgiCookieType;

void cgiCookiePrintAll(FILE *fp);

cgiCookieType *cgiCookie_new(const char *NAME, const char *VALUE);
void cgiCookie_free(cgiCookieType *cookie);
void cgiCookie_Domain_set(cgiCookieType *cookie, char *val);
void cgiCookie_Expires_set(cgiCookieType *cookie, char *val);
void cgiCookie_MaxAge_set(cgiCookieType *cookie, long int secs);
#define cgiCookie_MaxAge_max(cookie) (cgiCookie_MaxAge_set((cookie),(365*24*60*60)))

void cgiCookie_Path_set(cgiCookieType *cookie, char *val);
void cgiCookie_Secure_set(cgiCookieType *cookie, cgiCookieSecureType secure);

void cgiHeaderSetCookie(cgiCookieType *cookie);


#endif /* __COOKIE_H__ */

