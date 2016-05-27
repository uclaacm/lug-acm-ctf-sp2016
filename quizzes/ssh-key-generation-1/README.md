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


