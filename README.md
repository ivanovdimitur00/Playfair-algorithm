# Playfair-algorithm
A fun little project, describing how the Playfair algorithm works, using c++. 
<br/>
<br/>

## Table of contents
0. [Project status](#Project-status)
1. [Description](#Description)
2. [Algortihm explanation](#Algorithm-explanation)
3. [Key features and functionalities](#Key-features-and-functionalities)
4. [Structure](#Structure)
5. [Installation instructions](#Installation-instructions)
6. [Dependencies](#Dependencies)
7. [System requirements](#System-requirements)
8. [Usage examples](#Usage-examples)
9. [API references](#API-references)
10. [Bugs](#Bugs)
11. [Future improvements](#Futute-improvements)
<br/>

## Project status
Completed and working. Not expecting changes.
<br/>
<br/>

## Description
The purpose and goal of the project is to create a program, demonstrating a cryptography algorithm. The topic of the Playfair Algorithm was chosen. 

The intended audiences are the teacher and assistant in my cryptographic methods class and myself.
<br/>
<br/>

## Algorithm explanation
[Example from wikipedia](https://en.wikipedia.org/wiki/Playfair_cipher#:~:text=cipher.%5B13%5D-,Example,-%5Bedit%5D)
<br/>
<br/>

## Key features and functionalities
* Plain text message and keyword input;
* Plain text message and keyword truncation. Option to see results;
* Encryption using the Playfair algorithm. Option to see result;
* Decryption using the Playfair algorithm. Option to see result;
* Visuals for the generation of the playfair matrix and bigraph substitution;
* Capability to restore truncated white spaces and some symbols.
<br/>

## Structutre
* The project is a Monolithic C++ file;
* There is documentation included inside the source, as well as some ideas.
<br/>

## Installation instructions
1. Access the source code from this repository and download it;
2. Using an IDE and/or a compiler for C++, compile for your computer system and launch the executable.
> [!TIP]
> The options to see some results are present in the main() function.
<br/>

> [!NOTE]
> you can use [This online c++ compiler](https://www.onlinegdb.com/online_c++_compiler) to compile and run the code.
> No extra settings or flags required.
<br/>

## Dependencies
* C++ compiler. OR
* internet connection
<br/>

## System requirements
| System Requirements |
| ------------------- |
| X86 capable CPU     |
<br/>

## Usage examples
1. Input the message that you want to see truncated and encrypted;
2. Input the key that you want to be used in the encrypting process.
> [!TIP]
> There is a [sample text file](./input.txt) for input.
<br/>

## API references
Some functions that you need to know to edit the source code would be:
* `showExtractedSymbols(list<int>& , list<char> )` - shows the truncated symbols, including whitespaces and the position of each symbol in the string.
<br/>

## Bugs
 - [x] No bugs have occurred or have been documented.  
<br/>

## Future improvements
- [ ] Multiple language support (might need `wstring`);
- [ ] Figure out a way to dynamically create the alphbet matrix and the matrix for used letters (given supported languages and symbols);
  - [ ] Most likely a separate file (.CSV?) and a config file.
- [ ] Split the monolithic file into separate files;
- [ ] Add a config file for some processes to be made in/visible;
- [ ] ~rewrite the substitution (replace() ) function to be more readable~
