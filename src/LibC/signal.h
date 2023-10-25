// Copyright 2023 sysa. For authors see bottom.
// Licence: MIT

SIGABRT
SIGFPE
SIGILL
SIGINT
SIGSEGV
SIGTERM

SIGTRAP

SIGKILL
SIGSTOP

// functions
int raise(int sig);
int kill();
int signal(int sig, void* func(int sig));


/* Authors:
 *	Andreas Herzig
 */