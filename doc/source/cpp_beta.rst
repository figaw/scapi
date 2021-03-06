.. _cpp_beta:

SCAPI in C++ (Beta)
===================

Until recently Scapi was a java-only library that in some cases wrapped native elements using JNI.
We recently started to implement a c++ version of Scapi.
Java-Scapi is great for fast development of application and for POCing cryptographic protocols.
However, when performance is a main concern, a c++ implementation might be more suitable.

This is still "work in progress" and should be considered as beta. 
Currently the main part that was implemented is the "primitive layer" of scapi
but the scapi team is working these days on expending the c++ implementaion further.

.. _Linux:

Prerequisites on Linux
---------------------
There are a few prerequisites you must install before being able to compile scapi on your machine. 
You'll need to install:

1. Install `git`_
2. Install the `gcc`_ compiler environment: `gcc`, `make`, `ar`, `ld`, etc.
3. Install `boost`_
4. Install `cryptopp`_
5. Install `OpenSSL`_

On Ubuntu environment is should look like: ::

  $ sudo apt-get update
  $ sudo apt-get install build-essential
  $ sudo apt-get install git
  $ sudo apt-get install libboost-all-dev
  $ sudo apt-get install libcrypto++9 libcrypto++9-dbg libcrypto++-dev
  $ sudo apt-get install libssl-ocaml-dev libsslcommon2-dev
  
.. _Source:

Installing Scapi from Source (On UNIX-based Operating Systems)
--------------------------------------------------------------

In order to install scapi: ::

  $ git clone git://github.com/cryptobiu/scapi.git
  $ cd scapi/src/cpp/ScapiCpp
  $ make
  
.. _Tests:

Running the tests
-----------------

When building c++ Scapi, a file called ``ScapiTests.exe`` is created. Exceuting this file will run all the c++ related tests.
All of them should pass and you should get a notification about it.

.. _Link:

Likning against the scapi lib
-----------------------------

We inlcuded few sample files for quick start: ::

  $ cd scapi/src/cpp/CodeExamples
  $ g++ -std=c++11 -static simple_dlog.cpp ../ScapiCpp/scapi.a -lcrypto -lcrypto++ -ldl -lc
  $ ./a.out

Running the exectuble that was created (``a.out``) should produce few prints after using the OpenSSL implementation of Dlog. 

.. _Further: 

Further Reading
---------------

For furthre reading - refer to the extensive Java documentation. 
The c++ implementation usally follows the same concept except when language specific need required some change.
 
  
.. _git: http://git-scm.org/
.. _gcc: http://gcc.gnu.org/
.. _boost: http://www.boost.org/
.. _cryptopp: https://www.cryptopp.com/
.. _OpenSSL: https://www.openssl.org/
