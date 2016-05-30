## HerpDerp protocol

__Description__

```
HerpDerp Co distrusts SSL, and instead likes to use its own proprietary
datagram protocol (designed by an intern) for securely communicating amongst
its services. While sniffing traffic in a coffee shop, you managed to capture a
few packets from a client talking to a HerpDerp Co server.

You suspect that there are probably vulnerabilities in the protocol. Decrypt
the messages to find the flag.

(NOTE: the flag for this problem is guaranteed to be less than 8 characters)
```

__Hint__

```
Choose your mask (search space) carefully
```

__Flag__

```
tvziubz
```

__Points__

```
40
```

## Solution

`packets.zip` is the attachment containing the four packet content files.

Note that I do __NOT__ proclaim to be a cryptographer, so take the follow
analysis with a grain of salt, and verify any details. Please report
inaccuracies!

This is an exercise in analyzing protocols. The majority of applications and
devices (like car locks, internal apps) communicate using undocumented
protocols that are secure only because they're obscure. This is triply
dangerous when the designers try to "roll their own crypto" protocol instead of
using an established one like TLS.

Given that we only have the static packets, we definitely aren't trying to
break the communication using active attacks. Let's take a look at packet1:

```
HerpDerpSecureV0.1
Mode:
AES-128-CBC
IV:
0D5296940CC220619E54222436D0BDC6
Checksum:
8FC42C6DDF9966DB3B09E84365034357
Message:
¦Ý±3-ª^W7^Y½Q5<97>[¸<91>
```

The encrypted message shows up as garbled text in my text editor. If we are to
trust its header information, it seems to be a message based communication
protocol using symmetric AES-128 (CBC mode) for encryption. Since there are no
mention of keys, presumably the sender/receiver have already shared a key
beforehand.

To someone inexperienced with crypto usage, this might look daunting. What's
CBC? What's an IV? Is there math involved? The solution is actually
surprisingly simple.

Google "8FC42C6DDF9966DB3B09E84365034357", and you get a result that tells you:

```
Decoded hash Md5: 8fc42c6ddf9966db3b09e84365034357: the
```

The checksum is actually the MD5 hash of the message!

An aside about encrypted communication. A useful system should ensure a number
of properties, amongst which are confidentiality, integrity, and authenticity
(CIA). Encrypting just the message with a symmetric cipher only ensures
confidentiality. Real cryptosystems use, e.g. a
[MAC](https://en.wikipedia.org/wiki/Message_authentication_code) to ensure the
other two properties. The HerpDerpSecureV0.1 protocol, however, only tries to
ensure confidentiality and integrity, and using an MD5 checksum at that. This
is so woefully inadequate that it is a vulnerability in itself (even if the
encryption was used correctly, which I can't say for certain was).

This challenge then reduces to reversing four MD5 hashes. By no coincidence,
the first three decrypts to "the", "flag", "is". The fourth,
`E83D7F7F60338FDF40C527E7B4BB9D9D`, is a bit harder, with no results online.

For cracking raw hashes, both __John the Ripper__ (jumbo edition) and
__Hashcat__ are good enough for the job. Since JtR was used so much in the
other challenges, I'll show Hashcat usage here.

```
$ hashcat-cli64.bin -a 3 -m 0 --increment <(echo -n "E83D7F7F60338FDF40C527E7B4BB9D9D") "?l?l?l?l?l?l?l"
...
e83d7f7f60338fdf40c527e7b4bb9d9d:tvziubz     
                                             
All hashes have been recovered
```

And the flag is found (cracking time omitted ;). This command runs hashcat
(CPU-only version) with:

* `-a 3`: brute-force mode
* `-m 0`: MD5 hash
* `--increment`: try 1 character, 2 characters, 3 characters, etc.
* `?l?l?l?l?l?l?l`: the character mask (7 chars lowercase)

The problem description states that the flag will be 7 or fewer characters, but
it being all lowercase is an assumption on my part. Leaving aside the fact that
I know for sure the flag is all lowercase... it's a reasonable assumption to
make, given that 26^7 = 8 billion hashes. The alternative is to assume upper
and lowercase alphanumeric, which is 62^7 = 3.5 __trillion__ hashes. It would
take 500x longer to run the latter, so even if the former assumption doesn't
work out, its cost is negligible in the grand scheme of things. These kinds of
tricks are useful if you're in a time-constrained setting (like a CTF), or if
you have a toaster of a machine (even just 8 billion hashes took me 15
minutes to compute!).
