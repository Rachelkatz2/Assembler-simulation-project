# Assembler Project  
A C-based assembler that reads assembly (`.as`) source files and translates them into machine code files used by a linker.

## 📌 Overview
This project implements a full two-pass assembler, including macro expansion, symbol table management, instruction encoding, and error handling.

## 📁 Input
The assembler receives one or more source files with the extension `.as`.

### Example (`example.as`)
```
.extern LIST
.entry START

START:  mov  r3, r1
        add  r2, r3  
LIST:   .data 4, -2, 7
```

## 📤 Output Files
The assembler may generate up to four output files:

| Extension | Description |
|----------|-------------|
| `.am` | Source file after macro expansion |
| `.ob` | Machine code output |
| `.ext` | External labels (generated only if such labels exist) |
| `.ent` | Entry labels (generated only if such labels exist) |

## ⚙️ How It Works
The assembler runs in several phases:

### 1. Preprocessor  
Expands macros in the `.as` file and produces an `.am` file.

### 2. First Pass  
- Scans the `.am` file  
- Builds the symbol table  
- Calculates memory locations for code and data  
- Performs initial validations  

### 3. Second Pass  
- Encodes instructions and data  
- Resolves symbols  
- Handles entries/external references  
- Produces the `.ob`, `.ext`, and `.ent` files as needed  

### Error Handling  
If an error is found in a line, the assembler prints an error message and continues processing.  
Lines with errors are ignored in later phases.

## ▶️ Running the Program
The program can receive multiple `.as` files as arguments.  
Each file is processed independently.

Example:
```
./assembler file1.as file2.as
```
