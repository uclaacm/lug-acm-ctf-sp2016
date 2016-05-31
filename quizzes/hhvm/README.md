## HHVM

__Question__

```
Name the version of HHVM used by this CTF site. Give it in the form
"HHVM/x.x.x"
```

__Hint__

```
Headers are helpful
```

__Answer__

```
HHVM/3.13.1
```

__Points__

```
10
```

## Solution

As with the "webserver" quiz, this can simply be found by checking
the site's headers. We recommend the [`HTTPie` utility](https://github.com/jkbrzt/httpie): 

    http -h https://ctf.linux.ucla.edu/

Which gives the result: 

    HTTP/1.1 200 OK
    ...
    X-Powered-By: HHVM/3.13.1

This also could have been using `curl`:

    curl -i https://ctf.linux.ucla.edu/

Giving the result: 

    HTTP/1.1 200 OK
    ...
    X-Powered-By: HHVM/3.13.1
    ...

The headers come back in a different order, but that line is still present.
