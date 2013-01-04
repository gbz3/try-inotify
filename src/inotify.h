#ifndef _INOTIFY_H
#define _INOTIFY_H

#include <sys/inotify.h>

int open_inotify_fd();
int event_check( int );
int read_events( int );
int print_event( struct inotify_event *pevent );
int print_inotify_event( struct inotify_event *pevent, char *msg );

#endif

