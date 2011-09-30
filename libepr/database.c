/*
 * Database functions.
 * Last modified: Oct-13-2009 jmeloranta@gmail.com
 *
 * Written by:
 * Jussi Eloranta (jmeloranta@gmail.com)
 *
 */

#include "epr.h"

static void _upcase(char *);
static int _iskeyword(char *);

/*
 * Function: int epr_database_load_many(epr_type **, int, char *, char *)
 * Purpose: load many spectra into spectrum array based on wildcard
 *          file name.
 *
 * sp    = Spectrum pointer array [output; epr_type **]
 * nspec = Size of array sp [input; int]
 * wild  = Wildcard string for filename match [input; char *]
 * dir   = Directory to search [input; char *]
 *
 * returns 0 = not found or n = n matches found.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

static char *files[EPR_MAXFILES];

static int epr_database_compar(const void *a, const void *b) {

  char *x, *y;
  int i;

  x = *((char **) a);
  y = *((char **) b);

  for (i = 0; x[i] && y[i]; i++) {
    if(x[i] < y[i]) return 1;
    if(x[i] > y[i]) return -1;
  }
  return 0;
}
    
int epr_database_load_many(epr_type **sp, int nspec, char *wild, char *dir) {
  
  DIR *dirp;
  struct dirent *dp;
  int i, j;
  char tmp[MAXPATHLEN];

  if(!(dirp = opendir(dir))) 
    return 0;
  re_comp(wild);
  for (dp = readdir(dirp), i = 0; dp; dp = readdir(dirp)) {
    j = strlen(dp->d_name) - 4;
    if(strcmp(dp->d_name + j, ".spe")) continue;
    dp->d_name[j] = 0;
    if(!re_exec(dp->d_name)) continue;
    sprintf(tmp, "%s/%s", dir, dp->d_name);
    files[i] = (char *) malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(files[i], tmp);
    if(i == nspec && i < EPR_MAXFILES) break;
    i++;
  }
  qsort((void *) files, (size_t) i, (size_t) sizeof(char *), epr_database_compar);

  for(j = 0; j < i; j++) {
    sp[j] = epr_file_read(files[j]);
    free(files[j]);
  }
  return i;
}  

/*
 * Function: int epr_database_lookup(epr_type **, int, char *, char *)
 * Purpose: search spectra database directory for certain keyword
 *          and return pointers to each of these spectra.
 *
 * array   = array of pointers to epr_type structs (terminated with 0).
 *           [epr_type **, output].
 * size    = size of pointer array. [int; input]
 * string  = string to be searched from the fields: NAME, OPER, DATE
 *           [char *, input].
 * dir     = database directory [char *, input].
 *
 * Warning: if epr_read_file() fails then sp[i] may be a NULL pointer.
 *
 * return value:  0 = not found (or error)
 *                n = n matches found
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

int epr_database_lookup(epr_type **array, int size, char *string, char *dir) {

  FILE *fp;
  void _upcase();
  DIR *dirp;
  struct dirent *dp;
  struct stat sbuf;
  char buffer[MAXPATHLEN], string2[EPR_IBUFLEN];
  char text[EPR_IBUFLEN];
  int i = 0;

  if(!(dirp = opendir(dir)))
    return 0;

  strcpy(string2, string);
  _upcase(string2);
  for (dp = readdir(dirp); dp; dp = readdir(dirp)) {
    if(strcmp(dp->d_name + strlen(dp->d_name) - 4, ".spe")) continue;
    sprintf(buffer, "%s/%s", dir, dp->d_name);
    if(stat(buffer,&sbuf) < 0 || S_ISDIR(sbuf.st_mode)) continue;
    if(!(fp = fopen(buffer, "r"))) continue;
    while(fgets(text, sizeof(text), fp)) {
      _upcase(text);
      if(_iskeyword(text) && strstr(text,string2)) {
	char *tmp;
	if (i >= size) return 0;
	if((tmp = strrchr(buffer, '.'))) *tmp = 0;
	array[i] = epr_file_read(buffer);
	i++;
	break;
      }
    }
    fclose(fp);
  }
  return i;
}

static void _upcase(char *str) {

  for (;*str; str++)
    if(islower(*str)) *str = toupper(*str);
}

static int _iskeyword(char *str) {
  
  return !(strncmp(str, "NAME", 4) && strncmp(str, "OPER", 4) &&
	   strncmp(str, "DATE", 4) && strncmp(str, "INFO", 4));
}
