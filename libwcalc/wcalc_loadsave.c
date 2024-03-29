
/*
 * Copyright (C) 2001, 2002, 2004, 2005, 2006, 2008 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

/* #define DEBUG */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <sys/types.h>
#include <time.h>
 
#include "alert.h"
#include "units.h"
#include "wcalc_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif


/*
 * Prints out the required header to a file.  
 *   fp         = a valid, open, file pointer where the data should be 
 *                written to. 
 *   fname      = if not NULL, then the string in fname (the file
 *                name) is written to a comment line in the output
 *                file. 
 *   model_name = Unique name for the particular model being saved in
 *                the file.  This must be in the list given in
 *                wcalc_loadsave.h 
 */
void wcalc_save_header(FILE *fp, char *fname, char *model_name)
{
  time_t now;

  now = time(NULL);
 
  assert(fp != NULL);

  fprintf(fp,"#\n");
  if (fname != NULL){
    fprintf(fp,"# File:      %s\n", fname);
  }
  fprintf(fp,"# Modified:  %s", ctime(&now));
  fprintf(fp,"# Wcalc Version %s\n", VER);
  fprintf(fp,"\n");

  fprintf(fp,"[wcalc]\n");
  fprintf(fp,"wcalc_file_version = %s\n", WCALC_FILE_VERSION);
  fprintf(fp,"model_name = %s\n", model_name);

  fprintf(fp,"\n");

}
  

int wcalc_load(FILE *fp)
{
  char *val;
  
  /* we should never be given a NULL file pointer */
  assert(fp != NULL);


  val = file_read_val(fp, "[wcalc]", "wcalc_file_version");
  if( val == NULL ) {
    alert("The selected file does not have the mandatory\n"
	  "[wcalc] section");
    return -1;
  }

#ifdef DEBUG
  printf("wcalc_load():  wcalc_file_version = \"%s\"\n",val);
#endif
  
  if( strcmp(val, WCALC_FILE_VERSION) != 0) {
    alert("Unable to load a wcalc file with file version\n"
	  "\"%s\"\n", val);
    return -1;
  }

  val = file_read_val(fp,"[wcalc]", "model_name");
  if( val == NULL ) {
    alert("The selected file does not have the mandatory\n"
	  "key \"model_name\" in the [wcalc] section");
    return -1;
  }
#ifdef DEBUG
  printf("wcalc_load():  model_name = \"%s\"\n",val);
#endif
  
  if (strcmp(val, FILE_AIR_COIL) == 0) {
    return MODEL_AIR_COIL;
  }
  else if (strcmp(val, FILE_BARS) == 0) {
    return MODEL_BARS;
  }
  else if (strcmp(val, FILE_COAX) == 0) {
    return MODEL_COAX;
  }
  else if (strcmp(val, FILE_COPLANAR) == 0) {
    return MODEL_COPLANAR;
  }
  else if (strcmp(val, FILE_COUPLED_MICROSTRIP) == 0) {
    return MODEL_COUPLED_MICROSTRIP;
  }
  else if (strcmp(val, FILE_COUPLED_STRIPLINE) == 0) {
    return MODEL_COUPLED_STRIPLINE;
  }
  else if (strcmp(val, FILE_IC_MICROSTRIP) == 0) {
    return MODEL_IC_MICROSTRIP;
  }
  else if (strcmp(val, FILE_MICROSTRIP) == 0) {
    return MODEL_MICROSTRIP;
  }
  else if (strcmp(val, FILE_PARALLEL_RC) == 0) {
    return MODEL_PARALLEL_RC;
  }
  else if (strcmp(val, FILE_PARALLEL_RL) == 0) {
    return MODEL_PARALLEL_RL;
  }
  else if (strcmp(val, FILE_RODS) == 0) {
    return MODEL_RODS;
  }
  else if (strcmp(val, FILE_STRIPLINE) == 0) {
    return MODEL_STRIPLINE;
  }

  alert("wcalc_loadsave.c:wcalc_load():  model_name\n"
	"\"%s\" is not understood\n",val);

  return -1;
}


/* 
 * Reads the value of a particular key from a particular section of
 * a wcalc file.  For example:
 *  val=file_read_val(fp,"[air_coil]","rho"); 
 * will look for the variable "rho" in the [air_coil] section.
 * If found, 'val' will point to the string value for rho otherwise
 * NULL is returned.
 */

char * file_read_val(FILE *fp, const char *section, const char *key)
{
  char line[MAXLINELEN];
  char *tok, *val, *ret;
  int sec_ok=0;
  
  rewind(fp);
  
  while( fgets(line,MAXLINELEN,fp) != NULL ){
    /* read the first token from the line */
    tok = strtok(line,FIELDSEP);
    
    if (tok != NULL){

      /* XXX probably a bad idea to lowercase everything */
      /*
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]);
      }
      */

      /* process each token on the line, skipping comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if (strcmp(tok,section) == 0)
	    sec_ok=1;
	  else if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
	    sec_ok=0;
	  }
	  else if ( sec_ok ){
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
#ifdef DEBUG
	      fprintf(stderr,"wcalc_loadsave.c:file_read_val():  could not read value to go"
		      " with %s=.\n",tok);
#endif
	      return NULL;
	    }
#ifdef DEBUG
	    printf("wcalc_loadsave.c:file_read_val():  \"%s\" ?= \"%s\"\n",
		   tok,key);
#endif
	    if (strcmp(tok,key) == 0){
	      ret=strdup(val);
	      rewind(fp);
	      return ret;
	    }
	  } 
	} while ( (tok = strtok(NULL,FIELDSEP)) != NULL );
      }
    }
  }
  
  /* we shouldn't have gotten here if the key was found */
  rewind(fp);
  return NULL;
}



fspec * fspec_add_key(fspec *list,
		      char *name,
		      char *comment,
		      char type,
		      void *ofs
		      )
{
  fspec *new;
  fspec *cur;
  int i;
#ifdef DEBUG
  printf("fspec_add_key(list=%p,"
	 "              name=\"%s\",\n"
	 "              comment=\"%s\",\n"
	 "              type='%c',\n"
	 "              ofs=%p);\n",
	 list,name,comment,type,ofs);
#endif

  assert (list != NULL);
  
  /* Create the new entry */
  if ( (new = malloc(sizeof(fspec))) == NULL ){
    fprintf(stderr,"fspec_add_key():  malloc failed\n");
    exit(1);
  }

  if ( (new->key = malloc((strlen(name) + 3)*sizeof(char))) == NULL ) {
    fprintf(stderr,"fspec_add_key():  malloc failed\n");
    exit(1);
  }


  for (i=0 ; i<strlen(name) ; i++){
    /* XXX probably a bad idea to lowercase it all*/
    /*new->key[i] = tolower(name[i]); */
    new->key[i] = name[i]; 
  }
  new->key[strlen(name)] = '\0';

  if ( (new->comment = malloc((strlen(comment) + 1)*sizeof(char))) == NULL ) {
    fprintf(stderr,"fspec_add_key():  malloc failed\n");
    exit(1);
  }
  sprintf(new->comment,"%s",comment);

  if (type == 'f')
    new->spec_type=SPEC_FIXED;
  else
    new->spec_type=SPEC_KEY;
  new->type=type;
  new->ofs=(unsigned long) ofs;
  new->next=NULL;

  /* search to the end of the list */
  cur = list;
  while ( cur->next != NULL)
    cur = cur->next;
  
  /* append to the list */
  cur->next = new;
  new->prev=cur;

  return list;
}

fspec * fspec_add_sect(fspec *list,char *name)
{
  fspec *new;
  fspec *cur;
  int is_new=0;

  if (list == NULL)
    is_new=1;
  
  /* Create the new entry */
  if ( (new = malloc(sizeof(fspec))) == NULL ){
    fprintf(stderr,"fspec_add_sect():  malloc failed\n");
    exit(1);
  }

  if ( (new->key = malloc((strlen(name) + 3)*sizeof(char))) == NULL ) {
    fprintf(stderr,"fspec_add_sect():  malloc failed\n");
    exit(1);
  }
  sprintf(new->key,"[%s]",name);

  new->spec_type=SPEC_SECTION;
  new->comment=NULL;
  new->type='X';
  new->ofs=0;
  new->next = NULL;
  new->prev = NULL;

  if (is_new) {
#ifdef DEBUG
    printf("fspec_add_sect():  returning new fspec\n");
#endif
    return new;
  }
  else {
    /* search to the end of the list */
    cur = list;
    while ( cur->next != NULL)
      cur = cur->next;
    
    /* append to the list */
    cur->next = new;
    new->prev=cur;
#ifdef DEBUG
    printf("fspec_add_sect():  returning appended fspec\n");
#endif
    return list;
  }

}

fspec * fspec_add_comment(fspec *list,char *comment)
{
  fspec *new;
  fspec *cur;
  int is_new=0;

  if (list == NULL)
    is_new=1;
  
  /* Create the new entry */
  if ( (new = malloc(sizeof(fspec))) == NULL ){
    fprintf(stderr,"fspec_add_comment():  malloc failed\n");
    exit(1);
  }

  if ( (new->comment = malloc((strlen(comment) + 1)*sizeof(char))) == NULL ) {
    fprintf(stderr,"fspec_add_comment():  malloc failed\n");
    exit(1);
  }
  sprintf(new->comment,"%s",comment);

  new->spec_type=SPEC_COMMENT;
  new->key="[comment key]";
  new->type='X';
  new->ofs=0;
  new->next = NULL;
  new->prev = NULL;

  if (is_new) {
#ifdef DEBUG
    printf("fspec_add_comment():  returning new fspec\n");
#endif
    return new;
  }
  else {
    /* search to the end of the list */
    cur = list;
    while ( cur->next != NULL)
      cur = cur->next;
    
    /* append to the list */
    cur->next = new;
    new->prev=cur;
#ifdef DEBUG
    printf("fspec_add_comment():  returning appended fspec\n");
#endif
    return list;
  }

}

int fspec_write_file(fspec *list,FILE *fp,unsigned long base)
{
  fspec *cur;
  void *addr;
  char *tmps;

  assert(list != NULL);

  cur = list;
  while ( cur != NULL) {
    switch (cur->spec_type) {

    case SPEC_SECTION:
      fprintf(fp,"\n%s\n",cur->key);
      break;
      
    case SPEC_KEY:
      fprintf(fp,"\n# %s\n%s = ",cur->comment,cur->key);
      if (base != 0) {
	addr = (void *) (base + cur->ofs);
	switch (cur->type){
	  
	case 'd':
	  fprintf(fp,"%.15g",*((double *)addr));
	  break;
	  
	case 'i':
	  fprintf(fp,"%d",*((int *)addr));
	  break;
	
	case 's':
	  fprintf(fp,"%s",*((char **)addr));
	  break;
	  
	case 'f':
	  fprintf(fp,"%s",(char *) cur->ofs);
	  break;

	case 'u':
	  /* now write out the savestring for the units */
	  tmps = wc_units_to_savestr( *((wc_units **) addr));
#ifdef DEBUG
	  printf("fspec_write_file():  saving units string \"%s\"\n", tmps);
#endif
	  fprintf(fp, "%s", tmps);
	  free(tmps);

	  /* write out a comment showing what the units are */
	  tmps = wc_units_to_str( *((wc_units **) addr));
	  fprintf(fp, "\n# [%s]", tmps);
	  free(tmps);
	  break;

	default:
	  fprintf(stderr,"fspec_write_file():  Invalid type, '%c' in fspec\n",cur->type);
	  exit(1);
	}
      }
      fprintf(fp,"\n");
      break;

    case SPEC_FIXED:
      fprintf(fp,"\n# %s\n%s = %s\n",cur->comment,cur->key,(char *)cur->ofs);
      break;

    case SPEC_COMMENT:
      fprintf(fp,"\n# %s\n",cur->comment);
      break;

    default:
      fprintf(stderr,"fspec_write_file():  Invalid type, '%c' in fspec\n",cur->type);
      exit(1);
    }

    cur = cur->next;
  }

  fprintf(fp,"\n");

  return 0;
}


int fspec_read_file(fspec *list, FILE *fp, unsigned long base)
{
  fspec *cur;
  void *addr=NULL;
  int *gotval;
  int whichval;
  int nvals;
  char line[MAXLINELEN+1];
  char *tok,*val;
  int i;
  int found_section,found_key;
  char *section=NULL;
  int rslt=0;

  assert(list != NULL);

  /* count how many values we need to get */
  cur = list;
  nvals = 0;
  while ( cur != NULL) {
    if(cur->spec_type == SPEC_KEY)
      nvals++;
    cur = cur->next;
  }

#ifdef DEBUG
  printf("fspec_read_file():  nvals=%d\n",nvals);
#endif

  if ( (gotval=malloc(nvals*sizeof(int))) == NULL) {
    fprintf(stderr,"fspec_read_file():  malloc failed\n");
    exit(1);
  }
  
  for(i=0; i<nvals; i++)
    gotval[i] = 0;

  
  while( fgets(line,MAXLINELEN,fp) != NULL ){
#ifdef DEBUG
    printf("fspec_read_file():  read line \"%s\"\n", line);
#endif
    tok = strtok(line,FIELDSEP);
    if (tok != NULL){
      /*
      for (i=0 ; i<strlen(tok) ; i++){
	tok[i] = tolower(tok[i]); 
      }
      */
#ifdef DEBUG
	    printf("fspec_read_file():  tok = \"%s\", length=%ld\n",tok,(long int) strlen(tok));
#endif
      /* skip comment lines */
      if ( (tok[0] != '#') &&
	   (tok[0] != ';') &&
	   (tok[0] != '*') ){
	do {
	  if ( (tok[0] == '[') && (tok[strlen(tok)-1] == ']') ){
#ifdef DEBUG
	    printf("fspec_read_file():  Starting section \"%s\"\n",tok);
#endif
	    section=strdup(tok);
	  }
	  else {
#ifdef DEBUG
	    printf("fspec_read_file():  section \"%s\", read key \"%s\"\n",section,tok);
#endif
	    if ( (val = strtok(NULL,FIELDSEP)) == NULL ) {
	      fprintf(stderr,"Could not read value associated with"
		      "key \"%s\"\n",tok);
	      return -1;
	    }
	    
	    /*
	     * now figure out what to do with this key/value pair
	     * we've read 
	     */
	    whichval = 0;

	    /* 
	     * search until we find the correct section. 
	     */
	    cur = list;
	    found_section = 0;
	    while ( (cur != NULL) && !found_section ) {
	      if (cur->spec_type != SPEC_SECTION) {
		whichval++;
	      }
	      else if (strcmp(section,cur->key) == 0) {
		  found_section = 1;
	      }
	      cur=cur->next;
	    }
	    
	    if (! found_section) {
#ifdef DEBUG
	      printf("fspec_read_file():  Could not find section \"%s\" "
		     "of fspec.  Skipping...\n",section);
#endif
	      break;
	    }

	    /*
	     * now search for the matching key in this section 
	     */
	    found_key = 0;
	    while ( (cur != NULL) && 
		    (cur->spec_type != SPEC_SECTION) && 
		    !found_key) {
#ifdef DEBUG
	      printf("fspec_read_file():          is read key \"%s\" == "
		     "fspec key \"%s\"? ", tok, cur->key);
#endif
	      if ( ( cur->spec_type == SPEC_KEY ) && (strcmp(tok, cur->key) == 0) ) {
		found_key = 1;
#ifdef DEBUG
		printf("yes, value #%d\n",whichval);
#endif
		assert(whichval < nvals);
		gotval[whichval] = 1;
	      } else if ( (cur->spec_type == SPEC_FIXED) &&
			  strcmp(tok, cur->key) == 0) {
#ifdef DEBUG
		printf("yes.  skipping fixed key\n");
#endif
		found_key = 1;
	      } else {
		
#ifdef DEBUG
		printf("no\n");

		if ( cur->spec_type == SPEC_COMMENT )
		  printf("skipping comment\n");
#endif

		if (cur->spec_type == SPEC_KEY)
		  whichval++;

		cur = cur->next;
	      }

	    }

	    if (! found_key) {
	      /*
	      alert("Skipping unknown key \"%s\" in section \"%s\"\n",
	      tok,section);*/
	      printf("fspec_read_file():  Could not find key \"%s\" "
		     "in section \"%s\" of fspec\n",tok,section);
	      break;
	    }

#ifdef DEBUG
	    printf("checking for NULL cur (%p) \n", cur);
#endif
	    if (cur != NULL){
	      addr = (void *) (base + cur->ofs);
	    }

#ifdef DEBUG
	    printf("fspec_read_file():  Processing key of type \'%c\'\n", cur->type);
#endif

	    switch (cur->type){
	      
	    case 'd':
	      *((double *)addr) = atof(val);
#ifdef DEBUG
	      printf("fspec_read_file():  Just read (double) %s=%g\n",tok,*((double *)addr));
#endif
	      break;
	      
	    case 'i':
	      *((int *)addr) = atoi(val);
#ifdef DEBUG
	      printf("fspec_read_file():  Just read (int) %s=%d\n",tok,*((int *)addr));
#endif
	      break;
	  
	    case 's':
	      *((char **)addr) = strdup(val);
#ifdef DEBUG
	      printf("fspec_read_file():  Just read (string) %s=%s\n",tok,*((char **)addr));
#endif
	      break;
	  
	    case 'f':
#ifdef DEBUG
	      printf("fspec_read_file():  Just read (fixed) %s=%s\n", tok, val);
#endif
	      break;

	    case 'u':
	      wc_savestr_to_units(val, *((wc_units **)addr));
#ifdef DEBUG
	      printf("fspec_read_file():  Just read (units save-string) %s\n", tok);
#endif
	      break;

	    default:
	      fprintf(stderr,"fspec_read_file():  Invalid type, '%c' in fspec\n",cur->type);
	      exit(1);
	    }
	  }
	} while ( (tok = strtok(NULL,FIELDSEP)) != NULL );
      }
      else {
#ifdef DEBUG
	    printf("fspec_read_file():  skipping comment line "
		   "starting with\"%s\"\n",tok);
#endif
      }
    }
  }

  /* now see if we've read all we need to read */
  cur = list;
  section = "none";
  for (i=0; i<nvals; i++) {

    while ( (cur != NULL) && (cur->spec_type != SPEC_KEY) ) {
      section = cur->key;
      cur = cur->next;
    }
    if (!gotval[i]) {
      alert("fspec_read_file():  Missing value #%d\n"
	    "                    section \"%s\", key=\"%s\"\n"
	    "                    in the file\n",i,section,cur->key);
      rslt=-1;
    }

    if (cur != NULL) {
      cur = cur->next;
    }
  }

  return rslt;
}

char * fspec_write_string(fspec *list, unsigned long base)
{
  fspec *cur;
  void *addr;
  int len=1;
  int pass;
  char *str=NULL;
  /* XXX fixme (no overflow) */
  char tmps[80];
  char *tmps2;

  assert(list != NULL);

  /*
   * take 2 passes through.  1st time, we figure out the required
   * string length.  2nd time through, we copy stuff over.
   */
  for (pass=0; pass<2; pass++){
    cur = list;
    while ( cur != NULL) {
      switch (cur->spec_type) {
	
      case SPEC_SECTION:
	sprintf(tmps,"%s", cur->key);
	if (!pass)
	  len = len + 1 + strlen(tmps);
	else {
	  strcat(str,tmps);
	  strcat(str," ");
	}
	break;
      
      case SPEC_KEY:
	if (base != 0) {
	  addr = (void *) (base + cur->ofs);
	  switch (cur->type){
	  
	  case 'd':
	    sprintf(tmps,"%.15g",*((double *)addr));
	    break;
	    
	  case 'i':
	    sprintf(tmps,"%d",*((int *)addr));
	    break;
	    
	  case 's':
	    sprintf(tmps,"%s",*((char **)addr));
	    break;
	    
	  case 'f':
	    sprintf(tmps,"%s",(char *) cur->ofs);
	    break;
	    
	  case 'u':
	    tmps2 = wc_units_to_savestr( *((wc_units **) addr));
	    sprintf(tmps, "%s", tmps2);
	    free(tmps2);
	    break;

	  default:
	    fprintf(stderr,"fspec_write_string():  Invalid "
		    "type, '%c' in fspec\n",cur->type);
	    exit(1);
	  }
	}
	if (!pass)
	  len = len + 1 + strlen(tmps);
	else {
	  strcat(str,tmps);
	  strcat(str," ");
	}
	
	break;
	
      case SPEC_FIXED:
	if (!pass) {
	  len = len + 1 + strlen((char *) cur->ofs);
	}
	else {
	  strcat(str,(char *)cur->ofs);
	  strcat(str," ");
	}
	break;
	
      case SPEC_COMMENT:
	break;
	
      default:
	fprintf(stderr,"fspec_write_string():  Invalid type, '%c' in fspec\n",cur->type);
	exit(1);
      }
      
      cur = cur->next;
    }
    
    if (!pass) {
      if ( (str = malloc(len*sizeof(char))) == NULL) {
	fprintf(stderr,"fspec_write_string():  malloc() failed\n");
	exit(1);
      }
      str[0]='\0';
    }
  } 

  return str;
}

int fspec_read_string(fspec *list, const char *str, unsigned long base)
{
  fspec *cur;
  void *addr;
  int rslt=0;
  char *tok;
  char *section;
  char *mystr;

  assert(list != NULL);

  mystr = strdup(str);

  /* 
   * search until we find the section name
   */
  cur = list;
  while ( (cur != NULL) && (cur->spec_type != SPEC_SECTION) )
    cur = cur->next;
  if( cur != NULL) {
    section = cur->key;
#ifdef DEBUG
    printf("section = %s\n", section);
#endif
  } else {
    return -1;
  }

  if ( (tok=strtok(mystr," ")) == NULL ) {
    return -1;
  }

  /* 
   * search until we find the section in the savestring
   */

  while( (tok != NULL) && (strcmp(tok, section) != 0) ) {
#ifdef DEBUG
    printf("does \"%s\" = \"%s\"?\n", tok, section);
#endif
    tok = strtok(NULL, " ");
  }
  if(tok == NULL)
    return -1;

#ifdef DEBUG
  printf("loading string\n");
#endif

  cur = list;
  while ( cur != NULL) {
    switch (cur->spec_type) {
      
    case SPEC_SECTION:
      /* read the section tag and move on */
#ifdef DEBUG
      printf("tok = \"%s\"\n", tok);
#endif
      if ( (tok = strtok(NULL, " ")) == NULL ) {
	return -1;
      }
#ifdef DEBUG
      printf("tok = \"%s\"\n", tok);
#endif
      break;
      
    case SPEC_KEY:
      if (base != 0) {
	addr = (void *) (base + cur->ofs);
	switch (cur->type){
	  
	case 'd':
	  *((double *)addr) = atof(tok);
#ifdef DEBUG
	  printf("fspec_read_string():  read double = %g from \"%s\" (%s)\n",
		 *((double *)addr),tok,cur->comment);
#endif
	  break;
	  
	case 'i':
	  *((int *)addr) = atoi(tok);
#ifdef DEBUG
	  printf("fspec_read_string():  read int = %d from \"%s\" (%s)\n",
		 *((int *)addr),tok,cur->comment);
#endif
	  break;
	  
	case 's':
	  *((char **)addr) = strdup(tok);
#ifdef DEBUG
	  printf("fspec_read_string():  read string = %s from \"%s\" (%s)\n",
		 *((char **)addr),tok,cur->comment);
#endif
	  break;
	  
	case 'f':
#ifdef DEBUG
	  printf("fspec_read_string():  skipping fixed key\n");
#endif
	  break;
	  
	case 'u':
	  wc_savestr_to_units( tok, *((wc_units **)addr));
#ifdef DEBUG
	  printf("fspec_read_string():  read units save string string from \"%s\" (%s)\n",
		 tok, cur->comment);
#endif
	  break;
	default:
	  fprintf(stderr,"fspec_read_string():  Invalid type, '%c' in fspec\n",cur->type);
	  exit(1);
	}
	if ( (tok=strtok(NULL," ")) == NULL ) {
	  return -1;
	}

      }
      break;
	
    case SPEC_FIXED:
#ifdef DEBUG
      printf("fspec_read_string():  skipping SPEC_FIXED\n");
#endif
      if ( (tok=strtok(NULL," ")) == NULL ) {
	return -1;
      }
      break;
	
    case SPEC_COMMENT:
#ifdef DEBUG
      printf("fspec_read_string():  skipping SPEC_COMMENT\n");
#endif
      break;
	
    default:
      fprintf(stderr,"fspec_write_string():  Invalid type, '%c' in fspec\n",cur->type);
      exit(1);
    }
    
    cur = cur->next;
  }
  
  free(mystr);
  return rslt;
}

