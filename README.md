# Simple cache simulator 
This is a NRU cache replacement policy simulator.

## Usage
After compile the main program
```bash
./a.out config/cache1.org bench/reference1.lst index.rpt
```
where project is your binary code, cache1.org, reference1.lst are input files,
index.rpt is the output file name. (The input files are ASCII format.)
Output the total cache miss count into the output file, i.e. index.rpt.

### How does it works ?
We consider byte addresses and byte addressable cache systems.
Give a cache with the M-bit address bits, B-byte block size, E cache sets, and using
A-way set associativity. There are O = log2 B bits offset, and we need to select 
𝐾 = log2 E bits among upper (M-O) address bits for indexing the cache. There are totally \binom {M-O}{K}  possible valid indexing schemes. 
We done two tghings:
 (i) implement the NRU replacement policy and the LSB indexing scheme and 
 (ii) find a valid indexing scheme with minimal cache misses

### Input/output file example

Following input/output file format is a requirement in this project. You get no
point without any excuse for any format violation

In this project, the index of the first bit always has the biggest indexing number.
Meanwhile, the index of the last bit is always 0. For example, the last address in the
following file, reference2.lst, is 00101100, which indicates that the bit indexes 7, 6, 4,
1, 0 (𝑎7, 𝑎6, 𝑎4, 𝑎1, 𝑎0) are 0 and the bit indexes 5, 3, 2 (𝑎5, 𝑎3, 𝑎2) are 1.

**cache2.org**
```json
Address_bits: 8
Block_size: 4
Cache_sets: 4
Associativity: 2
```
**reference2.lst**
```json
.benchmark testcase1
00000000
00010000
00100000
00000000
00101100
00000000
00101100
.end
```
As shown previously (the first part of this project), the indexing bits can simply
use the LSB, i.e. the lower bits. In the following case, the least significant bits (LSBs)
are used as the indexing bits, i.e. 3 and 2. The final miss count is 5.
**index.rpt**
```json
Address bits: 8
Block size: 4
Cache sets: 4
Associativity: 2
Offset bit count: 2
Indexing bit count: 2
Indexing bits: 3 2
.benchmark testcase1
00000000 miss
00010000 miss
00100000 miss
00000000 miss
00101100 miss
00000000 hit
00101100 hit
.end
Total cache miss count: 5
```

Also (the second part of this project), the indexing bits can be carefully selected
to achieve a lower miss count. In this case, the bits 5 and 4 are used as the indexing
bits. The final miss count is reduced to 4.

**index.rpt**
```json
Address bits: 8
Block size: 4
Cache sets: 4
Associativity: 2
Offset bit count: 2
Indexing bit count: 2
Indexing bits: 5 4
.benchmark testcase1
00000000 miss
00010000 miss
00100000 miss
00000000 hit
00101100 miss
00000000 hit
00101100 hit
.end
Total cache miss count: 4
```