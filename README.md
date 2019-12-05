# BLASTn
An implementation of the basic local alignment search tool (Blast) for nucleotides. The goal of this project is to implement the BLASTn algorithm in C++ and interop with an FPGA implementation of the Smith-Waterman algorithm to greatly accelerate the word extension stage of BLASTn.

## Process
The BLASTn algorithm performs operations on DNA sequences in several steps, which together make up the parts of the algorithm. The process in which BLASTn operates is shown in Fig. 1.

Fig. 1. The BLASTn process
![BLASTn Process](docs/blastn-flowchart.png)

## Members
Electrical and Computer Engineering Senior Project at California State Polytechnic University, Pomona
- Alden Param
- Alex Chan
- Hmayak Apetyan
- Jacob London
- Simon Tutak
- Sivaramakrishnan Prabakar

## Advisors
- Mohamed El-Hadedy (Mohamed Aly)
- Mostafa M. Hashim Ellabaan

## Project Reporting
- [Paper](https://docs.google.com/document/d/1efzbBsXzkEi7pNegTqy0G0EC0baw4Xfw3ayusAVXP1I/edit?usp=sharing)
- [Presentation](https://docs.google.com/presentation/d/148pHGbZyhRuX7aTDIOD6cvWq6DEl1R97VqgUeb22P9o/edit?usp=sharing)
- [Poster](docs/blastn-poster.png)

## Cloning
```
# HTTPS
$ git clone https://github.com/Reconfigurable-Computing-CalPoly-Pomona/Reconfigurable-BLAST-N.git --recurse-submodules

# SSH
$ git clone git@github.com:Reconfigurable-Computing-CalPoly-Pomona/Reconfigurable-BLAST-N.git --recurse-submodules
```

## Building
For the Docker container, run `docker build -t blastn .` to create the environment with all necessary dependencies. BLASTn will be placed under `/root`. For building the C++ system, see the [C++ README](cpp/README.md).

## Dependencies
| Implementation | Dependency |
|----------------|------------|
|    GNU C++     | C++17, g++7.4+, GNU Make 4.2.1+ |
|   Visual C++   | Visual Studio Suite 2019 |
|    Python      | Python 3.6+, pip3, numpy, tqdm |
|    Vivado      | Vivaduo 2019.1 |

## Future Work
Future work would entail implementing the entire BLASTn algorithm on the FPGA. Although the majority of performance gain can be accomplished by hardware accelerating the Smith-Waterman portion of the algorithm, there is value in having the entire algorithm running end to end on the board. The bottleneck in the process is the transmission rate of data between the computer and the FPGA, something that could be solved in two different ways. The first is to have the entire algorithm run on the FPGA. The second would be to use a faster communication method, such as PCIE. Additionally, there is still some benchmarking that could be done to show the true speed of our implementation. More runtimes could be tested by testing our algorithm on multiple FPGAs, and even creating arrays of FPGAs to try to see how fast our version of BLASTn could run.  Another improvement to this project could be on the implementation of the Smith-Waterman algorithm itself. During our hardware implementation of the Smith-Waterman, we came up with a few different promising ideas that could result in a speed-up. Unfortunately, we had to abandon these ideas in the interest of time and choose our stable, but slower, implementation of Smith-Waterman (that was successfully implemented on the FPGA).
Additionally, during our implementation of the Smith Waterman Algorithm in VHDL, we were unable to properly simulate propagation delay. Unfortunately, the digital logic does not capture the delays that would occur in our real-world implementation. This could be addressed by an FPGA implementation that accounts for analog properties such as propagation delay between gates. As a result, our simulation displays an immediate reaction and does not properly illustrate the implementation we developed.

## Sponsors
- [Xilinx Inc.](https://www.xilinx.com/)
- [IBM](https://www.ibm.com/us-en/)
- [Center for Cognitive Computing System Research](https://www.c3sr.com/)
