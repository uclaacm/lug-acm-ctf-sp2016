## Forgetful Andy

__Description__

```
Andy has forgotten his password - the system administrators can reset it, but
he has a bad habit of password reuse, and has asked them to try to retreive it
from /etc/shadow so he can recover his myspace. This being a small company, the
administrators decided to help him.
```

__Hint__

```
Andy's myspace account wasn't very secure - even sha-512 can't help him.
```

__Flag__

```
1atlanta
```

__Points__

```
35
```

## Solution

There is a common utility for brute-forcing passwords - which is the only
option for a SHA-512 salted shadow file - called John the Ripper. It takes a
list of password attempts, and tries all of them.

The intention of this puzzle was to find the utility, and potentially hte clue
about his myspace account, to guess that Andy uses one of the common myspace
passwords. There is a list of many password files at
[skullsecurity](https://wiki.skullsecurity.org/Passwords), and this includes a
Myspace password dictionary. Then simply use John the Ripper
```
john --wordlist=myspace.txt ./shadow
```
To find the password - it is a relatively small list, so it should finish soon.
