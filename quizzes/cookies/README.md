## Cookies

__Question__

```
How many cookies does ctf.linux.ucla.edu use?
```

__Hint__

_None_

__Answer__

```
7
```

__Points__

```
10
```

## Solution

Cookies are pieces of data a website tells a browser to store on its behalf.
They're typically used to maintain state for a website, such as which user the
browser is logged in as, or what is in the shopping cart.

The simplest way to look at cookies is to use the browser's developer console
and inspect the Cookies tab. Doing so will show 7 cookies.

Unfortunately, a pure HTTP tool like __curl__ may not work here, due to the
possibility of a website using Javascript and AJAX to set additional cookies
beyond the initial page load.
