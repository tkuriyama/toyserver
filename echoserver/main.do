
redo-ifchange echoclient.c echoserver.c

# $3 is the redo variable that represents the
# output filename.  We want to build a file
# called "hello", but if we write that directly,
# then an interruption could result in a
# partially-written file.  Instead, write it to
# $3, and redo will move our output into its
# final location, only if this script completes
# successfully.
#
clang -o echoclient_comp echoclient.c -Wall -g
clang -o echoserver_comp echoserver.c -Wall -g