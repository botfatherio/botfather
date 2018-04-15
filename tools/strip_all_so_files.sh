# Parameter 1: Destdir

find $1 -type f -name \*.so | xargs strip --strip-debug