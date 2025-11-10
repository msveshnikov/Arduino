#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>
#include <Modulino.h>

ModulinoBuzzer buzzer;
Arduino_LED_Matrix matrix;

// Star Wars Imperial March notes
const int NOTE_G4 = 392;
const int NOTE_G5 = 784;
const int NOTE_DS5 = 622;
const int NOTE_F5 = 698;
const int NOTE_D5 = 587;
const int NOTE_C5 = 523;
const int NOTE_AS4 = 466;
const int NOTE_GS4 = 415;
const int NOTE_FS5 = 740;
const int NOTE_AS5 = 932;

// Enhanced Death Star rotation frames (16 frames for smoother rotation)
// Frame 0 - Trench at top
uint8_t deathStar0[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 3, 2, 2, 2, 3, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 1
uint8_t deathStar1[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 3, 2, 2, 2, 2, 2, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 2
uint8_t deathStar2[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 3, 2, 2, 2, 2, 2, 3, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 3
uint8_t deathStar3[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 3, 2, 2, 2, 2, 2, 3, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 4 - Trench at bottom
uint8_t deathStar4[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 3, 2, 3, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 5
uint8_t deathStar5[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 3, 2, 2, 2, 2, 3, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 6
uint8_t deathStar6[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 3, 2, 2, 2, 2, 3, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 7
uint8_t deathStar7[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 3, 2, 2, 2, 2, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 8 - Trench coming back to top
uint8_t deathStar8[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 3, 2, 2, 2, 3, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 9
uint8_t deathStar9[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 3, 2, 3, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 10
uint8_t deathStar10[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 2, 2, 2, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 11 - Left side
uint8_t deathStar11[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 2, 2, 2, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 12
uint8_t deathStar12[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 2, 2, 2, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 13
uint8_t deathStar13[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 2, 2, 2, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 14 - Right side
uint8_t deathStar14[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 7, 2, 2, 2, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

// Frame 15
uint8_t deathStar15[104] = {
    0, 0, 0, 1, 3, 5, 6, 5, 3, 1, 0, 0, 0,
    0, 0, 2, 5, 7, 7, 7, 7, 7, 5, 2, 0, 0,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    1, 4, 7, 7, 7, 7, 7, 2, 2, 2, 7, 4, 1,
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 2,
    1, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 1,
    0, 2, 5, 7, 7, 7, 7, 7, 7, 7, 5, 2, 0,
    0, 0, 1, 4, 6, 7, 7, 7, 6, 4, 1, 0, 0};

uint8_t *deathStarFrames[] = {
    deathStar0, deathStar1, deathStar2, deathStar3,
    deathStar4, deathStar5, deathStar6, deathStar7,
    deathStar8, deathStar9, deathStar10, deathStar11,
    deathStar12, deathStar13, deathStar14, deathStar15};

int currentFrame = 0;
unsigned long lastFrameChange = 0;
const unsigned long FRAME_INTERVAL = 150; // Smoother animation

void setup()
{
    matrix.begin();
    matrix.setGrayscaleBits(3); // Enable 8 shades of gray
    Modulino.begin(Wire1);
    buzzer.begin();
    Monitor.begin(9600);
}

void playImperialMarch()
{
    // Measure 1-2
    buzzer.tone(NOTE_G4, 500);
    updateDeathStar(500);
    buzzer.tone(NOTE_G4, 500);
    updateDeathStar(500);
    buzzer.tone(NOTE_G4, 500);
    updateDeathStar(500);

    // Measure 3
    buzzer.tone(NOTE_DS5, 350);
    updateDeathStar(350);
    buzzer.tone(NOTE_AS4, 150);
    updateDeathStar(150);

    // Measure 4
    buzzer.tone(NOTE_G4, 500);
    updateDeathStar(500);
    buzzer.tone(NOTE_DS5, 350);
    updateDeathStar(350);
    buzzer.tone(NOTE_AS4, 150);
    updateDeathStar(150);

    // Measure 5
    buzzer.tone(NOTE_G4, 1000);
    updateDeathStar(1000);

    // Measure 6-7
    buzzer.tone(NOTE_D5, 500);
    updateDeathStar(500);
    buzzer.tone(NOTE_D5, 500);
    updateDeathStar(500);
    buzzer.tone(NOTE_D5, 500);
    updateDeathStar(500);

    // Measure 8
    buzzer.tone(NOTE_DS5, 350);
    updateDeathStar(350);
    buzzer.tone(NOTE_AS4, 150);
    updateDeathStar(150);

    // Measure 9
    buzzer.tone(NOTE_GS4, 500);
    updateDeathStar(500);
    buzzer.tone(NOTE_DS5, 350);
    updateDeathStar(350);
    buzzer.tone(NOTE_AS4, 150);
    updateDeathStar(150);

    // Measure 10
    buzzer.tone(NOTE_G4, 1000);
    updateDeathStar(1000);
}

void updateDeathStar(int duration)
{
    unsigned long startTime = millis();
    while (millis() - startTime < duration)
    {
        if (millis() - lastFrameChange >= FRAME_INTERVAL)
        {
            matrix.draw(deathStarFrames[currentFrame]);
            currentFrame = (currentFrame + 1) % 16;
            lastFrameChange = millis();
        }
    }
}

void loop()
{
    Monitor.println("Test");
    playImperialMarch();
    delay(1000); // Pause before repeating
}