#ifndef _OPT_H
#define _OPT_H

#include <unistd.h>

struct opt_t {
  int files_size;		/* size of files */
  char *files[128];		/* list of filenames */
};

int init_opt( struct opt_t *target, int argc, char *argv[] );

#endif

