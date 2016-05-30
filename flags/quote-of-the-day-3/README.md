## Quote of the Day 3

__Description__

```
The captain is sick of people breaking into his qotd server. From now on,
whether it dispenses a quote is entirely based on chance.

Or is it?

(Analyze the binary, retrieve today's quote from the server, and give us its
md5 hash)

ctf2.linux.ucla.edu:7003
```

__Hint__

```
time(0) is not very secure
```

__Flag__

```
9b581b1dc5d92333f8561c9c705b2bee
```

__Points__

```
50
```

## Solution

`secure_qotd_v3` is the attachment with the outdated quote. `secure_qotd_v3.c`
is the source code with the correct quote.

This program is the same as the one in [Quote of the Day
2](../quote-of-the-day-2/), up until right after the `read`. A quick scan of
the disassembly shows that it calls functions like `time`, and `srand`, and
`rand`. Scary!

If you stare at the execution while letting it step through the execution
(`layout asm` really helps with that), you will notice this loop. Annotate it
as you go along.

```
 400e1d:     mov    $0x0,%edi
 400e22:     callq  400a10 <time@plt>   # t = time(0)
 400e27:     mov    %eax,%edx
 400e29:     mov    -0x80(%rbp),%eax    # something0 = -0x80(%rbp)
 400e2c:     imul   %edx,%eax           # prod = t * something0
 400e2f:     mov    %eax,%edi
 400e31:     callq  4009d0 <srand@plt>  # srand(prod)
 400e36:     callq  400aa0 <rand@plt>   # r = rand()
 400e3b:     mov    %eax,-0x80(%rbp)    # something0 = r
 400e3e:     mov    -0x60(%rbp),%rdx    # something1 = -0x60(%rbp)
 400e42:     mov    -0x7c(%rbp),%eax    # something2 = -0x7c(%rbp)
 400e45:     cltq   
 400e47:     movzbl (%rdx,%rax,1),%edx  # ele = something1[something2]
 400e4b:     mov    -0x80(%rbp),%eax
 400e4e:     mov    %edx,%ecx
 400e50:     xor    %eax,%ecx           # ele = ele ^ r
 400e52:     mov    -0x60(%rbp),%rdx
 400e56:     mov    -0x7c(%rbp),%eax    # something1 and something2 again
 400e59:     cltq   
 400e5b:     mov    %cl,(%rdx,%rax,1)   # something1[something2] = (char)ele
 400e5e:     addl   $0x1,-0x7c(%rbp)    # something2 += 1
 400e62:     cmpl   $0xf,-0x7c(%rbp)
 400e66:     jle    400e1d <serve_request+0x168>    # j if something2 < 16
```

Like in the previous challenge, `-0x60(%rbp)` (what I called "something1")
is the address of the input buffer. Thus, this loop is xor'ing the input with
the output of `rand`! I guess the Captain did say that only the "lucky" will
pass the test.

The program then moves on to compare the input buffer with another buffer in
`-0x30(%rbp)`. What's in there? If we go back to the first mention of
`-0x30(%rbp)`, we see this:

```
 400dfb:     movq   $0x0,-0x30(%rbp)
 400e03:     movq   $0x0,-0x28(%rbp)
```

It's essentially a buffer of all zeroes. This means that your "password" must
come out to 16 zeroes when xor'd with some funky application of `rand`.

No human can type that. Why don't we fight code with code? We can generate the
password with our own C program:

```
#include <time.h>
#include <stdio.h>

int main() {
    int i;
    int r = 1;
    for (i = 0; i < 16; i++) {
        srand(time(0) * r);
        r = rand();
        char c = (char)r;
        fwrite(&c, 1, 1, stdout);
    }
}
```

Make sure your time is correct first! Sync it with an NTP server. Then, run the
password generator and pipe the password to the service right away:

```
$ gcc gen_password.c -o gen_password
$ ./gen_password | nc ctf2.linux.ucla.edu 7003
Welcome to my fabulous Quote of the Day dispenser!
To receive my wisdom, please enter the password.
Password: Here's the quote of the day:
In My Youth I Was Not As Old As I Am Now
```

Get the MD5 of the quote, and the flag is yours.
