#include <stdio.h>
#include <stdbool.h>
void convertPrintBinary(int number, char* bits);
void decimalToBinary(int number, char* bits);
void clearBinary(char* bits);
void printBinary(char* bits);


    
int main(int argc, const char *argv[]) {
    // Storage for user entered number
    int number = 0;
    bool valid = false;
    // Creation of 32 bit array for binary conversion
    char bits[32] = {
        '0', '0', '0', '0', '0', '0', '0', '0', 
        '0', '0', '0', '0', '0', '0', '0', '0', 
        '0', '0', '0', '0', '0', '0', '0', '0', 
        '0', '0', '0', '0', '0', '0', '0', '0', 
    };
    
    // Prompting user to enter a number
    while(!valid) {
        printf("Please enter a number between 0 and 1000: \n");
        // Saving user input to 'number' variable
        scanf("%d", &number);
        // Ensuring user entered value is valid
        if(number < 0 || number > 1000) {
            printf("The number entered is invalid.");
        }
        else {
            valid = true;
        }
    }
    // Printing the number entered
    printf("The number entered is: %d\n", number);
    // Converting the number to binary and printing it out
    printf("The number in binary is: ");
    convertPrintBinary(number, &bits[0]);
}

// Executing all functions (clear, conversion, and print)
void convertPrintBinary(int number, char* bits) {
    clearBinary(&bits[0]);
    decimalToBinary(number, &bits[0]);
    printBinary(&bits[0]);
}


// Method for printing the binary number
void printBinary(char* bits) {
    for(int i = 31; i >= 0; --i){
        printf("%c", bits[i]);
    }
    printf("\n");
}

// Method for converting to binary
void decimalToBinary(int number, char* bits) {
    for(int i = 0; number > 0; i++) {
        // Converting an int to a char by adding '0' to the ASCII value
        bits[i] = (number % 2) + '0';
        number = (number / 2);
    }
}

// Method for clearing the binary array 
void clearBinary(char* bits) {
    decimalToBinary(0, &bits[0]);
}


