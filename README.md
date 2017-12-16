# DUE6502
Arduino DUE driver for 6502

To generate C load modules from assembly, e.g. for TinyBasic, use:

    ../win2c64/win2c64 -P TinyBasic.s ; ../ptf2c/PTF2C TinyBasic.ptf

Then edit Main.cpp to use the entry-points for the program loader.

