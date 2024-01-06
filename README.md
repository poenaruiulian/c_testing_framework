# C Testing Framework

This project was built as the final project for my Operating System course and represents a C Testing Framework that can detect C files from a folder structure given by the user, compile those files (outputting the executable files in the `Output` folder locating in the same directory as the C file) and execute the program, giving feedback on the result.

#### Steps to use the framework:

1. To properly use the framework, the folder structure mentioned above should have a specific hierarchy:

```
directory/
├── filename.c         # C program source file
├── filename_inputfiles/
│   ├── input1.txt     # Input file for test case 1 of Program1
│   ├── input2.txt     # Input file for test case 2 of Program1
│   ├── input3.txt     # Input file for test case 3 of Program1
│   └── ...            # Other input files for filename.c
├── filename_checkfiles/
│   ├── checkfile1.txt # Check file for test case 1 of Program1
│   ├── checkfile2.txt # Check file for test case 2 of Program1
│   ├── checkfile3.txt # Check file for test case 3 of Program1
│   └── ...            # Other check files for filename.c
└── ...                # Other files that are located in the directory,
                       # including other subdirectories containing C programs
```

The framework executing function relies on this structure, the number of `input` and `checkfile` does not have any restrictions.

2. To find the path to the folder structure and search in each directory from the structure we use the `get_path_for_each_directory` function, which needs two parameters:
- `folderStructure` - refers to the absolute path to the folder structure
- `firstFolderInTheStructure` - refers to the first folder in the structure
  
An example would be: `/Users/poe/Desktop/proiect_so/director1/director2/director3/director4` and `director1`. These are necessary for the framework so we can test any folder structure from our unit, without the structure itself being in the framework's directory. 


#### Some observations:

1. I used C for the language of the project, so I had access to the `dirent.h` library which helped me read files from directories. Also, I have used the `system` function for some of the functionalities.
2. Some of the bits are not 100% optimised, for example, the values from the **input** and **checkfile** files I have used char[100] which could be overflowed.
3. The code itself is in `main.c` file and for every function (and some other block of codes) I have written a short documentation for a better understanding of the code.




