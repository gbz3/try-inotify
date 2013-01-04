#include <signal.h>
#include <stdio.h>

#include "inotify.h"
#include "opt.h"

int keep_running = 1;

void signal_handler( int signum ) {
  fprintf( stdout, "SIGNUM=%d\n", signum );
  keep_running = 0;
}

int main( int argc, char *argv[] ) {

  struct opt_t opt;
  int ret;

  if( signal( SIGINT, signal_handler ) == SIG_IGN )
    signal( SIGINT, SIG_IGN );

  if( ( ret = init_opt( &opt, argc, argv ) ) != 0 ) {
    fprintf( stderr, "error at init_opt()=%d\n", ret );
    return ret;
  }

  int inotify_fd;
  inotify_fd = open_inotify_fd();
  if( inotify_fd <= 0 ) {
    perror( " " );
    return 1;
  }

  int i, wd;
  for( i = 0; i < opt.files_size; i++ ) {
    fprintf( stdout, "file:%d=[%s]\n", i + 1, opt.files[i] );
    wd = inotify_add_watch( inotify_fd, opt.files[i], IN_ALL_EVENTS );
    if( wd < 0 ) {
      perror( opt.files[i] );
      return 1;
    }
  }

  while( keep_running ) {
    if( event_check( inotify_fd ) > 0 ) {
      int r = read_events( inotify_fd );
      if( r < 0 ) {
        break;
      }
    }
  }

  fprintf( stdout, "HOGE\n" );
  return 0;
}

