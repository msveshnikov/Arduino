#include <Arduino_RouterBridge.h>

extern "C" void matrixWrite(const uint32_t *buf);
extern "C" void matrixBegin();

// Digit patterns for 0-9 (5 pixels wide each)
// Each row is 13 bits: packed across 4 uint32_t values
// Bits fill horizontally left-to-right, starting from LSB
// Format: [bits 0-31], [bits 32-63], [bits 64-95], [bits 96-103]

// Digit 0: ###
//          # #
//          # #
//          # #
//          ###
const uint32_t digit0[] = {
    0x07, // Row 0: 111 (reversed: 111)
    0x05, // Row 1: 101 (reversed: 101)
    0x05, // Row 2: 101 (reversed: 101)
    0x05, // Row 3: 101 (reversed: 101)
    0x07, // Row 4: 111 (reversed: 111)
    0x00, // Row 5: empty
    0x00, // Row 6: empty
    0x00, // Row 7: empty
};

// Digit 1:  #
//          ##
//           #
//           #
//          ###
const uint32_t digit1[] = {
    0x02,
    0x06,
    0x02,
    0x02,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Digit 2: ###
//            #
//          ###
//          #
//          ###
const uint32_t digit2[] = {
    0x07,
    0x01,
    0x07,
    0x04,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Digit 3: ###
//            #
//          ###
//            #
//          ###
const uint32_t digit3[] = {
    0x07,
    0x01,
    0x07,
    0x01,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Digit 4: # #
//          # #
//          ###
//            #
//            #
const uint32_t digit4[] = {
    0x05,
    0x05,
    0x07,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
};

// Digit 5: ###
//          #
//          ###
//            #
//          ###
const uint32_t digit5[] = {
    0x07,
    0x04,
    0x07,
    0x01,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Digit 6: ###
//          #
//          ###
//          # #
//          ###
const uint32_t digit6[] = {
    0x07,
    0x04,
    0x07,
    0x05,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Digit 7: ###
//            #
//           #
//          #
//          #
const uint32_t digit7[] = {
    0x07,
    0x01,
    0x02,
    0x04,
    0x04,
    0x00,
    0x00,
    0x00,
};

// Digit 8: ###
//          # #
//          ###
//          # #
//          ###
const uint32_t digit8[] = {
    0x07,
    0x05,
    0x07,
    0x05,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Digit 9: ###
//          # #
//          ###
//            #
//          ###
const uint32_t digit9[] = {
    0x07,
    0x05,
    0x07,
    0x01,
    0x07,
    0x00,
    0x00,
    0x00,
};

// Array of pointers to digit patterns
const uint32_t *digits[10] = {
    digit0, digit1, digit2, digit3, digit4,
    digit5, digit6, digit7, digit8, digit9};

// Display buffer for the 13x8 matrix
uint32_t displayBuffer[4];

// Function to display a 2-digit number (0-99)
void displayNumber(int num)
{
    // Clamp number to 0-99
    if (num < 0)
        num = 0;
    if (num > 99)
        num = 99;

    // Extract tens and ones digits
    int tens = num / 10;
    int ones = num % 10;

    // Clear display buffer
    displayBuffer[0] = 0;
    displayBuffer[1] = 0;
    displayBuffer[2] = 0;
    displayBuffer[3] = 0;

    // Build each row by combining tens digit (left) and ones digit (right)
    // Each row is 13 bits packed as: [bits 0-31][bits 32-63][bits 64-95][bits 96-103]
    // Tens digit: columns 0-2 (3 pixels) - reversed
    // Gap: column 3
    // Ones digit: columns 4-6 (3 pixels) - reversed

    // Helper function to reverse 3 bits
    auto reverseBits = [](uint32_t val) -> uint32_t
    {
        return ((val & 0x01) << 2) | (val & 0x02) | ((val & 0x04) >> 2);
    };

    uint32_t row0 = reverseBits(digits[tens][0] & 0x07) | (reverseBits(digits[ones][0] & 0x07) << 4);
    uint32_t row1 = reverseBits(digits[tens][1] & 0x07) | (reverseBits(digits[ones][1] & 0x07) << 4);
    uint32_t row2 = reverseBits(digits[tens][2] & 0x07) | (reverseBits(digits[ones][2] & 0x07) << 4);
    uint32_t row3 = reverseBits(digits[tens][3] & 0x07) | (reverseBits(digits[ones][3] & 0x07) << 4);
    uint32_t row4 = reverseBits(digits[tens][4] & 0x07) | (reverseBits(digits[ones][4] & 0x07) << 4);
    uint32_t row5 = 0;
    uint32_t row6 = 0;
    uint32_t row7 = 0;

    // Pack rows into buffer (13 bits per row = 104 total bits)
    // Distribution: 32 + 32 + 32 + 8 bits
    displayBuffer[0] = row0 | (row1 << 13) | (row2 << 26);
    displayBuffer[1] = (row2 >> 6) | (row3 << 7) | (row4 << 20);
    displayBuffer[2] = (row4 >> 12) | (row5 << 1) | (row6 << 14) | (row7 << 27);
    displayBuffer[3] = (row7 >> 5);

    // Write to matrix
    matrixWrite(displayBuffer);
}

void setup()
{
    Bridge.begin();
}

void loop()
{
    int temperature;
    bool ok = Bridge.call("get_temperature").result(temperature);

    if (ok)
    {
        if (temperature < 0)
            temperature = 0;
        displayNumber(temperature);
    }
    delay(1000);
}
