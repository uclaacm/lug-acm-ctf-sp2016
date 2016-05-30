## Quote of the Day 2

__Description__

```
The captain was told that his qotd service wasn't very secure in its password
checking. So he made it better!

(Analyze the binary, retrieve today's quote from the server, and give us its
md5 hash)

ctf2.linux.ucla.edu:7002
```

__Hint__

_None_

__Flag__

```
d2e1222c827b9bc7c9c6b98857782858
```

__Points__

```
50
```

## Solution

`secure_qotd_v2` is the attachment with the outdated quote. `secure_qotd_v2.c`
is the source code with the correct quote.

You may have been able to solve [Quote of the Day 1](../quote-of-the-day-1/)
without running __gdb__, but secure_qotd_v2 is a bit more complex. Unless you
can read disassembly like reading newspaper for breakfast, you'd probably want
to debug it.

How to start? Professionals would probably have a different idea; my instinct
is to find a way to break on the print statements. In a networked program like secure_qotd_v2 however, a print actually means a `send` instead.

Let's try that:

```
gdb ./secure_qotd_v2
(gdb) b send
(gdb) run 7002
```

From another terminal, do:

```
telnet localhost 7002
```

And... nothing happens. The telnet session continues independently of the gdb
session somehow, and gdb never breaks. What's going on? A bit of digging
through the disassembly (`objdump -d secure_qotd_v2`), you may find a `fork` in
there. This networked program forks child processes to handle requests, and by
default gdb only follows the parent. To debug multiprocess programs, interrupt
the debug session with Ctrl+C, and do this:

```
(gdb) set detach-on-fork off
(gdb) set follow-fork-mode child
```

This tells gdb to keep debugging all forked processes, and follow the child by
default. The `info inferior` and `inferior` commands can be used to [switch
contexts](https://sourceware.org/gdb/current/onlinedocs/gdb/Inferiors-and-Programs.html).

Now, run `continue` and retry the telnet thing. gdb will show you some variant
of this:

```
[Switching to process 18222]

Breakpoint 1, __libc_send (fd=4, buf=0x4010d0, n=110, flags=0)
```

`bt` will show you where you are:

```
#0  __libc_send (fd=4, buf=0x4010d0, n=110, flags=0)
    at ../sysdeps/unix/sysv/linux/x86_64/send.c:26
#1  0x0000000000400cc8 in serve_request ()
#2  0x0000000000401001 in main ()
```

You're not interested in the `send` function itself, so let it do its thing by
calling `finish`, and return to `serve_request`:

```
(gdb) finish
Run till exit from #0  __libc_send (fd=4, buf=0x4010d0, n=110, flags=0)
    at ../sysdeps/unix/sysv/linux/x86_64/send.c:26
0x0000000000400cc8 in serve_request ()
Value returned is $5 = 110
(gdb) bt
#0  0x0000000000400cc8 in serve_request ()
#1  0x0000000000401001 in main ()
```

At this point, it will be useful to run `display/3i $pc` so that as you step
through the disassembly, gdb will show you the next few instructions. It will
also be useful to keep the full disassembly of the program available on the
side, so you can keep track of the big picture of where you are. Stepping
through an instruction can be done with `ni` (to step quickly, run `ni` once,
then just press enter in gdb to repreat the previous command).

Eventually, you'll encounter a call to `read`. This is where the program takes
input from the user. Give an easily recognizable string as a password, say,
"aaaaaaaaaaaaaaaaaaa", so that you can find it in memory later. If you examine
the arguments to `read`, you'll find that `%rsi`, which is the second parameter
(the buffer address), was moved from `%rcx`, which was moved from
`-0x60(%rbp)`. This means that `0x60(%rbp)`, i.e. the _content_ of %rbp + 0x60
is the memory address of the buffer. Check it by examining that address:

```
(gdb) x $rbp-0x60
0x7fffffffe320: 0x00007fffffffe2d0
(gdb) x/8cb 0x7fffffffe2d0
0x7fffffffe2d0: 97 'a'  97 'a'  97 'a'  97 'a'  97 'a'  97 'a'  97 'a'  97 'a'
```

This is thus the input buffer. From this point on, it's a lot of logic and
analyzing the flow of execution to figure out what the program might be doing.
For this, gdb's `layout asm` offers an easier view of the disassembly. It's
also very useful to annotate your copy of the disassembly as you go along, so
you can sort of "decompile" the program back to something more high level.

Here's an example. A little further, you will see that the function `swap` is
called a few times with the input buffer and two numbers as arguments. Follow
the call into `swap` by calling `si` instead of `ni`, and try to understand the
function:

```
0000000000400b90 <swap>:
  400b90:     push   %rbp
  400b91:     mov    %rsp,%rbp
  400b94:     mov    %rdi,-0x18(%rbp)   # arg1
  400b98:     mov    %esi,-0x1c(%rbp)   # arg2
  400b9b:     mov    %edx,-0x20(%rbp)   # arg3
  400b9e:     mov    -0x1c(%rbp),%eax
  400ba1:     movslq %eax,%rdx
  400ba4:     mov    -0x18(%rbp),%rax
  400ba8:     add    %rdx,%rax          # a = arg1 + arg2
  400bab:     movzbl (%rax),%eax        # b = *a
  400bae:     mov    %al,-0x1(%rbp)
  400bb1:     mov    -0x1c(%rbp),%eax
  400bb4:     movslq %eax,%rdx
  400bb7:     mov    -0x18(%rbp),%rax
  400bbb:     add    %rax,%rdx          # a = arg1 + arg2
  400bbe:     mov    -0x20(%rbp),%eax
  400bc1:     movslq %eax,%rcx
  400bc4:     mov    -0x18(%rbp),%rax
  400bc8:     add    %rcx,%rax          # c = arg1 + arg3
  400bcb:     movzbl (%rax),%eax
  400bce:     mov    %al,(%rdx)         # *a = *c
  400bd0:     mov    -0x20(%rbp),%eax
  400bd3:     movslq %eax,%rdx
  400bd6:     mov    -0x18(%rbp),%rax
  400bda:     add    %rax,%rdx          # c = arg1 + arg3
  400bdd:     movzbl -0x1(%rbp),%eax
  400be1:     mov    %al,(%rdx)         # *c = b
  400be3:     pop    %rbp
  400be4:     retq
```

Following the annotations, it's more clear now that the function swaps two
elements in the buffer given in arg1. You can then infer the result of the four
swap calls without looking through the function again.

Do the same kind of analysis for the rest of the program. You will notice that
a loop occurs which modifies the input buffer byte by byte. Finally, there is a
comparison loop that compares between the input buffer and another array at
`-0x30(%rbp)`.

It's a lot of work (perhaps a bit too much for a 3-hour CTF -- in retrospect,
this is easily 100 pts), but with some persistence, you should be able to work
out what it's doing to the input buffer, and reverse that process on the key in
order to figure out the password.
