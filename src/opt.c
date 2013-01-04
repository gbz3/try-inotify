#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opt.h"

int init_opt( struct opt_t *target, int argc, char *argv[] ) {

  target->files_size = 0;

  int result;
  while( ( result = getopt( argc, argv, "f:" ) ) != -1 ) {
    switch( result ) {
      case 'f':
        target->files[target->files_size] = malloc( strlen( optarg ) + 1 );
        strncpy( target->files[target->files_size], optarg, strlen( optarg ) + 1 );
        target->files_size++;
        break;
    }
  }

  return target->files_size? 0: -1;
}

