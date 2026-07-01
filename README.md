# BitFS SBB bruteforcer

The pieces of an automated script to do slow bully battery. Some pieces heavily adapted from [Tyler Kehne's platform walking bruteforcer](https://github.com/TylerKehne/sm64-tas-scripting).

I'm attempting to keep everything as maintainable and platform-independent as possible, so that anyone can contribute.

## Contents
- `modules/` - a set of libraries for working with `libsm64` and creating segmented auto-TASing workflows.
- `libsm64/` - prebuilt binaries for `libsm64`.
- `drivers/` - test drivers and bruteforcers built on the modules.