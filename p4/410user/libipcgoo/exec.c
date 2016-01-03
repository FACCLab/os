extern int pre_exec();
extern int sys_exec(char *execname, char *argvec[]);
int exec(char *execname, char *argvec[])
{
    // If pre_exec fails, ignore it, maybe udriv_* isn't implemented? The worst
    // case is that the exec'd program forgets which console its running on and
    // moves back to the VGA window.
    pre_exec();
    return sys_exec(execname, argvec);
}

