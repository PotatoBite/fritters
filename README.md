# fritters
Basic encryption utilities, for educational purposes mainly, but suitable for small, non-critical production use. Comes with no warranty. 

## How to use

This builds a static and a shared library, so link against `fritters` or `fritters_static`, and uses cmake in the process.

- Recommended way (using [collector](https://github.com/PotatoBite/collector) ):
```cmake
include("collector/collector.cmake")#need to be after project() call

...
collect( "https://github.com/PotatoBite/fritters" "v0.0.1" ${yourTarget} )

...
target_link_libraries(${yourTarget} fritters_static)
```

- Configure, build and install with cmake. Then be sure that is accessible from your code, ie(using cmake): 
```cmake
...
set (FRITTERS_DIR "C:\\fritters")

...
target_include_directories (${yourTarget} PRIVATE ${FRITTERS_DIR}/include )#add path to headers
target_link_directories (${yourTarget} PRIVATE ${FRITTERS_DIR}/lib)#add path to libs

...
target_link_libraries(${yourTarget} fritters_static)
```

- Using the source code directly(the `.cpp` files): Instead of relaying in compiled lib, you can use the folder `src` directly, and just compile the required file for the algorithm you are going to use, just need to include, `fritters/<desired_algorithm>.cpp` in one of your implementation files(`.cpp`, preferably in the main source file with the app entry point) or compile the file along as other source of your program, in the subsequent ones just need the definitions(`fritters/<desired_algorithm>.h`).

## Implemented algorithms

This is a list of the strongest, most production ready implementations, other existing may be in development, or subject to breaking changes:

- RC4 (classic implementation) `#include <fritters/RC4.h>`:


  ```c++
RC4 cipher("this is the key to use jpiyo874fiuh2in lpY%$^");
std::string message = "string to encrypt or decrypt"
cipher.inplaceCipher(message);
std::cout << message << std::endl;//message encrypted

RC4 decipher("this is the key to use jpiyo874fiuh2in lpY%$^");
decipher.inplaceCipher(message);
std::cout << message << std::endl;//"string to encrypt or decrypt"

  ```