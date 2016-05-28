## Infiltration 1

__Description__

```
Your cousin's kid has been spamming your Twitter feed with links to his
"awesome new website". Bored one afternoon, you took a closer look and realized
that what he has is more than just a webpage....

http://ctf3.linux.ucla.edu/~shadow/
```

__Hint__

```
This kid is not very good with passwords
```

__Flag__

```
jvstdwhcxxgejypr
```

__Points__

```
25
```

## Solution

The `shadow` directory contains the home directory of the `shadow` user. The
`public_html` inside is what you see when you go to
`ctf3.linux.ucla.edu/~shadow`.

The description of this challenge is a bit subtle. It suggests URL fuzzing
(similar to [Apache](../apache/)), but all it's trying to say is that, more
than just a webpage, the kid has an account in the ctf3.linux.ucla.edu server.
This is suggested by the "~shadow" in the URL pathname, which is how
traditional multi-user servers give webspaces to their users.

If there's an account, there's a way to login:

```
ssh shadow@ctf3.linux.ucla.edu
```

What is the password? One might try the common ones like "password", or
"password1", but for this challenge, a clue exists in the webpage itself. In
particular, this remark:

```
my favorite comic is xkcd!
```

To get to this point, it's really a matter of whether you're familiar with xkcd
and it clicks right away, or how good your google-fu is (in my defense, [Cicada
3301](https://en.wikipedia.org/wiki/Cicada_3301) features _way_ more obscure
references!). The famous xkcd comic in question is [Password
Strength](https://xkcd.com/936/), which cites "correcthorsebatterystaple" as an
example of a good password. That is also the exact password for the shadow
user.

After successfully logging in, the flag will be sitting right there in the home
directory.
