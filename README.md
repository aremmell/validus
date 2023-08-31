# Validus

![Codacy grade](https://img.shields.io/codacy/grade/cd00cab5af0347d887bb202a85a3dc9c?logo=codacy) [![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_validus&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_validus) [![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_validus&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_validus)

Validus: a high-throughput, yet beefy 192-bit OWHF written in (*reasonably*) portable C11/17.

<!-- SPDX-License-Identifier: MIT -->
<!-- Copyright (c) 2004-current Ryan M. Lederman <lederman@gmail.com> -->

<!-- toc -->

- [Intro](#intro)
- [TL;DR](#tldr)
- [Disclaimer](#disclaimer)
- [Building from source](#building-from-source)
  - [Build products](#build-products)
- [CLI Interface](#cli-interface)
- [Full Documentation](#full-documentation)
- [Releases](#releases)
- [Latin](#latin)

<!-- tocstop -->

## <a id="intro" /> Intro

I know the world doesn't need another hash function; I began this project 19 years ago. Things were different then: CPUs were not even a hundredth of what they are now; ditto for disk and memory I/O. I had an idea after looking at the source code for Ronald Rivest's MD5 algorithm which is of course world-famous (and no longer used in any serious capacity due to security concerns/collisions/rainbow tables). Between us, I approached him with my idea, and he brushed me aside and said he's already working on something similar for "MD6." I'll let history speak for itself there.

## <a id="tldr" /> TLDR

After researching and reading through every hash function's source code I could find, I thought that the fundamental way hash functions were structured at the time could be improved upon–specifically by processing larger blocks of input data in one go (192-bit seemed to be the sweet spot–it could be 512 or 1024 by now 🙃), equating to less time spent doing things other than computing the hash–such as, but not limited to: waiting on I/O, copying buffers around, pushing and popping args on and off the stack, etc.

Validus was born from the aforementioned concept, and I believe it to be a success in that regard. I no longer have access to the test and timing rigs that I built to prove it, but it is considerably faster than any other hash producing a 192-bit fingerprint that I am aware of. In fact, it's within ~1% of FNV-1a @ 64-bit the last time I checked, which is essentially a 10 LOC algorithm (Validus is hundreds of LOC).

## <a id="disclaimer" /> Disclaimer

I am *not* making any claims about cryptographic security in Validus (I am not a cryptologist), nor do I guarantee it not vulnurable to collisions, but it does pass the [Avalanche Criterion](https://arishs.medium.com/analyze-your-hash-functions-the-avalanche-metrics-calculation-767b7445ee6f) (*1 bit of input changes = 50% of output bits change*).

Validus is now in the public domain because it's a waste to leave it sitting in a private repository. It's been cleaned up a bit, and there's a CLI interface to hash files and strings and such.

All questions, comments, and criticisms are welcome–especially if you find a bug, typo, or flaw (or you use it in a cool project–I'd love to have a list here of projects utilizing it).

## <a id="building-from-source" /> Building from source

CMake: there's a presets file, and Ninja is set as the default build tool. If you have VS Code, you just need the CMake tools extension and you can build and run with a few keystrokes. Without VS Code (or experience with CMake), as long as you have a reasonably recent version of CMake, the following should do the trick: `cmake --preset release && cmake --build build`.

### <a id="build-products" /> Build products

Upon a successful build, the following are created[^1]:

- `build/validus`: CLI application
- `build/libvalidus.a`: Static library
- `build/libvalidus.so`: Shared library

[^1]: The exact filenames and extensions are platform-dependent. For example, on Windows, you will get
`validus.exe`, `validus_static.lib` and `validus_shared.dll`.

## <a id="cli-interface" /> CLI interface

Once CMake finishes building, the CLI app will be located at `build/validus`. The following is a breakdown of its basic usage:

```log
validus usage:
        -s string Hash string and output fingerprint
        -f file   Hash file and output fingerprint
        -p        Performance evaluation test
        -t        Verify that Validus is functioning correctly
        -v        Display version information
        -h        Show this message
```

Most of these are self-explanatory. The `-t` option causes the algorithm to hash a known set of strings, with a predefined known correct output. If the output is green, Validus is working correctly; if it's red, something has gone wrong during compilation and it is probably a bug. Please [file an issue](https://github.com/aremmell/validus/issues/new) if you encounter this situtation!

## <a id="full-documentation" /> Full Documentation

Thanks to Doxygen, Validus has a [dedicated documentation site](https://validus.rml.dev).

## <a id="releases" /> Releases

Some tweaks to the build configuration need to take place, but sooner rather than later, static/shared libraries and the CLI app will be packaged (at least for macOS, Windows and Linux on x86_64 & aarch64). In the mean time, building with CMake will have to do.

## <a id="latin" /> Latin

If you read this far, you might as well know that Validus means "strong, robust, able" in Latin. Hopefully someone doesn't break it the first week it's on GitHub.
