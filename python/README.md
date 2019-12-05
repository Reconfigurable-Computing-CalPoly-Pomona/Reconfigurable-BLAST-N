# Python Blastn
The Python implementation of Blastn. This design is the prototype design of the C++ version, and is less complete than the C++ implementation.
## Running
Execute the command `$ python3 main.py` to utilize the default values, or specify with command line arguments.
## Dependencies
* Install the `requirements.txt` file.
## Command Line Arguments
| Argument | Description | Default Value |
|----------|------------------------------------------|---------------|
| `-q`     | Path to query file                       | `data/SRR7236689--ARG830.fa`
| `-d`     | Path to data file                        | `data/Gn-SRR7236689_contigs.fasta`
| `-l`     | The word length.                         | `11`
| `-dt`    | The minimum Dust filter threshold.       | `1`
| `-ma`    | The Smith-Waterman score for a match.    | `2`
| `-mi`    | The Smith-Waterman score for a mismatch. | `-1`
| `-g`     | The Smith-Waterman score for a gap.      | `-1`
