# Make sure everything has been built before we start
# redo-ifchange all

# check for Error string...
# if ./course_server_comp | grep 'Error' >/dev/null; then
#     echo "Something Broke" >&2
#     exit 1
# else
#     echo "Success" >&2
# fi


# check for leaks...
# if leaks -atExit -- ./course_server_comp | grep LEAK:; then
#     echo "There's a memory leak in the server" >&2
#     exit 1
# else
#     echo "No memory leak" >&2
#     exit 0
# fi