try-inotify
===========

# make
gcc -o main src/main.c src/opt.c src/inotify.c
# ./main -f . &
[1] 2728
file:1=[.]
# touch hoge
00:18:36.973692 IN_CREATE: size=16 name=[hoge]
00:18:36.973778 IN_OPEN: size=16 name=[hoge]
00:18:36.973808 IN_ATTRIB: size=16 name=[hoge]
00:18:36.973824 IN_CLOSE_WRITE: size=16 name=[hoge]

