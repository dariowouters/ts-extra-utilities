"""Minimal PE/disasm helpers for analyzing eurotrucks2.exe."""
import struct, re

EXE = r'D:\Steam\steamapps\common\Euro Truck Simulator 2\bin\win_x64\eurotrucks2.exe'
BASE = 0x140000000

_data = open(EXE, 'rb').read()
_pe = struct.unpack_from('<I', _data, 0x3C)[0]
_numsec = struct.unpack_from('<H', _data, _pe + 6)[0]
_optsize = struct.unpack_from('<H', _data, _pe + 20)[0]
_soff = _pe + 24 + _optsize
secs = {}
for _i in range(_numsec):
    _s = _soff + _i * 40
    _name = _data[_s:_s + 8].split(b'\0')[0].decode('latin1')
    _vsize, _vaddr, _rsize, _roff = struct.unpack_from('<IIII', _data, _s + 8)
    secs[_name] = (_vaddr, _vsize, _roff, _rsize)


def va2off(va):
    """VA (with image base) -> file offset, or None."""
    a = va - BASE
    for name, (vaddr, vsize, roff, rsize) in secs.items():
        if vaddr <= a < vaddr + rsize:
            return roff + (a - vaddr)
    return None


def off2va(off):
    a = off
    for name, (vaddr, vsize, roff, rsize) in secs.items():
        if roff <= a < roff + rsize:
            return BASE + vaddr + (a - roff)
    return None


def read(va, n):
    o = va2off(va)
    return _data[o:o + n]


def u8(va): return read(va, 1)[0]
def u16(va): return struct.unpack('<H', read(va, 2))[0]
def u32(va): return struct.unpack('<I', read(va, 4))[0]
def i32(va): return struct.unpack('<i', read(va, 4))[0]
def u64(va): return struct.unpack('<Q', read(va, 8))[0]


def scan(pattern, section='.text'):
    """pattern: list of ints, None = wildcard. Returns list of VAs."""
    va, vsize, roff, rsize = secs[section]
    buf = _data[roff:roff + rsize]
    rx = bytearray()
    for b in pattern:
        rx += b'.' if b is None else re.escape(bytes([b]))
    rx = re.compile(bytes(rx), re.S)
    return [BASE + va + m.start() for m in rx.finditer(buf)]


def scan_bytes(raw, section='.text'):
    va, vsize, roff, rsize = secs[section]
    buf = _data[roff:roff + rsize]
    hits = []
    i = buf.find(raw)
    while i != -1:
        hits.append(BASE + va + i)
        i = buf.find(raw, i + 1)
    return hits


def callers_of(target):
    """Find `call rel32` sites in .text that jump to target."""
    hits = []
    va, vsize, roff, rsize = secs['.text']
    buf = _data[roff:roff + rsize]
    i = buf.find(b'\xe8')
    while i != -1:
        if i + 5 <= len(buf):
            rel = struct.unpack_from('<i', buf, i + 1)[0]
            if BASE + va + i + 5 + rel == target:
                hits.append(BASE + va + i)
        i = buf.find(b'\xe8', i + 1)
    return hits


def qword_refs(target, section='.rdata'):
    """Find qwords in a section equal to target (vtable entries, pointers)."""
    va, vsize, roff, rsize = secs[section]
    buf = _data[roff:roff + rsize]
    needle = struct.pack('<Q', target)
    hits = []
    i = buf.find(needle)
    while i != -1:
        hits.append(BASE + va + i)
        i = buf.find(needle, i + 1)
    return hits


def hexdump(va, n=64):
    o = va2off(va)
    for i in range(0, n, 16):
        chunk = _data[o + i:o + i + 16]
        print(f'{va + i:#x}  ' + ' '.join(f'{b:02x}' for b in chunk))
