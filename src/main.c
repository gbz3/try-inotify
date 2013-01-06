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
  struct monitor_t mon;
  int ret, i;

  if( signal( SIGINT, signal_handler ) == SIG_IGN )
    signal( SIGINT, SIG_IGN );

  if( ( ret = init_opt( &opt, argc, argv ) ) <= 0 ) {
    fprintf( stderr, "error at init_opt()=%d\n", ret );
    return ret;
  }

  if( !open_inotify_fd( &mon ) ) {
    return 1;
  }

  for( i = 0; i < opt.files_size; i++ ) {
    watch_file( &mon, opt.files[i] );
  }

  for( i = 0; i < opt.dirs_size; i++ ) {
    watch_dir( &mon, opt.dirs[i], opt.recursive );
  }

  while( keep_running ) {
    if( event_check( &mon ) > 0 ) {
      int r = read_events( &mon );
      if( r < 0 ) {
        break;
      }
    }
  }

  fprintf( stdout, "HOGE\n" );
  return 0;
}

