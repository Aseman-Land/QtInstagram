# libqinstagram
It's an experimental library and Is under the development to work with instgram API using Qt libraries.
It's free, opensource and released under the LGPL license.

# Build

It's easy to build it. install Qt and dependencies:

```bash
sudo apt-get install g++ git qt5-default qtbase5-dev
```

And then build it:

```bash
git clone https://github.com/Aseman-Land/QtInstagram.git
cd QtInstagram
mkdir build && cd build
qmake .. PREFIX=/usr
make
sudo make install
```

# How to use it

It's easy too. Just add below line to the `.pro` file on your project

```perl
QT += instagram
```

And include it in the source files:

```c++
#include <QtInstagram>
```



