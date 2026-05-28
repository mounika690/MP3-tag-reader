# MP3-tag-reader
# MP3 Tag Reader and Editor in C

## Overview

This project is a simple MP3 Tag Reader and Editor developed in C language.
It reads and modifies ID3v1 metadata tags from MP3 files.

The application can:

* View MP3 metadata
* Edit MP3 tags
* Display song information
* Handle ID3v1 tags

---

## Features

* Read MP3 tag information
* Edit:

  * Title
  * Artist
  * Album
  * Year
  * Comment
* Display metadata clearly
* File handling using C
* Structure-based implementation

---

## Technologies Used

* C Programming
* File Handling
* Structures
* Command Line Interface

---

## Project Structure

```text
include/   -> Header files
src/       -> Source files
sample_mp3 -> Sample MP3 files
```

---

## Compilation

Using GCC:

```bash
gcc src/*.c -Iinclude -o mp3tag
```

Run:

```bash
./mp3tag sample.mp3
```

---

## Example Output

```text
Title   : Song Name
Artist  : Artist Name
Album   : Album Name
Year    : 2024
Comment : Demo File
```

---

## Learning Outcomes

* File operations in C
* Binary file handling
* Structures in C
* Modular programming
* Metadata processing

---

## Future Enhancements

* Support for ID3v2 tags
* GUI version
* Playlist management
* Batch editing

---

## Author

Developed a project in C programming.
