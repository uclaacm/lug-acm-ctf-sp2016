## Quote of the Day 1

__Description__

```
A certain distinguished captain is fond of speaking words of wisdom. As such,
he provides a quote-of-the-day service so his followers can be touched by his
brilliance every day. Not everyone has access though; it will only let you in
if you know the password!

You managed to convince a sailor to steal the qotd binary and send it to you.
However, the quote in the binary is out of date. Analyze the binary, retrieve
today's quote from the server, and give us its md5 hash!

ctf2.linux.ucla.edu:7001
```

__Hint__

```
There's a very obvious segment of code in serve_request
```

__Flag__

```
51de72fc70308c816406370a71e56012
```

__Points__

```
30
```

## Solution

`secure_qotd` is the attachment with the outdated quote. `secure_qotd.c` is the
source code with the correct quote.

This challenge requires reverse engineering the binary given to you. The binary
is a service that listens on a port. When you connect to it (via __nc__ or
__telnet__), it asks for a password and dispenses a quote if you get it right.
You need to figure out the password, connect to the real service running on
ctf2.linux.ucla.edu:7001, and retrieve the quote on the server.

Reverse engineering may be fresh in the minds of CS33 students. I'm not a
reverse engineering expert myself, but I've seen people typically approach it
first by calling the __readelf__ or  __strings__ utilities and check if any
interesting info comes up. In this case however, this approach isn't very
useful.

Before diving straight into the __gdb__ grinder though, it might be useful to
take a quick look through the disassembly first:

```
objdump -d secure_qotd > secure_qotd.S
```

If you scan through the disassembly, you will find libc library functions, as
well as normal-sounding functions like `reap_children`, `serve_request`, and
`main`. In the process of skimming through it, you may also notice a big block
of code inside `serve_request`:

```
  400cd6:       c6 45 d0 67             movb   $0x67,-0x30(%rbp)
  400cda:       c6 45 d1 31             movb   $0x31,-0x2f(%rbp)
  400cde:       c6 45 d2 66             movb   $0x66,-0x2e(%rbp)
  400ce2:       c6 45 d3 66             movb   $0x66,-0x2d(%rbp)
  400ce6:       c6 45 d4 6d             movb   $0x6d,-0x2c(%rbp)
  400cea:       c6 45 d5 65             movb   $0x65,-0x2b(%rbp)
  400cee:       c6 45 d6 66             movb   $0x66,-0x2a(%rbp)
  400cf2:       c6 45 d7 61             movb   $0x61,-0x29(%rbp)
  400cf6:       c6 45 d8 62             movb   $0x62,-0x28(%rbp)
  400cfa:       c6 45 d9 51             movb   $0x51,-0x27(%rbp)
  400cfe:       c6 45 da 75             movb   $0x75,-0x26(%rbp)
  400d02:       c6 45 db 30             movb   $0x30,-0x25(%rbp)
  400d06:       c6 45 dc 74             movb   $0x74,-0x24(%rbp)
  400d0a:       c6 45 dd 65             movb   $0x65,-0x23(%rbp)
  400d0e:       c6 45 de 73             movb   $0x73,-0x22(%rbp)
  400d12:       c6 45 df 21             movb   $0x21,-0x21(%rbp)
```

If you've read the hint (or if you're just plain perceptive!), you may realize
that all of these bytes are alphanumeric ASCII characters. Looking this up in
an ASCII table, you'll find that it translates to

```
g1ffmefabQu0tes!
```

And that is indeed the password! From the terminal:

```
$ echo -n "g1ffmefabQu0tes!" | nc ctf2.linux.ucla.edu 7001
Welcome to my fabulous Quote of the Day dispenser!
To receive my wisdom, please enter the password.
Password: Here's the quote of the day:
Only Eat Breakfast In the Morning
```

To retrieve the md5 hash of this, you can use an online tool, or simply the
__md5sum__ utility:

```
$ echo -n "Only Eat Breakfast In the Morning" | md5sum -
51de72fc70308c816406370a71e56012  -
```

If you're a gdb buff who want to see some real reverse engineering, see [Quote
of the Day 2](../quote-of-the-day-2/). The techniques discussed there applies
to this (with a lot less work, of course).
