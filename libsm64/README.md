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

## Linux note
The supplied binaries were built against EndeavourOS on 2026-06-30. They may not work on systems built against Ubuntu or Fedora. If you are running one of those distros and would like to rebuild libsm64 against your distro, please do so.

Note also that the EU and Shindou libraries required patches to the audio code. As such, I cannot guarantee their accuracy.

> **NOTE:** Unencrypted DLLs should ***never, ever** be checked into Git. I do not wish to be visited by the Nintendo ninjas.