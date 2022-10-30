#include <math.h>
#include <stdio.h>
#include <string.h>

float A, B, C;
float cubeWidth = 20;
int width = 100, height = 30;
float zBuffer[100 * 30];
char buffer[100 * 30];
int backgroundASCIICode = ' ';
int distanceFromCam = 100;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    float sinA = sin(A);
    float sinB = sin(B);
    float sinC = sin(C);
    float cosA = cos(A);
    float cosB = cos(B);
    float cosC = cos(C);
    return j * sinA * sinB * cosC - k * cosA * sinB * cosC + 
           j * cosA * sinC + k * sinA * sinC + i * cosB * cosC;
}

float calculateY(int i, int j, int k) {
    float sinA = sin(A);
    float sinB = sin(B);
    float sinC = sin(C);
    float cosA = cos(A);
    float cosB = cos(B);
    float cosC = cos(C);
    return j * cosA * cosC + k * sinA * cosC - 
           j * sinA * sinB * sinC + k * cosA * sinB * sinC -
           i * cosB * sinC;
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) -
           j * sin(A) * cos(B) +
           i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(width  / 2 - cubeWidth + K1 * ooz * x  * 2);
    yp = (int)(height / 2 + K1 * ooz * y);
    
    idx = xp + yp * width;
    if(idx >= 0 && idx < width * height) {
        if(ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    printf("\x1b[2J");
    while(1) {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '-');
            }
        }
        printf("\x1b[H");
        for(int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.005;
        B += 0.005;
    }
    return 0;
}