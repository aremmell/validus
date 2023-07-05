# validus

192-bit OWHF

## Building w/ CMake (and w/o VS Code)

Obviously, the paths need to be modified to suit.

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=build/install --check-system-vars --debug-output
-S/Users/ryan/Documents/Development/validus -B/Users/ryan/Documents/Development/validus/build -G Ninja
```
