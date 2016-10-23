How to add unified version message in your program
==================================================

libwbmqtt-dev provides wbmqtt/version.h helper file which allows you to
add unified message for --version flag of your program.

This message will look like this:

```
$ wb-some-util --version
wb-some-util version 0.1
Git revision 6c3fd4979ee6ecbb69cf56c6c6180a4ee4bc7857
Build date: Oct 23 2016 17:11:34

Copyright (c) 2016 Contactless Devices LCC
Distributed under the MIT licence
Written by User Name <user.name@example.com>.
```

It collects all this data automatically using preprocessor arguments from Makefile.

* Git revision - from Git itself;
* Version number - from debian/changelog;
* Tool name, copyright string and writers names - from defines as shown below.


Add in your main.cpp
--------------------

```
...

#define WBMQTT_NAME "wb-some-util" // program name to display in first line
#define WBMQTT_COPYRIGHT "2016 Contactless Devices LLC" // string to add after "Copyright (C) "
#define WBMQTT_WRITERS "User Name <user.name@example.com> and Other User <other.user@example.com>" // last string after "Written by "

// Also, you can _override_ version and/or commit information by writing this (without quotes):
// Avoid this! Auto-fill while building is much clearer way to keep things correct!
#define WBMQTT_VERSION 0.1.2
#define WBMQTT_COMMIT 1234abcd

#include <wbmqtt/version.h>

// Here you can use VERSION_MESSAGE() macro which returns a whole message string
// Print this message in your argument parser logic using, for example, this:
cerr << VERSION_MESSAGE();

...
```

Add in your Makefile
--------------------
```
...
# your C++ flags or something like this
CXXFLAGS=...
...

# get Git revision from git itself
GIT_REVISION:=$(shell git rev-parse HEAD)

# get version from changelog 
DEB_VERSION:=$(shell head -1 debian/changelog | awk '{ print $$2 }' | sed 's/[\(\)]//g')

# append this data to CXXFLAGS or whatever like this you prefer to use
CXXFLAGS += -DWBMQTT_COMMIT="$(GIT_REVISION)" -DWBMQTT_VERSION="$(DEB_VERSION)"

...

# your build targets

...

```


Other tips and tricks
=====================

Overriding license
------------------

By default, this helper use "MIT license" string to describe license. If you want to change it, define this before including wbmqtt/version.h:

```
...
#define WBMQTT_LICENSE "GPLv3 license"
...
#include <wbmqtt/helper.h>
```


Mark 'dirty' Git tree
---------------------

Git tree is 'dirty' if there are uncommited changes in build environment. It's quite dangerous for package building, because you can lose your
uncommited changes and it will be impossible to restore exactly the same state of the package.

To detect this, add the following in your Makefile:

```
...
# your C++ flags
CXXFLAGS=...
...

ifeq ($(shell git diff-index --quiet HEAD --; echo $$?), 1)
CXXFLAGS += -DWBMQTT_DIRTYTREE
endif

```

This will add red blinking "GIT TREE WAS DIRTY DURING BUILD!" in your version message.

Also, you may want to print such message during package building, so you can improve your Makefile by adding this:

```
...
ifeq ($(shell git diff-index --quiet HEAD --; echo $$?), 1)
CXXFLAGS += -DWBMQTT_DIRTYTREE

RED_BLINK=$(shell echo -e "\033[31m\033[5m")
RESET_FORMAT=$(shell echo -e "\033[0m")
$(info $(RED_BLINK))
$(info GIT TREE WAS DIRTY DURING BUILD!)
$(info $(RESET_FORMAT))

endif
...
```
