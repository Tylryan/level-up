# CLIBS
## Description
At the moment, these libraries only work on Unix like operating systems. Source files get compiled into [ARCHIVE](https://en.wikipedia.org/wiki/Archive_file) files. Once compiled, you can install them. The default installation paths are as follows.
- **Archive Files:** `/usr/local/lib`
- **Header Files:** `/usr/local/include`

## Instructions
- `make` in order to compile all the source code in this directory.  
- `make clean install` To install (Linux) all source code in this directory.  
- `make uninstall` To uninstall (Linux) all source code.  
> Note: You can install individual libraries as well by going into a library's directory and running `make install`.

**As with the rest of code in this repo, there may (probably) will be bugs**
