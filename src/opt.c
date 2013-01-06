#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opt.h"

int init_opt( struct opt_t *target, int argc, char *argv[] ) {

  target->recursive = 0;
  target->files_size = 0;
  target->dirs_size = 0;

  int result;
  while( ( result = getopt( argc, argv, "d:f:r" ) ) != -1 ) {
    switch( result ) {
      case 'r':
        target->recursive = 1;
        break;
      case 'f':
        target->files[target->files_size] = malloc( strlen( optarg ) + 1 );
        strncpy( target->files[target->files_size], optarg, strlen( optarg ) + 1 );
        target->files_size++;
        break;
      case 'd':
        target->dirs[target->dirs_size] = malloc( strlen( optarg ) + 1 );
        strncpy( target->dirs[target->dirs_size], optarg, strlen( optarg ) + 1 );
        target->dirs_size++;
        break;
//      case 'd':
//        target->dirs[target->dirs_size] =
//          malloc( strlen( optarg ) + ( optarg[strlen(optarg)-1] == '/' )? 1: 2 );
//        strncpy( target->dirs[target->dirs_size], optarg, strlen( optarg ) + 1 );
//        if( optarg[strlen(optarg)-1] != '/' ) {
//          target->dirs[target->dirs_size][strlen( optarg )] = '/';
//          target->dirs[target->dirs_size][strlen( optarg )+1] = 0;
//        }
//        target->dirs_size++;
//        break;
    }
  }

  return ( target->files_size + target->dirs_size );
}

