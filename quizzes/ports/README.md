## Ports

__Question__

```
What ports are open on ctf.linux.ucla.edu?

(Give a sorted list of port numbers, e.g. "5 9 50 204")
```

__Hint__

```
There are more than just TCP ports...
```

__Answer__

```
22 80 443 5000 9000
```

__Points__

```
20
```

## Solution

This was one of the trickier quiz problems. For background, computers interface
with the network through ports. Ports can range from 1 to 65535. When a server
runs a service (such as a website), it listens on a specific port (usually port
80 for HTTP) and waits for other machines to connect to it via that port. As
such, an __open port__ is one that a service is listening from.

To scan for open ports on a system, a common command line tool people use is
__nmap__. For this challenge, if one simply runs nmap (e.g. `nmap
ctf.linux.ucla.edu`), they would find that ports 22 (SSH), 80 (HTTP), 443
(HTTPS), and 9000 (HHVM) are open.

However, they would miss port 5000, the reason being that it is a _UDP port_.
Nmap does not scan UDP ports by default, and one would need to pass the `-sU`
flag to tell it to do a UDP scan. Because of the nature of UDP scans however,
running `nmap -sU ctf.linux.ucla.edu` would take a fairly long time. Another
default of nmap is to scan the top 1000 most commonly used port numbers; thus,
one optimization that can be tried is to scan fewer, perhaps the top 100 ports
instead. There are other options that can help speed it up even further, but
`nmap -sU --top-ports 100 ctf.linux.ucla.edu` would've been sufficient to find
the pesky UDP port in a reasonable amount of time.
