# C++ Blastn
Blastn implemented in C++. This implementation is designed to be the fast implementation which will interop with an FPGA implementation of the Smith-Waterman algorithm.
## External Libraries
* https://github.com/greg7mdp/parallel-hashmap
## Building
### Windows
  * Open `Blastn.sln` in Visual Studio 2019.
  * Visual Studio Setup
    * Open project properties
    * On the top, select `Configuration: All Configurations`
    * On the top, select `Platform: All Platforms`
    * Go to VC++ Directories
      * Select `Include Directories -> Edit...`
        * Click in the `New Line` button (shaped as a folder)
        * Enter `$(ProjectDir)\include\parallel-hashmap`
        * Click `OK` at the bottom
    * Go to `C/C++ -> Optimization`
      * Select `Optimization: Maximum Optimization (Favor Speed) (/O2)`
      * Select `Favor Size or Speed: Favor Fast Code (/Ot)`
    * Go to `C/C++ -> Preprocessor`
      * Select `Preprocessor Definitions: <Edit...>`
        * In definitions, enter `_CRT_SECURE_NO_WARNINGS`
        * This allows for the timestamp system to work as `<ctime>` is used.
    * Go to `C/C++ -> Language`
      * Select `C++ Language Standard: ISO C++17 Standard (/std:c++17)`
  * On the top, specify `Release`, `x64`, and press `Ctrl + Shift + B` to build.
  * The generated executable will be `x64\Release\Blastn.exe`.
### Mac and Linux
  * In the `cpp` directory, run the `make` command.
  * The generated binary will be `blastn`.

## Command Line Arguments
| Argument       | Description                    | Default Value |
|----------------|--------------------------------|---------------|
| `-query`       | Path to query file             | `data/SRR7236689--ARG830.fa`
| `-subject`     | Path to data file              | `data/Gn-SRR7236689_contigs.fasta`
| `-out`         | Output file                    | `result.txt`
| `-sep`         | Query / subject seperator      | `>`
| `-word-length` | Word length                    | `11`
| `-sw-minscore` | Smith Waterman min score       | `20`
| `-sw-match`    | Smith Waterman match score     | `2`
| `-sw-mismatch` | Smith Waterman mismatch score  | `-3`
| `-sw-gap`      | Smith Waterman gap score       | `-1`
| `-dust-thresh` | Dust threshold                 | `0.1`
| `-dust-length` | Dust pattern length            | `3`
| `-lambda`      | E-value lambda constant        | `0.267`
| `-kappa`       | E-value kappa constant         | `0.041`
| `-spacing`     | Spacing between output columns | `6`
| `-fpga`        | Score using an FPGA through UART | `COM4`
| `-test`        | Expects `dust`, `extend`, `match`, `pairs`, `sequence`, `sw` or `smith waterman`, `sort`, `split` | NA
