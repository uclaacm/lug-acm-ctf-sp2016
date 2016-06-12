## Evil Andy

__Description__

```
(Prerequisite: Forgetful Andy)

After all the trouble of getting his password reset, Andy was fired for leaking
company secrets. All we have is his home directory after he shredded most of
the files. We need to figure out what information was leaked.
```

__Hint__

```
There are some tools for hiding information in images, and Andy still likes to
reuse passwords.
```

__Flag__

```
L3AhNuC#k>N^f}(CTK7B
```

__Points__

```
25
```

Good practice when doing any sort of forensics is to check automatic system
logs. In this case, there is a file in the home directory called
`.bash_history` - it stores recent commands run by the user. In this case, it
shows Andy emailing the file `Tux.jgp` to someone at the competing company.

Given the fact that Tux.jpg is suspicious, but the image file itself appears to
contain only a penguin, it is probably that Andy tried to hide the information
in it in some other way. This is a field of cryptography called stegonagraphy,
hiding information such that, whether or not it's encrypted (better if it is),
it is simply invisible to an unknowing observer.

There are a few common tools to do this in Linux. The one used in this case was
steghide, and the password (when prompted with `steghide -sf Tux.jpg`) was the
same as Andy's user password - 1atlanta. This would yield the secret file.

## Solution


