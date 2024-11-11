#include <stdio.h>
#include <string.h>

#define ROUNDS 16  // Number of Feistel rounds

void input_string(char *input, char *prompt) {
    int valid_input = 0;

    while (!valid_input) {
        printf("%s", prompt);
        scanf("%8s", input);

        // Check if the length of the string is exactly 8
        if (strlen(input) == 8) {
            int is_binary = 1;
            for (int i = 0; i < 8; i++) {
                if (input[i] != '0' && input[i] != '1') {
                    is_binary = 0;
                    break;
                }
            }

            if (is_binary) {
                valid_input = 1;
            } else {
                printf("Error: The string must be in binary.\n");
            }
        } else {
            printf("Error: Please enter exactly 8 bits.\n");
        }
    }
}

// Permuted Choice 1
void permuted_choice_1(const char* input, char* output) {
    int pc1_map[7] = {1, 2, 3, 4, 5, 6, 7};  // Remove bit at index 0
    for (int i = 0; i < 7; i++) {
        output[i] = input[pc1_map[i]];
    }
    output[7] = '\0';
}

// Left Circular Shift
void left_shift(char* left, int left_len, char* right, int right_len, int shift) {
    char temp_left[left_len + 1];
    char temp_right[right_len + 1];

    for (int i = 0; i < left_len; i++) {
        temp_left[i] = left[(i + shift) % left_len];
    }
    temp_left[left_len] = '\0';
    strncpy(left, temp_left, left_len);

    for (int i = 0; i < right_len; i++) {
        temp_right[i] = right[(i + shift) % right_len];
    }
    temp_right[right_len] = '\0';
    strncpy(right, temp_right, right_len);
}

// Permuted Choice 2
void permuted_choice_2(const char* input, char* output) {
    int pc2_map[6] = {0, 1, 2, 4, 5, 6};  // Remove bit at index 3
    for (int i = 0; i < 6; i++) {
        output[i] = input[pc2_map[i]];
    }
    output[6] = '\0';
}

// Generate round keys
void generate_round_keys(const char* key, char round_keys[][7], int rounds) {
    char pc1_output[8];
    permuted_choice_1(key, pc1_output);

    char left[4], right[5];
    strncpy(left, pc1_output, 3);
    left[3] = '\0';
    strcpy(right, pc1_output + 3);

    for (int i = 0; i < rounds; i++) {
        int shift = (i == 0 || i == 1 || i == 3 || i == 15) ? 1 : 2;
        left_shift(left, 3, right, 4, shift);

        char combined[8];
        snprintf(combined, 8, "%s%s", left, right);

        permuted_choice_2(combined, round_keys[i]);
    }
}

// Expansion box (4 to 6 bits)
void expansion_box(const char* input, char* output) {
    int expand_map[6] = {0, 1, 3, 2, 3, 1};  // Example expansion mapping
    for (int i = 0; i < 6; i++) {
        output[i] = input[expand_map[i]];
    }
    output[6] = '\0';
}

// S-box (6 to 4 bit)
void sbox_6to4(const char* input, char* output) {
    // 4 x 16 S-box table
    int sbox[4][16] = {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    };

    int row = (input[0] - '0') * 2 + (input[5] - '0');

    int col = (input[1] - '0') * 8 + (input[2] - '0') * 4 + (input[3] - '0') * 2 + (input[4] - '0');

    int result = sbox[row][col];

    // Convert the 4-bit output to a binary string
    for (int i = 3; i >= 0; i--) {
        output[i] = (result % 2) + '0';
        result /= 2;
    }
    output[4] = '\0';
}


// Permutation box
void permutation_box(const char* input, char* output) {
    int perm_map[4] = {1, 3, 0, 2};  // sample permutation
    for (int i = 0; i < 4; i++) {
        output[i] = input[perm_map[i]];
    }
    output[4] = '\0';
}

// XOR
void xor(char* result, const char* a, const char* b, int len) {
    for (int i = 0; i < len; i++) {
        result[i] = (a[i] == b[i]) ? '0' : '1';
    }
    result[len] = '\0';
}

// f function
void feistel_function(const char* right, const char* subkey, char* output) {
    char expanded[7];
    expansion_box(right, expanded);

    char xor_result[7];
    xor(xor_result, expanded, subkey, 6);

    char sbox_output[5];
    sbox_6to4(xor_result, sbox_output);

    permutation_box(sbox_output, output);
}

// Feistel round (encryption/decryption)
void feistel_round(char* left, char* right, const char* subkey) {
    char f_output[5];
    feistel_function(right, subkey, f_output);

    char temp[5];
    xor(temp, left, f_output, 4);

    strcpy(left, right);
    strcpy(right, temp);
}

// Feistel encryption
void encrypt(char* left, char* right, char round_keys[][7]) {
    for (int i = 0; i < ROUNDS; i++) {
        feistel_round(left, right, round_keys[i]);
    }
}

// Feistel decryption
void decrypt(char* left, char* right, char round_keys[][7]) {
    for (int i = ROUNDS - 1; i >= 0; i--) {
        feistel_round(right, left, round_keys[i]);
    }
}

int main() {
    char initial_key[9], plaintext[9];
    input_string(plaintext, "Enter plaintext (8-bit): ");
    input_string(initial_key, "Enter key (8-bit): ");

    // Generate round keys
    char round_keys[ROUNDS][7];
    generate_round_keys(initial_key, round_keys, ROUNDS);

    // Comment it out
    printf("\nRound keys:\n");
    for (int i = 0; i < ROUNDS; i++) {
        printf("Round %d key: %s\n", i + 1, round_keys[i]);
    }

    // Split plaintext into left and right halves
    char left[5], right[5];
    strncpy(left, plaintext, 4);
    left[4] = '\0';
    strcpy(right, plaintext + 4);

    // printf("\nPlaintext L0: %s, R0: %s\n", left, right);
    printf("\nPlaintext: %s%s\n", left, right);

    // Encrypt
    encrypt(left, right, round_keys);
    // printf("Ciphertext: L: %s, R: %s\n", left, right);
    printf("Ciphertext: %s%s\n", left, right);

    // Decrypt
    decrypt(left, right, round_keys);
    // printf("Decrypted: L: %s, R: %s\n", left, right);
    printf("Decrypted: %s%s\n", left, right);

    return 0;
}
