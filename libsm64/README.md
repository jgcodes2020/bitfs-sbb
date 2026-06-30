# libsm64
Bundled binaries for libsm64.

## Unlocking
Place your SM64 ROMS in a `roms/` folder inside this one (it's been `.gitignore`'d for this purpose). ROMs should be named `sm64-<version>.z64` (and be in .z64 format).

Install `cryptography` via `pip`, then do the following:
```
python3 fernet-lock.py unlock-all <versions>
```
where `<versions>` is one or more of `us`, `jp`, `eu`, `sh`.

The Python script can also be used to lock or unlock individual DLLs as needed.

> **NOTE:** Unencrypted DLLs should ***never, ever** be checked into Git. I do not wish to be visited by the Nintendo ninjas.