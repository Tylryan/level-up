# LEVEL UP
> In no particular order

# Description
The goal of this repository is to "level up" not only one's programming abilities (that could be done with just leet code), it is also to level up one's understanding of how computer applications work on a fundamental level.
Each project embraces the "do it from scratch" idealology with the aim to demistify the magic of many, more complex, applications. Additionally, my personal goal for each project is that they each be done in C or C++ (simply to take advantage of it's built in libraries).

**The source code for this repository not only might have bugs, it probably does. Use at your own risk.**

# Parsing/Transpiling
## A Basic Calculator
### About
Writing a basic calculator that can evauate expressions using operator precidence will probably be a good first step for learning anything to do with Parsing. Learning this concept will allow you to start getting an idea of how Transpilers work. 
### Concepts
- Lexing
- Parsing
- Operator Precedence
- Abstract Syntax Trees
### Examples
- Write a program that takes in user input and prints out the evaulated output to the console.  
  - (6 + 2) * (3 + 4) -> 56  
  - ((1 + 2) * 4) / 2 -> 6  
- Write the same program, except instead of using user input, read it from a file.  
### Next Steps
After this, you could probably figure out:
- Parsing Json
- **Potentially**: Convert Markdown to some other markup language. (This is more transpiling)
- **Potentially**: Write a program that reads source code (file/functions/comments) and creates documentation for it. (This is more transpiling)

## A Basic Regular Expression Engine
### About
This will probably be more tricky, but the same concepts from the calculator will be used.


# Interpreters / Compilers
An excellent source for learning about Interpreters, Transpilers, and Compilers is [Crafting Interpreters](https://craftinginterpreters.com/contents.html) by Robert Nystrom.
- [] Compiler / Interpreter / Transpiler  
  - Lexical Analysis  
  - Static Analysis  
  - Semantic Analysis  
  - Code Generation  
  - Optimizing  


# Computer Graphics
## About

## Examples
- [] Simple TUI Library from scratch
- [] Simple GUI Library from scratch
- [] 2D Rendering
  - ROMs for Emulators
  - Games
	- SDL & OpenGL
	- Basic Physics
	- Basic Collision
 - MS Paint App (For creating sprites for your games)
- [] Basic Window Manager
  - [Suckless DWM](https://dwm.suckless.org/) is a basic tiling window manager.
- [] Data Visualization
  - 2D Graphs etc
  - 3D Modeling

# Emulators
## About

## Examples
Build a software emulator for an old gaming console or computer system. 
Emulate the hardware components and allow users to run vintage games or software on modern computers.
- [] Chip 8 Emulator
	- [Austin Morlan - Chip8 Emulator](https://austinmorlan.com/posts/chip8_emulator)  
	- Expected Completion: 6-8 HRs  
- [] NES Emulator
- [] Contribute to [DUCKSTATION](https://github.com/stenzek/duckstation)
- [] Contribute to [PCX2](https://pcsx2.net/)


## File Compression
- [] Lossless Compression  
	- Huffman coding  
	- Run Length Encoding (RLE)  
	- Burrows-Wheeler Transformation (BWT)  
- [] Lossy Compression (images/audio)  

# Networking
- [] A simple Rest API starting from TCP
- [] Packet Sniffer
- [] Port Sniffer
- [] Network Protocol Implementation (Simplified Versions)
  - TCP
  - UDP
- [] Wifi Signal Strength Analyzer
  - Scan and analyze Wifi signal strengths from different locations
  - Visualize signal strength
- [] Wifi Hotspot manager
  - Create a tool that allows a user to connect to Wifi hotspots
  - Strength signal information [see bullet above]
- [] Wifi File Sharing application
  - Allow devices to share files via wifi WITHOUT using the internet
  - Get names of devices connected to wifi
  - Choose which device to send data to
  - Using sockets transfer files from one device to another
  - Checkout data encryption
- [] Bluetooth device locator
  - Find the strength of a bluetooth connection to a device.
  - When that device gets closer, the strength should increase

# Web Browsers
- Create a simple web browser
 - [] Navigating to websites 
 - [] displaying web pages
 - [] bookmarking favorite sites
 - [] Html parsing -> graphics

# Security / Cryptography
- Implement Cryptographic Algorighms
  - RSA
  - AES
  - SHA
- [] Create encryption and decryption functions

- Security Tools
  - [] Password Cracker
  - [] Intrusion Detection System
  - [] Vulnerability Scanner

# Database Systems
- [] Build a simple database engine
  - Supports basic SQL queries
  - indexing
  - query optimization
  - transaction management
  - Efficient data storage and retrieval mechanisms
- [] Database file system
  - Create a file system that integrates with a database
  - Store large sets of data efficiently
  - Manage file metadata
  - handle concurrent access

# AI
- [] A simple neural network

# Digital Circuit Simulator
- Build a digital circuit simulator that allows users to design and simulate logic circuits. 
  - basic logic gates
  - flip-flops
  - combinational circuits. 
  - Visualize the circuit behavior and simulate different input scenarios.

# Operating Systems
- [] Kernel Development
  - Multitasking
  - Memory Management
  - File Systems
  - System Calls
  - Scheduling Algorithms
- [] RTOS
  - Task Scheduling
  - Interprocess Communication
  - Hardware Abstraction

# Other Projects
## Text Editor
- Create a basic text editor

## Basic Version Control system
- Create a basic version of Git

## Text Based Search Engine
- Build a simple text-based search engine that indexes a collection of documents and allows users to search for specific keywords. 
  - Implement basic ranking algorithms to display search results.
##  Example
  - An example of something like this would be [FZF](https://github.com/junegunn/fzf) by Junegunn Choi  

## Memory Allocator
- [] Implement a dynamic memory allocator similar to the malloc and free functions in C/C++. 
- Implement an Arena allocator
- Explore algorithms like first-fit, best-fit, and worst-fit for memory allocation and deallocation.
