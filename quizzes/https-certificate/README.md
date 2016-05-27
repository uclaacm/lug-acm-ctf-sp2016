## HTTPS certificate

__Question__

```
When does the certificate for ctf.linux.ucla.edu expire? (mm/dd/yyyy)
```

__Hint__

_None_

__Answer__

```
08/14/2016
```

__Points__

```
10
```

## Solution

SSL certificates are what enables websites to use HTTPS and thereby encrypt
their traffic to and from the user. Certs have an expiration date, after which
browsers will treat it as invalid and present a scary warning to the user.

The simplest way to find information about a website's certificate is to visit
it with a browser and use the browser's certificate viewer. This is typically
invoked by clicking on the lock icon on the URL bar and clicking show details
or more information.

On a common Unix system, it is also possible to invoke the __openssl__ tool to
retrieve certificate information:

```
openssl s_client -showcerts -connect ctf.linux.ucla.edu:443 </dev/null \
    2>/dev/null | openssl x509 -noout -enddate
```

This will show the expiration date in GMT. To achieve the date in the answer,
this expiration date needs to be converted to PST.
