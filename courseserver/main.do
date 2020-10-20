# If main.c changes, this script needs to be
# re-run.

redo-ifchange course_server.c course_client.c

# $3 is the redo variable that represents the
# output filename.  We want to build a file
# called "hello", but if we write that directly,
# then an interruption could result in a
# partially-written file.  Instead, write it to
# $3, and redo will move our output into its
# final location, only if this script completes
# successfully.
#

clang -o course_server_comp course_server.c -Wall -g
clang -o course_client_comp course_client.c -Wall -g