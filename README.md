# fastmemmanager
A header-only memory manager template to avoid creating and destroying objects continuously in fast C/C++ applications.

Using this templat you can create all the objects your application will use at the beginning and then request them as you need them and return them as they are are disposed.
Of course you need to take care of avoiding memory leaks but at the end it will tell you if you forgot to return any object.

It's only one header file you can include in your project. There is no library.

You can also define one initial allocation size in number of objects with increment size and max number of increments.

The Makefile is only used to create unit tests.
Unit tests use Google unit test gtest. If you want to run unit test you need to install gtest and modify the Makefile in test folder to indicate the root for google test.

At this moment you can see how the header can be used in the unit test source code. Later I'll add an example.

You can use this source as you please for any project at your own risk.
I try to create this code error-free but there is not guarantee.

Thank you
Rodolfo
