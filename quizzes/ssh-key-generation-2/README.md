## SSH Key Generation 2

__Question__

```
Suppose you already have an SSH key located at ~/.ssh/seasnet

It uses the old key storage format, with insecure key derivation for the
128-bit AES wrapping encryption. Give the command to convert it into the new
SSH key storage format, which uses a more KDF. You do want to change the
password, but not the number of KDF rounds. 

Give the options in this order: 

* Convert the key format
* Change the password
* Choose the keyfile (give the full path to the key identified above)

Give the command in this form: 

commandName -f optionArg -e optionArg 2

(Note the single spaces between flags, and the lack of a trailing space at the
end)
```

__Hint__

```
OpenSSH 6.5 brought some nice improvements
```

__Answer__

```
ssh-keygen -o -p -f ~/.ssh/seasnet
```

__Points__

```
15
```

## Solution

The `-o` option [invokes the use](http://www.tedunangst.com/flak/post/new-openssh-key-format-and-bcrypt-pbkdf) of the new key storage format. 
The `-p` option changes the password by prompting for the old one and then new
one. This is preferable to having it be passed in with `-P old_passphrase -N new_passphrase`, which would show up in bash history, iTerm history, etc.
Finally, `-f` specifies the output file (with a full path to avoid ambiguity).


