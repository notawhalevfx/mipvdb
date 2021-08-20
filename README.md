# mipvdb
mipvdb is a standalone application that generates MIP vdb for volumetrics data. Can be useful for heavy scenes that use a lot of volumetric data to minimize memory footprint. At this moment support only Linux based system.

## Usage:
    Usage: ./mipvdb [options] in.vdb out.%l.vdb

    Generic options:
    --version                   Print version
    -h [ --help ]               Help message.
    -v [ --verbose ]            Verbose mode.

    VDB MIP options:
    -l [ --levels ] arg         Numbers of MIP levels to generate. 
                                Default: 2
    -i [ --interpolation ] arg  Voxel interpolation point, box or quadratic. 
                                Default: box
    -n [ --naming ] arg         Naming style numbers or words (half, quater, 
                                eight, sexteenth, thirtysecond). 
                                Default: numbers
    -o [ --onefile ]            Write all MIP in one file. 
                                Default: Off

## Installing
```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/mipvdb ../
make
make install
```

## Dependency
Package | Version | Automatically Installed by Cmake
--- | --- | ---
OpenVDB | v8.1.0 | YES
tbb | 2018_U6 | YES
blosc | v1.5.0 | YES
jemalloc | 5.2.1 | YES
boost | 1.56 | NO