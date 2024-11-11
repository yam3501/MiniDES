# MiniDES

### By: Yash Mhaskar | Date: 2 - Oct - 24

C implementation of a Feistel cipher for 8-bit strings.

--------

## Usage

1. Clone the repository:
    ```bash
    git clone https://github.com/yam3501/MiniDES.git
    ```
2. Compile the program:
    ```bash
    gcc -o minides main.c
    ```
3. Run the program:
    ```bash
    ./minides
    ```
4. Enter 8-bit binary strings for both the plaintext and the key when prompted.

## Example
```bash
Enter plaintext (8-bit): 11001100
Enter key (8-bit): 10101010

Round keys:
Round 1 key: 100101
Round 2 key: 001010
Round 3 key: 100010
Round 4 key: 001101
Round 5 key: 100101
Round 6 key: 010101
Round 7 key: 001101
Round 8 key: 100101
Round 9 key: 010101
Round 10 key: 001101
Round 11 key: 100101
Round 12 key: 010101
Round 13 key: 001101
Round 14 key: 100101
Round 15 key: 010101
Round 16 key: 100010

Plaintext: 11001100
Ciphertext: 11000100
Decrypted: 11001100
```
## Screenshots

![screenshot](https://github.com/user-attachments/assets/0381b8db-f08c-43ef-8a30-f728a6ea4bf5)
