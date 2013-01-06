inot
====
inotify を使用しファイル, ディレクトリの変更を報告するコマンドです。  

使い方
------

```
# ./inot -f README.md -d . -r &
[1] 2850
# make clean
17:15:06.072457 IN_OPEN: size=16 name=[./Makefile]
17:15:06.072754 IN_ACCESS: size=16 name=[./Makefile]
17:15:06.072866 IN_CLOSE_NOWRITE: size=16 name=[./Makefile]
17:15:06.075362 IN_DELETE: size=16 name=[./inot]
```

オプションの解説
----------------

+ -f /path/to/filename
    監視対象のファイルパスを指定します。

+ -d /path/to/directory
    監視対象のディレクトリパスを指定します。

+ -r
    -d で指定したディレクトリ配下の全てのディレクトリを対象に含めます。

関連情報
--------

1. [リンク1](http://www.ibm.com/developerworks/jp/linux/library/l-inotify/ "inotify で Linux ファイルシステムのイベントを監視する")

ライセンス
----------
Copyright &copy; 2013 gbz3
...

