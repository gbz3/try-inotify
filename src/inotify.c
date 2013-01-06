#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "inotify.h"

#define BUFF_SIZE 16384

int open_inotify_fd( struct monitor_t* mon ) {
  mon->inotify_fd = inotify_init();
  if( mon->inotify_fd < 0 ) {
    perror( "inotify_init()" );
    return 0;
  }
  mon->names[0] = NULL;
  return 1;
}

int watch_file( struct monitor_t* mon, char* name ) {

  struct stat fStat;
  if( stat( name, &fStat ) ) {
    perror( name );
    return 0;
  }

#ifdef DEBUG
  fprintf( stdout, "file=[%s] ISREG=%d ISDIR=%d ISLNK=%d\n",
    name, S_ISREG( fStat.st_mode ), S_ISDIR( fStat.st_mode ), S_ISLNK( fStat.st_mode ) );
#endif

  int wd;
  wd = inotify_add_watch( mon->inotify_fd, name, IN_ALL_EVENTS );
  if( wd < 0 ) {
    perror( name );
    return 0;
  }
  mon->names[wd] = name;
  return 1;
}

int watch_dir( struct monitor_t* mon, char* name, int recursive ) {

  struct stat fStat;
  if( stat( name, &fStat ) ) {
    perror( name );
    return 0;
  }

#ifdef DEBUG
  fprintf( stdout, "dir=[%s] ISREG=%d ISDIR=%d ISLNK=%d\n",
    name, S_ISREG( fStat.st_mode ), S_ISDIR( fStat.st_mode ), S_ISLNK( fStat.st_mode ) );
#endif

  int wd;
  wd = inotify_add_watch( mon->inotify_fd, name, IN_ALL_EVENTS );
  if( wd < 0 ) {
    perror( name );
    return 0;
  }
  mon->names[wd] = name;

  if( recursive && S_ISDIR( fStat.st_mode ) ) {
    DIR* pDir;
    struct dirent* pEnt;

    if( ( pDir = opendir( name ) ) == NULL ) {
      perror( name );
      return 0;
    }

    while( ( pEnt = readdir( pDir ) ) != NULL ) {
      if( !strncmp( ".", pEnt->d_name, 1 ) || !strncmp( "..", pEnt->d_name, 2 ) ) {
        continue;
      }

      char* eName = malloc( strlen( name ) + strlen( pEnt->d_name ) + 1 );
      sprintf( eName, "%s/%s", name, pEnt->d_name );

#ifdef DEBUG
      fprintf( stdout, "%s\n", eName );
#endif

      struct stat curStat;
      if( stat( eName, &curStat ) ) {
        perror( eName );
        return 0;
      }

      if( S_ISDIR( curStat.st_mode ) ) {
        watch_dir( mon, eName, recursive );
      } else {
        free( eName );
      }
    }
  }
  return 1;
}

int event_check( struct monitor_t* mon ) {
  fd_set rfds;
  FD_ZERO( &rfds );
  FD_SET( mon->inotify_fd, &rfds );
  return select( FD_SETSIZE, &rfds, NULL, NULL, NULL );
}

int read_events( struct monitor_t* mon ) {
  char buffer[BUFF_SIZE];
  size_t buffer_i, event_size;
  ssize_t ret;
  struct inotify_event *pevent;

  ret = read( mon->inotify_fd, buffer, BUFF_SIZE );
  if( ret <= 0 ) {
    perror( "read()" );
    return ret;
  }

  buffer_i = 0;
  while( buffer_i < ret ) {
    pevent = (struct inotify_event *) &( buffer[buffer_i] );
    event_size = offsetof( struct inotify_event, name ) + pevent->len;
    print_event( pevent, mon->names[pevent->wd] );
    buffer_i += event_size;
  }

}

int print_event( struct inotify_event* pevent, char* name ) {
  switch( pevent->mask ) {
    case IN_ACCESS:        return print_inotify_event( pevent, name, "IN_ACCESS" );
    case IN_MODIFY:        return print_inotify_event( pevent, name, "IN_MODIFY" );
    case IN_ATTRIB:        return print_inotify_event( pevent, name, "IN_ATTRIB" );
    case IN_CLOSE_WRITE:   return print_inotify_event( pevent, name, "IN_CLOSE_WRITE" );
    case IN_CLOSE_NOWRITE: return print_inotify_event( pevent, name, "IN_CLOSE_NOWRITE" );
    case IN_CLOSE:         return print_inotify_event( pevent, name, "IN_CLOSE" );
    case IN_OPEN:          return print_inotify_event( pevent, name, "IN_OPEN" );
    case IN_MOVED_FROM:    return print_inotify_event( pevent, name, "IN_MOVED_FROM" );
    case IN_MOVED_TO:      return print_inotify_event( pevent, name, "IN_MOVED_TO" );
    case IN_MOVE:          return print_inotify_event( pevent, name, "IN_MOVE" );
    case IN_CREATE:        return print_inotify_event( pevent, name, "IN_CREATE" );
    case IN_DELETE:        return print_inotify_event( pevent, name, "IN_DELETE" );
    case IN_DELETE_SELF:   return print_inotify_event( pevent, name, "IN_DELETE_SELF" );
    case IN_MOVE_SELF:     return print_inotify_event( pevent, name, "IN_MOVE_SELF" );
    case IN_UNMOUNT:       return print_inotify_event( pevent, name, "IN_UNMOUNT" );
    case IN_Q_OVERFLOW:    return print_inotify_event( pevent, name, "IN_Q_OVERFLOW" );
    case IN_IGNORED:       return print_inotify_event( pevent, name, "IN_IGNORED" );
  }
}

int print_inotify_event( struct inotify_event* pevent, char* name, char* msg ) {
  struct timeval now;
  struct tm *tmptr = NULL;

  gettimeofday( &now, NULL );
  tmptr = localtime( &now.tv_sec );
  fprintf( stdout, "%02d:%02d:%02d.%06d ",
    tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec, now.tv_usec );
  return fprintf( stdout, "%s: size=%d name=[%s/%s]\n", msg, pevent->len, name, pevent->name );
}

