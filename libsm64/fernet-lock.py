#!/usr/bin/python3

from argparse import ArgumentParser, ArgumentError

from typing import Callable
from pathlib import Path

parser = ArgumentParser(
    description="Locks and unlocks libsm64 with a ROM.",
    exit_on_error=True
)
parser.set_defaults(func=lambda x: parser.print_usage())
subparsers = parser.add_subparsers(
    dest="p_subcmd", 
    required=True, 
    metavar="<subcmd>"
)
unlock_parser = subparsers.add_parser("unlock", 
    help="Unlocks libsm64."
)
unlock_parser.add_argument("input",
    help="The input.",
    type=Path
)
unlock_parser.add_argument("-k", "--key",
    help="The ROM key to use.",
    dest="rom",
    type=Path,
    required=True
)
unlock_parser.add_argument("-o", "--output",
    help="The output file.",
    dest="output",
    type=Path,
    required=True
)

lock_parser = subparsers.add_parser("lock", 
    help="Locks libsm64."
)
lock_parser.add_argument("input",
    help="The input.",
    type=Path
)
lock_parser.add_argument("-k", "--key",
    help="The ROM key to use.",
    dest="rom",
    type=Path,
    required=True
)
lock_parser.add_argument("-o", "--output",
    help="The output file.",
    dest="output",
    type=Path,
    required=True
)

args = parser.parse_args()

import struct
import base64

from cryptography.fernet import Fernet
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

# Code copied from https://github.com/branpk/wafel/blob/b0f47ef7813092529945950445915e25f60e92fd/wafel/game_versions.py
# AKA Wafel 0.7.1
def get_key(rom: bytes) -> bytes:
    kdf = PBKDF2HMAC(  # type: ignore
        algorithm=hashes.SHA256(),
        length=32,
        salt=b'',
        iterations=10_000,
    )
    key = kdf.derive(rom)
    return base64.urlsafe_b64encode(key)


def encrypt(rom: bytes, plain_dll: bytes) -> bytes:
    key = get_key(rom)
    return Fernet(key).encrypt(plain_dll)


def decrypt(rom: bytes, cipher_dll: bytes) -> bytes:
    key = get_key(rom)
    return Fernet(key).decrypt(cipher_dll)


def swap_v64(b: bytes) -> bytes:
    length = len(b) // 2
    ints = struct.unpack('<' + str(length) + 'H', b)
    return struct.pack('>' + str(length) + 'H', *ints)


def swap_n64(b: bytes) -> bytes:
    length = len(b) // 4
    ints = struct.unpack('<' + str(length) + 'I', b)
    return struct.pack('>' + str(length) + 'I', *ints)


def rom_to_z64(rom: bytes) -> bytes:
    head = struct.unpack('>I', rom[:4])[0]
    swap_fn: Callable[[bytes], bytes] = {
        0x80371240: lambda b: b,
        0x37804012: swap_v64,
        0x40123780: swap_n64,
    }[head]
    return swap_fn(rom)


def lock(lib_p: Path, rom_p: Path, clib_p: Path):
    rom = b""
    lib = b""
    with open(lib_p, "rb") as lib_f, open(rom_p, "rb") as rom_f:
        rom = rom_to_z64(rom_f.read())
        lib = lib_f.read()
    clib = encrypt(rom, lib)
    with open(clib_p, "wb") as clib_f:
        clib_f.write(clib)


def unlock(clib_p: Path, rom_p: Path, lib_p: Path):
    rom = b""
    clib = b""
    with open(clib_p, "rb") as clib_f, open(rom_p, "rb") as rom_f:
        rom = rom_to_z64(rom_f.read())
        clib = clib_f.read()
    lib = decrypt(rom, clib)
    with open(lib_p, "wb") as lib_f:
        lib_f.write(lib)
        
if args.p_subcmd == "lock":
    lock(args.input, args.rom, args.output)
elif args.p_subcmd == "unlock":
    unlock(args.input, args.rom, args.output)