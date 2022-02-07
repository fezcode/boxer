# Boxer

Creates archieve files with instructions given in a `boxerfile`

## Building
This project uses `CMake` and `Ninja` build system.


For first time:
```bash
cmake -B build -GNinja
ninja -C build
```

Just for compiling:
```
ninja -C build
```

## Commands

### ARCHIVE_NAME [NAME]
     Sets name of box/archive. Adds `.tar.gz` at the end of name.

### ADD_DIR <DIR_NAME> [AS <OUTPUT_DIR>]
     Adds directory named `DIR_NAME` to box.
     If `OUTPUT_DIR` is given then directory name is changed to `OUTPUT_DIR`.
     
### ADD_FILE <FILENAME> [AS <DEST_FILENAME>]
     Adds current `FILENAME` to box. 

### ADD_FILE_MATCHING <REGEX> IN <DIR> GET <ALL|LATEST|FIRST|LAST> [AS <OUTPUT_FILENAME> ] TO <DESTINATION>
     Add file with regular expression.
     Options:
     <REGEX>         : [must] is used to get matching files.
     IN              : [must] 
     <DIR>           : [must] search regex in.
     GET             : [must] 
     <SELECTOR>      : [must] pick one below
                     |-> ALL:    Add all files matching.
                     |-> LATEST: Get only latest modified.
                     |-> FIRST:  Get first match.
                     |-> LAST:   Get last match.
     TO              : [must]
     <DESTINATION>   : [must] destination directory.
     AS              : [optional] is used to get matching files.
     OUTPUT_FILENAME : [optional] must exists if AS is given.

### !!!WARNINGS
 - Beware of leading `/` in archive filenames (AS part)


## Tests
You can find example `.boxerfile` in `test-files` directory.
In project directory, run:

```bash
build/boxer -f test-files/.boxerfile
```

to run tests.