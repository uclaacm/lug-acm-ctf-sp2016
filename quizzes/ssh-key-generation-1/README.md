## SSH Key Generation 1

__Question__

```
Give the command to generate an SSH key with these parameters (IN THIS ORDER,
meaning you list the RSA option first, then the bits options, etc.): 

* It uses RSA
* To fend off the NSA, it's 8,192 bits in length
* You want it placed in the file "my-new-key" under "Downloads" in your home
  directory

Give the command in this form: 

commandName -f optionArg -e optionArg2

(Note the single spaces between flags, and the lack of a trailing space at the
end). This ONLY AN EXAMPLE. These are NOT the actual flags and options you'll
use.
```

__Hint__

```
Manpages are your friend
```

__Answer__

```
ssh-keygen -t rsa -b 8192 -f ~/Downloads/my-new-key
```

__Points__

```
15
```

## Solution

Reading the [manpage](http://linux.die.net/man/1/ssh-keygen) for `ssh-keygen` shows that it takes many handy arguments: 

* `-t` to specify the type of key, here RSA 
* `-b` to specify the size of the key in bits (beyond 8192 is unreasonable for now)
* `-f` to specify the path to the output file (which will be created if it does not exist)


