## Infiltration 2

__Description__

```
(Prerequisite: Infiltration 1)

The kid appears to be sharing the server with a rather interesting person.
You'd bet he keeps some interesting things in his home directory.
```

__Hint__

```
Trump has long but simple passwords. Attack accordingly.
```

__Flag__

```
qjcwfvghpvqtnzau
```

__Points__

```
40
```

## Solution

The `shadow` file is the `/etc/shadow` of ctf3.linux.ucla.edu, filtered to only
dtrump's entry. The `dtrump` folder is the home directory of the `dtrump` user.

Since [Infiltration 1](../infiltration-1) is a prerequisite for this challenge,
this means that at this point you're already able to login to shadow at
ctf3.linux.ucla.edu. This challenge requires a small bit of exploration (hence,
it's a Forensics challenge), namely that you should realize either by listing
`/home/` or checking `/etc/passwd` that there's a "dtrump" user in the server.

Unfortunately, you have no permission to see what's in his home directory. What
to do now? There are many directions you can take, but one classic approach of
hackers who have infiltrated a system is to steal the password files
`/etc/passwd` and `/etc/shadow`.

A brief sidenote about password files. In old Unix systems, user account
information is stored in `/etc/passwd`, which lists the names and password
hashes. This was a problem - because the passwd file stored important
information, it must be readable by all users. But then any user can take the
passwd file and crack the hashes in it! Thus, people moved the password hashes
to a file called `/etc/shadow` instead. By doing this, the shadow file can be
made unreadable to normal users, while keeping public information inside the
passwd file.

But interestingly, you're able to read that file!

```
shadow@ctf3:~$ ls -l /etc/shadow
-rw-r----- 1 root shadow 753 May 25 10:10 /etc/shadow
shadow@ctf3:~$ groups
shadow
```

The lesson here is to never make a user called "shadow". By pilfering the
shadow file, you can now run a password cracker on it to recover the password
of dtrump.

The classic tool for cracking password files is __John the Ripper__.
Unfortunately, the hint suggests that Trump uses "long but simple passwords".
These kinds of passwords are not as amenable to straight-up bruteforce, because
they have a large search space. An approach that is almost always better is to
do a __dictionary attack__, by giving JtR a good wordlist to work off of. If
you look for password cracking dictionaries online, you will find several. One
famous one is the rockyou corpus, which contains millions of commonly used
passwords in the real world. Once you have the dictionary, you can invoke JtR
as such:

```
john shadow -wordlist=rockyou.txt
```

Depending on your computer's speed, it may take anywhere from 1 to 5 minutes.
Eventually, JtR will tell you:

```
Loaded 1 password hash (md5crypt, crypt(3) $1$ [MD5 128/128 AVX 12x])
Will run 8 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
butiloveyou2     (dtrump)
```

Login as dtrump, and the flag will be in his home directory.
