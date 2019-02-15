# Squarepatch:

## Work out nx from np (np=nx\*nx\*nx):

> ./np2nx.sh 1000000

```
nx=97 np=912673
nx=98 np=941192
nx=99 np=970299
nx=100 np=1000000   <----
nx=101 np=1030301
```

and update `const int nx` in initial_square.cpp

## Generate sqpatch.txt and sqpatchTEST.bin:

```
for i in 46 58 67 74 80 84 89 93 97 100;do 
    nx=$i;
    echo nx=$nx;

    sed "s-XXXX-$nx-" initial_square_template.cpp > eff.cpp;
    CC -O3 eff.cpp -o $nx.exe;

    /usr/bin/time -p ./$nx.exe ;
    mv sqpatch.txt sqpatch.txt.$nx;
    mv sqpatchTEST.bin sqpatchTEST.bin.$nx;
done
```

## Example: 

As expected, file size is linear with np
(7 doubles: x, y, z, vx, vy, vz, p_0, 1 double=8bytes=64bits):

```
MBytes = size(double) *7 *np
For np=97336, size=8*7*97336=5450816 bytes
```

|nx|time |sqpatch.bin |np|np
|---|---|---|---|---
||(sec)|size (bytes)|||
nx=46| real 0.86    | 5450816   |np=97336      |   10^5
nx=58| real 1.76    | 10926272  |np=195112     | 2.10^5
nx=67| real 2.65    | 16842728  |np=300763     | 3.10^5
nx=74| real 3.59    | 22692544  |np=405224     | 4.10^5
nx=80| real 4.46    | 28672000  |np=512000     | 5.10^5
nx=84| real 5.26    | 33191424  |np=592704     | 6.10^5
nx=89| real 6.14    | 39478264  |np=704969     | 7.10^5
nx=93| real 7.03    | 45043992  |np=804357     | 8.10^5
nx=97| real 7.91    | 51109688  |np=912673     | 9.10^5
nx=100| real 8.73   | 56'000'000  |np=1'000'000    |   10^6