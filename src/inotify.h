#ifndef _INOTIFY_H
#define _INOTIFY_H

#include <sys/inotify.h>

#define FILE_MAX 1024

struct monitor_t {
  int inotify_fd;
  char* names[FILE_MAX];
};

int open_inotify_fd( struct monitor_t* );
int watch_file( struct monitor_t*, char* );
int watch_dir( struct monitor_t*, char*, int );
int event_check( struct monitor_t* );
int read_events( struct monitor_t* );

int print_event( struct inotify_event* pevent, char* name );
int print_inotify_event( struct inotify_event* pevent, char* msg, char* name );

#endif

