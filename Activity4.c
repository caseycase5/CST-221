#include <stdio.h>

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
    
    printf("The number entered is: %d\n", number);
    
    
}
