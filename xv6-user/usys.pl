#!/usr/bin/perl -w

# Generate usys.S, the stubs for syscalls.

print "# generated by usys.pl - do not edit\n";

print "#include \"kernel/include/sysnum.h\"\n";

sub entry {
    my $name = shift;
    print ".global $name\n";
    print "${name}:\n";
    print " li a7, SYS_${name}\n";
    print " ecall\n";
    print " ret\n";
}
	
entry("clone");
entry("exit");
entry("wait4");
entry("pipe2");
entry("read");
entry("write");
entry("close");
# entry("kill");
entry("execve");
entry("openat");
entry("fstat");
entry("mkdirat");
entry("chdir");
entry("dup");
entry("getpid");
entry("brk");
entry("nanosleep");
# entry("uptime");
# entry("test_proc");
entry("dev");
# entry("readdir");
entry("getcwd");
entry("getppid");
entry("mmap");
entry("munmap")
# entry("remove");
# entry("trace");
# entry("sysinfo");
# entry("rename");
