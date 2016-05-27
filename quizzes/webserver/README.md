## Webserver

__Question__

```
What webserver is acm.cs.ucla.edu running? (Give the answer as
<server>/<version>)
```

__Hint__

_None_

__Answer__

```
nginx/1.4.6
```

__Points__

```
10
```

## Solution

This quiz required a bit of knowledge about HTTP. Common webservers like Apache
or Nginx, when serving requests, often add a `Server` line into the response
header to let users know which HTTP server they're talking to.

There are several methods of retrieving the headers. For web developers,
probably the most intuitive method is to just fire up their browser's developer
console and check the response headers. There will be a clear `Server:
nginx/1.4.6 (Ubuntu)` line.

Another easy method is to use _curl_ to retrieve the headers, e.g. `curl -I
acm.cs.ucla.edu`.
