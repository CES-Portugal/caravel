# CARAVEL
## Table of contents
- **[General info](#general-info)**
- **[Project Structure](#project-structure)**
- **[Tools and Libraries](#tools-and-libraries)**
- **[Usage](#usage)**
- **[Credits](#credits)**

## General info
This project was made during my internship at Continental Engineering Services, its objectives are:
- Recreate the behaviour of a node in a CAN network
- Serve as an interface for dynamic sending and receiving of CAN messages
- Have scripting options to rerun “commands”

## Project Structure
It can be decomposed into two parts:

1. Simple command line program
2. Interactive program

The first part only allows for executing one command at a time.</br>
The second uses the properties of the **_caf_** library, so that multiple commands can be executed.</br>
Both parts allow for the parsing of simulation files, some examples are available in the [test](./test) folder.</br>
Received network packets, are logged to a .PCAP file and saved to the [logs](./logs) folder, in which for the first part will always be the _output.pcap_, as for the second part the files will have the name of the time they were created, due to the possibility of trying to receive messages more than one time in a single program run.

## Tools and Libraries
- [Conan](https://conan.io/)
- [Boost.Program_Options](https://www.boost.org/doc/libs/1_63_0/doc/html/program_options.html)
- [PcapPlusPlus](https://pcapplusplus.github.io/)
- [C++ Actor Framework](https://www.actor-framework.org/)

## Usage

#### 1. Run the binary caravel file:
```
$ ./caravel --help
```
All the available commands will be displayed.

#### 2. Execute the desired command:
Examples:
```
$ ./caravel send --id 123 --message AA
$ ./caravel receive
$ ./caravel --file PATH_TO_FILE
$ ./caravel interactive
```
To enter the interactive mode you must use the last command shown above (possible inputs for this mode are present in the [simulation](./test/simulation.txt) file).</br>
In order to run a simulation in the interactive mode you may input `simulate FILE_PATH`.

#### 3. End the program:
Part 1 - Use Ctrl-C.</br>
Part 2 - Insert `exit` to the command line.

## Credits
Credits to [Continental Engineering Services](https://www.linkedin.com/company/conti-engineering/) for proposing and assisting in the project's development.
