# Word to Number Converter

A C++ program that converts numbers written in English words to their numerical representation. This parser handles standard numbers, fractions, decimals, and negative numbers with support for magnitudes up to billions.

## Features

- Converts written numbers to digits (e.g., "fifty four" → 54)
- Handles large numbers up to billions
- Supports fractions (e.g., "three quarters" → 0.75)
- Processes decimal numbers (e.g., "twenty point five" → 20.5)
- Handles negative numbers
- Supports mixed numbers with fractions (e.g., "two and a half" → 2.5)

## Usage

The program runs in interactive mode, allowing you to input numbers in words and receive their numerical representation. To exit, type 'n', 'N', or 'exit'.

```bash
$ ./word_to_number
Enter a number in words (e.g., 'negative three thousand point five' or 'two and a half'): 
To exit, enter 'n', 'N' or 'exit'
>three billion one hundred and ninety nine million nine hundred and ninety nine thousand nine hundred and ninety nine
Numeric value: 3,199,999,999
```

## Examples

### Standard Numbers
```
Input: fifty four thousand three hundred and twenty one
Output: 54,321

Input: three billion one hundred and ninety nine million nine hundred and ninety nine thousand nine hundred and ninety nine
Output: 3,199,999,999
```

### Fractions
```
Input: three quarters
Output: 0.75

Input: two and a half
Output: 2.5

Input: two thirds
Output: 0.666667
```

### Decimals
```
Input: fifty four thousand point five
Output: 54,000.5

Input: three point one four one five nine
Output: 3.14159
```

### Negative Numbers
```
Input: negative three billion
Output: -3,000,000,000

Input: negative two and a quarter
Output: -2.25
```

## Supported Number Words

### Basic Numbers
- Zero through ninety-nine
- Standard number words (one, two, three, etc.)
- Magnitude words (hundred, thousand, million, billion)

### Fractions
- half/halves
- third/thirds
- quarter/quarters
- fourth/fourths
- eighth/eighths

## Technical Details

The converter uses a state machine approach to parse number words and handle different cases:
- Regular numbers
- Fractions
- Decimals
- Magnitudes
- Negative numbers

The code maintains a parsing state that tracks:
- Current number being built
- Running total
- Current magnitude
- Negative flag
- Decimal processing state

## Limitations

- Maximum number size is the limit for `long long` in C++(~2^63 - 1). However, max tens power is `billion/s`
- Only processes English number words
- Requires proper formatting and spacing of input
- Does not handle mathematical expressions or operations

## Building

### Prerequisites
- C++ compiler with C++11 support
- Standard C++ library

### Compilation
```bash
g++ -std=c++11(or upwards) word_to_number.cpp -o [name of .exe]
```
