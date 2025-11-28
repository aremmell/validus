# Validus

![Codacy grade](https://img.shields.io/codacy/grade/cd00cab5af0347d887bb202a85a3dc9c?logo=codacy) [![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_validus&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_validus) [![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_validus&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_validus)

Validus: An extremely fast and portable (C17) 192-bit OWHF.

<!-- SPDX-License-Identifier: MIT -->
<!-- Copyright (c) 2004 Ryan M. Lederman <lederman@gmail.com> -->

<!-- toc -->

- [Intro](#intro)
- [TL;DR](#tldr)
- [Disclaimer](#disclaimer)
- [Building from source](#building-from-source)
  - [Build products](#build-products)
- [CLI Interface](#cli-interface)
- [Documentation](#documentation)
- [TODO](#todo)
- [Validus?](#latin)

<!-- tocstop -->

## <a id="intro" /> Intro

I know the world doesn't need another hash function; I began this project 19 years ago. Things were different then: CPUs were not even a hundredth of what they are now; ditto for disk and memory I/O. I had an idea after looking at the source code for Ronald Rivest's MD5 algorithm which is of course world-famous (and no longer used in any serious capacity due to security concerns/collisions/rainbow tables). Full diclosure: I approached him with my idea, and he brushed me aside and told he's already working on something similar for "MD6." I'll let history speak for itself there.

## <a id="tldr" /> TL;DR

After researching and reading through every hash function's source code I could find way back when, I thought that the fundamental way hash functions were structured at the time could be improved upon–specifically by processing larger blocks of input data per iteration (192-bit seemed to be the sweet spot–it could be 512 or 1024 by now 🙃), equating to less time spent doing things other than computing the hash. These are things such as, but not limited to: waiting on I/O, copying buffers around, pushing and popping args on and off the stack, etc.

Validus was born from my curiosity about testing my theory, and I believe it to be a success in that regard. I no longer have access to the test and timing rigs that I built to prove it, but it is considerably faster than any other OWHF producing a 192-bit fingerprint that I am aware of. In fact, it's within ~1% of FNV-1a @ 64-bit the last time I checked.

## <a id="disclaimer" /> Disclaimer

I explicitly *don't* claim that Validus is cryptographically secure, or even correct (I am not a cryptologist), nor do I guarantee it is not vulnurable to collisions, but it does pass the [Avalanche Criterion](https://arishs.medium.com/analyze-your-hash-functions-the-avalanche-metrics-calculation-767b7445ee6f) (*1 bit of input changes = 50% of output bits change*).

Validus is now in the public domain because it's a waste to leave it sitting in a private repository. It's been cleaned up a bit, and there's a CLI interface to hash files and strings and such.

All questions, comments, and criticisms are welcome–especially if you find a bug, typo, or flaw.

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

The CLI app located at `build/validus` is used as follows:

```log
validus usage:
        -s string Hash string and output fingerprint
        -f file   Hash file and output fingerprint
        -p        Performance evaluation test
        -t        Verify that Validus is functioning correctly
        -v        Display version information
        -h        Show this message
```

Most of these are self-explanatory. The `-t` option causes the algorithm to hash a known set of strings, with a predefined known correct output. If the output is green, Validus is working correctly; if it's red, something has gone wrong during compilation and it is probably an architecture-related bug. Please [file an issue](https://github.com/aremmell/validus/issues/new) if you encounter this situtation!

## <a id="documentation" /> Documentation

Thanks to Doxygen, Validus has a [dedicated documentation site](https://validus.rml.dev).

## <a id="todo" /> TODO

These items remain incomplete at the time of this writing:

- Robust test suite for use in CI pipeline.
- Use new features available in C23.
- Update CMake configuration, as they have added many warnings since 2023 (_status quo_).
- Create binaries for common OS/arch configurations.

## <a id="latin" /> Validus?

If you read this far, you maybe interested to know that Validus means "strong, robust, able" in Latin.
