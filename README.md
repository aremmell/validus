# Validus One-Way Hash Function

[![Build Status](https://app.travis-ci.com/aremmell/validus.svg?branch=master)](https://app.travis-ci.com/aremmell/validus)

A high-throughput, yet beefy **192-bit** OWHF written in (*reasonably*) portable C11/17. 

I know the world doesn't need another hash function; I began this project 19 years ago. Things were different then: CPUs were not even a hundredth of what they are now; ditto for disk and memory I/O. I had an idea after looking at the source code for Ronald Rivest's MD5 algorithm which is of course world-famous (and no longer used in any serious capacity due to security concerns/collisions/rainbow tables). Between us, I approached him with my idea, and he brushed me aside and said he's already working on something similar for "MD6." I'll let history speak for itself there.

## TL;DR

After researching and reading through every hash function's source code I could find, I thought that the fundamental way hash functions were structured at the time could be improved upon–specifically by processing larger blocks of input data in one go (192-bit seemed to be the sweet spot–it could be 512 or 1024 by now 🙃), equating to less time spent doing things other than computing the hash–such as, but not limited to: waiting on I/O, copying buffers around, pushing and popping args on and off the stack, etc.

Validus was born from the aforementioned concept, and I believe it to be a success in that regard. I no longer have access to the test and timing rigs that I built to prove it, but it is considerably faster than any other hash producing a 192-bit fingerprint that I am aware of. In fact, it's within ~1% of FNV-1a @ 64-bit the last time I checked, which is essentially a 10 LOC algorithm (Validus is hundreds of LOC).

I am *not* making any claims about cryptographic security in Validus (I am not a cryptologist), nor do I guarantee it is collision-free, but it does pass the [Secure Hash Avalanche Test](https://arishs.medium.com/analyze-your-hash-functions-the-avalanche-metrics-calculation-767b7445ee6f) (*1 bit of input changes = 50% of output bits change*).

I've decided to put it in the public domain, because it is a waste to leave it sitting in a private repo. It's been cleaned up a bit, and there's a CLI interface to hash files and strings and such.

All questions, comments, and criticisms are welcome–especially if you find a bug, typo, or flaw (or you use it in a cool project–I'd love to have a list here of projects utilizing it).

## Building

Straightforward CMake: a presets file, and Ninja as the default build tool. If you have VS Code, you just need the CMake tools extension and you can build and run with a few keystrokes. Without VS Code (or experience with CMake), I would recommend looking at [`configure.sh`](./configure.sh) which I wrote so that CMake installations succeeed even with ancient CMake vesions that don't support presets. In fact, you can most likely compile everything by executing `./configue.sh build`.

## Releases

Some day soon I will package up a release with some precompiled static/shared libraries and CLI app (at least for macOS, Windows and Linux).

## Latin

If you read this far, you might as well know that Validus means "strong, robust, able" in Latin. Hopefully someone doesn't break it the first week it's on GitHub.
