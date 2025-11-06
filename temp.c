#include <Arduino_RouterBridge.h>

// Function declarations for Arduino UNO Q LED Matrix
extern "C" void matrixWrite(const uint32_t* buf);
extern "C" void matrixBegin();

// Digit patterns for 8x13 LED matrix - explicit 6 columns (each column = 8 bits)
// Each digit occupies 6 columns (6x8). Columns are vertical bytes, index 0..5.
const uint8_t DIGITS[10][6] = {
    // 0
    { 0x7E, 0x81, 0x81, 0x81, 0x81, 0x7E },
    // 1
    { 0x00, 0x82, 0x82, 0xFF, 0x80, 0x80 },
    // 2
    { 0xC2, 0xA1, 0x91, 0x89, 0x86, 0x00 },
    // 3
    { 0x42, 0x81, 0x89, 0x89, 0x76, 0x00 },
    // 4
    { 0x1F, 0x10, 0x10, 0xFF, 0x10, 0x10 },
    // 5
    { 0x8F, 0x89, 0x89, 0x89, 0x71, 0x00 },
    // 6
    { 0x7E, 0x89, 0x89, 0x89, 0x72, 0x00 },
    // 7
    { 0x01, 0xE1, 0x11, 0x09, 0x07, 0x00 },
    // 8
    { 0x76, 0x89, 0x89, 0x89, 0x76, 0x00 },
    // 9
    { 0x4E, 0x91, 0x91, 0x91, 0x7E, 0x00 }
};

// Buffer to store the final display pattern as 4 rows of 32 bits
// Each uint32_t represents one row of the display, bits go from left to right
uint32_t displayRows[4] = {0};

// Convert vertical columns to horizontal rows for two digits
void displayDigits(int leftDigit, int rightDigit) {
    // Clear display buffer
    for (int i = 0; i < 4; i++) {
            displayRows[i] = 0;
    }
    
    // Process left digit (first 6 columns)
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 6; col++) {
            if (DIGITS[leftDigit][col] & (1 << row)) {
                // Set bit in the appropriate row, shifting from left side
                displayRows[row/2] |= 1UL << (31 - col - (row%2)*16);
            }
        }
    }
    
    // Process right digit (last 6 columns, after 1 column gap)
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 6; col++) {
            if (DIGITS[rightDigit][col] & (1 << row)) {
                // Set bit in the appropriate row, shifting from left side
                displayRows[row/2] |= 1UL << (31 - (col + 7) - (row%2)*16);
            }
        }
    }
    
    // Write to the matrix
    matrixWrite(displayRows);

}

void displayNumber(int number) {
    // Clear columns
    for (int i = 0; i < 13; ++i) displayCols[i] = 0;

        int tens = number / 10;
        int ones = number % 10;
        displayDigits(tens, ones);

    // Pack the 13 bytes into 4 uint32_t words (little-endian byte order)
    uint32_t packed[4] = {0, 0, 0, 0};
    for (int i = 0; i < 13; ++i) {
        int wi = i / 4;
        int bi = i % 4;
        packed[wi] |= ((uint32_t)displayCols[i]) << (bi * 8);
    }

    // Write to display
    matrixWrite(packed);
}



void setup() {
    Serial.begin(9600);  // Initialize Serial communication
    matrixBegin();
    Bridge.begin();
    Serial.println("Temperature logging started");
}

void loop() {
    int temperature;
    bool ok = Bridge.call("get_temperature").result(temperature);
    
    if (ok) {
        if(temperature < 0) temperature = 0;
        displayNumber(temperature);
        
        // Log the temperature reading
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
    } else {
        Serial.println("Failed to read temperature");
    }
    delay(1000);
}
