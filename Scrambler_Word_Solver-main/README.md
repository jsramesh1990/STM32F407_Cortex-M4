# Scrambler Word Solver

![C](https://img.shields.io/badge/language-C-blue)
![Status](https://img.shields.io/badge/status-working-brightgreen)
![Dictionary](https://img.shields.io/badge/dictionary-words.txt-orange)
![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-lightgrey)

## Overview

A command-line anagram and scramble word solver written in C.

This project loads a dictionary from `words.txt`, cleans and validates words, then finds all valid English anagrams for a scrambled input.

Example:

* Input: `ew`

* Output: `we`

* Input: `stpo`

* Output:

  * stop
  * pots
  * post
  * tops
  * spot

---

## Features

* Supports word lengths from **2 to 45 letters**
* Loads large dictionary files (`words.txt`)
* Removes invalid entries automatically:

  * numbers
  * symbols
  * empty lines
* Accepts alphabet-only input
* Finds **all matching anagrams**
* Dynamic memory allocation for large word sets

---

## Project Structure

```bash
.
├── scrambler.c
├── words.txt
└── README.md
```

---

## Build

Compile using GCC:

```bash
gcc scrambler.c -o scrambler
```

Run:

```bash
./scrambler
```

---

## Working Flow

```text
Start
  ↓
Load words.txt
  ↓
Clean dictionary
  ├─ remove invalid symbols
  ├─ remove numbers
  ├─ keep only A-Z words
  └─ keep words length 2-45
  ↓
Store words in memory
  ↓
User enters scrambled word
  ↓
Validate input
  ↓
Sort input letters
  ↓
Compare with sorted dictionary words
  ↓
Display all matches
  ↓
Repeat until user enters q
  ↓
Exit
```

---

## Input Rules

Allowed:

```text
apple
stpo
ew
triangle
```

Not allowed:

```text
abc123
hello!
a
superlongwordmorethan45characters...
```

---

## Example Session

```bash
$ ./scrambler
Loaded words: 11873

Enter scrambled word: ew
Matches:
- we

Enter scrambled word: stpo
Matches:
- stop
- spot
- post
- pots
- tops
```

---

## Dictionary Notes

The quality of results depends on `words.txt`.

Recommended sources:

* ENABLE word list
* SCOWL word list
* WordNet

---

## Future Improvements

* Trie-based lookup
* Hash map optimization
* Scrabble scoring
* Hint generation
* Difficulty levels
* Puzzle generator

---

## Author

CLI scramble solver project in C for word-game experimentation and dictionary-based anagram solving.
