#include <stddef.h>
#include <stdio.h>
#include <sys/select.h>
#include <time.h>

#include "inotify.h"

#define BUFF_SIZE 16384

int open_inotify_fd() {
  int fd;

  fd = inotify_init();
  if( fd < 0 ) {
    perror( "inotify_init()" );
  }
  return fd;
}

int event_check( int fd ) {
  fd_set rfds;
  FD_ZERO( &rfds );
  FD_SET( fd, &rfds );
  return select( FD_SETSIZE, &rfds, NULL, NULL, NULL );
}

int read_events( int fd ) {
  char buffer[BUFF_SIZE];
  size_t buffer_i, event_size;
  ssize_t ret;
  struct inotify_event *pevent;

  ret = read( fd, buffer, BUFF_SIZE );
  if( ret <= 0 ) {
    perror( "read()" );
    return ret;
  }

  buffer_i = 0;
  while( buffer_i < ret ) {
    pevent = (struct inotify_event *) &( buffer[buffer_i] );
    event_size = offsetof( struct inotify_event, name ) + pevent->len;
    print_event( pevent );
    buffer_i += event_size;
  }

}

int print_event( struct inotify_event* pevent ) {
  switch( pevent->mask ) {
    case IN_ACCESS:        return print_inotify_event( pevent, "IN_ACCESS" );
    case IN_MODIFY:        return print_inotify_event( pevent, "IN_MODIFY" );
    case IN_ATTRIB:        return print_inotify_event( pevent, "IN_ATTRIB" );
    case IN_CLOSE_WRITE:   return print_inotify_event( pevent, "IN_CLOSE_WRITE" );
    case IN_CLOSE_NOWRITE: return print_inotify_event( pevent, "IN_CLOSE_NOWRITE" );
    case IN_CLOSE:         return print_inotify_event( pevent, "IN_CLOSE" );
    case IN_OPEN:          return print_inotify_event( pevent, "IN_OPEN" );
    case IN_MOVED_FROM:    return print_inotify_event( pevent, "IN_MOVED_FROM" );
    case IN_MOVED_TO:      return print_inotify_event( pevent, "IN_MOVED_TO" );
    case IN_MOVE:          return print_inotify_event( pevent, "IN_MOVE" );
    case IN_CREATE:        return print_inotify_event( pevent, "IN_CREATE" );
    case IN_DELETE:        return print_inotify_event( pevent, "IN_DELETE" );
    case IN_DELETE_SELF:   return print_inotify_event( pevent, "IN_DELETE_SELF" );
    case IN_MOVE_SELF:     return print_inotify_event( pevent, "IN_MOVE_SELF" );
    case IN_UNMOUNT:       return print_inotify_event( pevent, "IN_UNMOUNT" );
    case IN_Q_OVERFLOW:    return print_inotify_event( pevent, "IN_Q_OVERFLOW" );
    case IN_IGNORED:       return print_inotify_event( pevent, "IN_IGNORED" );
  }
}

int print_inotify_event( struct inotify_event* pevent, char *msg ) {
  struct timeval now;
  struct tm *tmptr = NULL;

  gettimeofday( &now, NULL );
  tmptr = localtime( &now.tv_sec );
  fprintf( stdout, "%02d:%02d:%02d.%06d ",
    tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec, now.tv_usec );
  return fprintf( stdout, "%s: size=%d name=[%s]\n", msg, pevent->len, pevent->name );
}

