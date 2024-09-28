# Be Careful!

Below are all famous bugs that have brought down systems. If you want to experiment with any of the starter code that they give, take caution as you could lose important files or even usability of your system.

You may not be able to understand much of this at the beginning of the course. As we go through, more and more of these bugs will become clear. Systems programming doesn't have to be in C only!

## Basic Memory/C Bugs
[Vulnerabilities in IOHIDeous, Mac OS X](https://siguza.github.io/IOHIDeous/)
[Heartbleed](https://xkcd.com/1354/)
[PHP Memory Bug](http://www.inulledmyself.com/2015/05/exploiting-memory-corruption-bugs-in.html) -- Please don't use PHP
["Cloudbleed"](https://bugs.chromium.org/p/project-zero/issues/detail?id=1139)

## Famous Race Conditions
[Making money on digital ocean](http://josipfranjkovic.blogspot.com/2015/04/race-conditions-on-facebook.html),
[Awesome Race Condition](http://mailinator.blogspot.com/2009/06/beautiful-race-condition.html),
[Bitcoin Race Conditions](https://www.josipfranjkovic.com/blog/race-conditions-on-web),
[Dirty Cow, My Favorite](https://github.com/dirtycow/dirtycow.github.io/wiki/VulnerabilityDetails),
[Hyper Threading](http://gallium.inria.fr/blog/intel-skylake-bug/),
[Kernel Race Conditions](http://seclists.org/oss-sec/2013/q1/326),
Not a bug but still cool -- [Random number race generation](https://github.com/dasmithii/RCRand)

## Memory Stuff
[Memory Allocation in Python](https://lukasa.co.uk/2016/12/Debugging_Your_Operating_System/?hn=1),
[Memory Leaks](https://www.joyent.com/blog/walmart-node-js-memory-leak),
[`kfree` twice to get root](http://seclists.org/oss-sec/2017/q1/471)

## Processes
[Using waitid to escape docker](https://www.twistlock.com/2017/12/27/escaping-docker-container-using-waitid-cve-2017-5123/),
[Famous `fork` gotcha](http://rachelbythebay.com/w/2014/08/19/fork/)

## Filesystems
[Apple Filesystems](https://cxsecurity.com/issue/WLB-2015100149?),
[Ubuntu Filesystems](http://seclists.org/oss-sec/2015/q2/717)

## Event Drive IO
[`epoll` broken](https://idea.popcount.org/2017-02-20-epoll-is-fundamentally-broken-12/)

## Networking
[gethostbyname overflow](http://www.openwall.com/lists/oss-security/2015/01/27/9),
[tricking antiviruses](https://bugs.chromium.org/p/project-zero/issues/detail?id=820),
[Breaking SSL](https://guidovranken.wordpress.com/2016/02/27/openssl-cve-2016-0799-heap-corruption-via-bio_printf/)

## Scheduling
[Decades of wasted cores](http://www.ece.ubc.ca/~sasha/papers/eurosys16-final29.pdf)

## Signal
[Privilege Escalation with `kill(-1, SIGKILL)`](http://www.openwall.com/lists/oss-security/2017/10/12/1)