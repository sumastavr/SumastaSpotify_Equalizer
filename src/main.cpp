#include <Arduino.h>


// all these libraries are required for the Teensy Audio Library
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//#include <SmartLEDShieldV4.h>  // comment out this line for if you're not using SmartLED Shield V4 hardware (this line needs to be before #include <SmartMatrix3.h>)
#include <MatrixHardware_Teensy3_ShieldV1toV3.h>    // SmartMatrix Shield for Teensy 3 V1-V3
#include <SmartMatrix.h>
#include <FastLED.h>

#define totalBars 128

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = totalBars;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN; // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels, or use SMARTMATRIX_HUB75_64ROW_MOD32SCAN for common 64x64 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

#define ADC_INPUT_PIN   15
#define ON_BLE          11
#define UP_VOL          12

AudioInputAnalog         input(ADC_INPUT_PIN);
AudioAnalyzeFFT1024       fft;
AudioConnection          audioConnection(input, 0, fft, 0);

// The scale sets how much sound is needed in each frequency range to
// show all 32 bars.  Higher numbers are more sensitive.
float scale = 256.0;

// An array to hold the 16 frequency bands
float level[totalBars];

// This array holds the on-screen levels.  When the signal drops quickly,
// these are used to lower the on-screen level 1 bar per update, which
// looks more pleasing to corresponds to human sound perception.
int shown[totalBars];
int capDot[totalBars];
int counterCapDot[totalBars];
const int defaultScrollOffset = 6;

const SM_RGB black = CRGB(0, 0, 0);

byte status = 0;

uint8_t getVerticalRedHue(int val);
uint8_t getVerticalGreenHue(int val);

void setup()
{
    Serial.begin(9600);
    Serial.println("Begin");

        // Initialize Matrix
    matrix.addLayer(&backgroundLayer); 
    matrix.addLayer(&scrollingLayer); 
    matrix.begin();
    
    matrix.setBrightness(50);
    
    scrollingLayer.setColor({0xff, 0xff, 0xff});
    scrollingLayer.setMode(wrapForward);
    scrollingLayer.setSpeed(40);
    scrollingLayer.setFont(font6x10);
    scrollingLayer.start("Sumasta Station", 1);

    for(int i=0;i<totalBars;i++){
      capDot[i]=32;
    }

    // Audio requires memory to work.
    AudioMemory(24);
}

void loop(){
  
    if (fft.available()) {

        level[0] = fft.read(2);
        level[1] = fft.read(3);
        level[2] = fft.read(4);
        level[3] = fft.read(5);
        level[4] = fft.read(6);
        level[5] = fft.read(7);
        level[6] = fft.read(8);
        level[7] = fft.read(9);
        level[8] = fft.read(10);
        level[9] = fft.read(11);
        level[10] = fft.read(12);
        level[11] = fft.read(13);
        level[12] = fft.read(14);
        level[13] = fft.read(15);
        level[14] = fft.read(16);
        level[15] = fft.read(17);

        level[16] = fft.read(18);
        level[17] = fft.read(19);
        level[18] = fft.read(20);
        level[19] = fft.read(21);
        level[20] = fft.read(22);
        level[21] = fft.read(23);
        level[22] = fft.read(24);
        level[23] = fft.read(25);
        level[24] = fft.read(26);
        level[25] = fft.read(27);
        level[26] = fft.read(28);
        level[27] = fft.read(29);
        level[28] = fft.read(30);
        level[29] = fft.read(31);
        level[30] = fft.read(32);
        level[31] = fft.read(33);

        level[32] = fft.read(34);
        level[33] = fft.read(35);
        level[34] = fft.read(36);
        level[35] = fft.read(37);
        level[36] = fft.read(38);
        level[37] = fft.read(39);
        level[38] = fft.read(40);
        level[39] = fft.read(41);
        level[40] = fft.read(42);
        level[41] = fft.read(43);
        level[42] = fft.read(44);
        level[43] = fft.read(45);
        level[44] = fft.read(46);
        level[45] = fft.read(47);
        level[46] = fft.read(48);
        level[47] = fft.read(49);

        level[48] = fft.read(50,51);
        level[49] = fft.read(52,53);
        level[50] = fft.read(54,55);
        level[51] = fft.read(56,57);
        level[52] = fft.read(58,59);
        level[53] = fft.read(60,61);
        level[54] = fft.read(62,63);
        level[55] = fft.read(64,65);
        level[56] = fft.read(66,67);
        level[57] = fft.read(68,69);
        level[58] = fft.read(70,71);
        level[59] = fft.read(72,73);
        level[60] = fft.read(74,75);
        level[61] = fft.read(76,77);
        level[62] = fft.read(78,79);
        level[63] = fft.read(80,81);

        level[64] = fft.read(82,83);
        level[65] = fft.read(84,85);
        level[66] = fft.read(86,87);
        level[67] = fft.read(88,89);
        level[68] = fft.read(90,91);
        level[69] = fft.read(92,93);
        level[70] = fft.read(94,95);
        level[71] = fft.read(96,97);
        level[72] = fft.read(98,99);
        level[73] = fft.read(100,101);
        level[74] = fft.read(102,103);
        level[75] = fft.read(104,105);
        level[76] = fft.read(106,107);
        level[77] = fft.read(108,109);
        level[78] = fft.read(110,111);
        level[79] = fft.read(112,113);

        level[80] = fft.read(114,115);
        level[81] = fft.read(116,117);
        level[82] = fft.read(118,119);
        level[83] = fft.read(120,121);
        level[84] = fft.read(122,123);
        level[85] = fft.read(124,125);
        level[86] = fft.read(126,127);
        level[87] = fft.read(128,130);
        level[88] = fft.read(131,132);
        level[89] = fft.read(133,135);
        level[90] = fft.read(136,138);
        level[91] = fft.read(139,141);
        level[92] = fft.read(142,144);
        level[93] = fft.read(145,146);
        level[94] = fft.read(147,149);
        
        level[95] = fft.read(150,152);
        level[96] = fft.read(153,155);
        level[97] = fft.read(156,158);
        level[98] = fft.read(159,162);
        level[99] = fft.read(163,166);
        level[100] = fft.read(167,170);
        level[101] = fft.read(171,173);
        
        level[102] = fft.read(174,177);
        level[103] = fft.read(178,181);
        level[104] = fft.read(182,189);
        level[105] = fft.read(190,197);
        level[106] = fft.read(198,205);
        level[107] = fft.read(206,213);
        level[108] = fft.read(214,221);
        level[109] = fft.read(222,229);
        level[110] = fft.read(230,237);
        level[111] = fft.read(238,245);
        level[112] = fft.read(246,253);
        level[113] = fft.read(254,261);
        level[114] = fft.read(262,269);
        level[115] = fft.read(270,277);
        level[116] = fft.read(278,287);
        level[117] = fft.read(288,297);
        level[118] = fft.read(298,307);

        level[119] = fft.read(308,317);
        level[120] = fft.read(318,327);
        level[121] = fft.read(328,337);
        level[122] = fft.read(338,347);
        level[123] = fft.read(348,357);
        level[124] = fft.read(358,367);
        level[125] = fft.read(368,379);
        level[126] = fft.read(380,391);
        level[127] = fft.read(392,403);

        /*
        level[121] = fft.read(404,415);
        level[122] = fft.read(416,427);
        level[123] = fft.read(428,439);
        level[124] = fft.read(440,453);
        level[125] = fft.read(454,467);
        level[126] = fft.read(468,483);
        level[127] = fft.read(484,511);
        */
        
        backgroundLayer.fillScreen(black);

        boolean overwriteCapDot=true;

        for (int i = 0; i < totalBars; i++) {
            
 
           // level[i]=fft.read(i+2);
            
            // TODO: conversion from FFT data to display bars should be
            // exponentially scaled.  But how to keep it a simple example?
            int val = level[i] * scale;

            // trim the bars vertically to fill the matrix height
            if (val >= kMatrixHeight) val = kMatrixHeight - 1;

            if (val > shown[i]) {
                shown[i] = val;

                if(val>=capDot[i]){
                  capDot[i]=val;
                }
                
            } else {
                if (shown[i] > 0) shown[i] = shown[i] - 2;
                val = shown[i];
                counterCapDot[i]++;
                if(counterCapDot[i]>2){
                  capDot[i]--;
                  if(capDot[i]<1)capDot[i]=1;
                  counterCapDot[i]=0;
                }
                overwriteCapDot=false;
            }

            
            if (shown[i] >= 0) {
                
                for (int j = 0; j < kMatrixWidth/16; j++) {
                        
                    //backgroundLayer.drawLine(i,32,i, (kMatrixHeight - 1) - val, color);

                    for(int h=0;h<val+1;h++){
                      uint8_t greenHue=getVerticalGreenHue(h);
                      uint8_t redHue=getVerticalRedHue(h);
                      
                      backgroundLayer.drawPixel(i, (kMatrixHeight - 1) - h, CRGB(redHue,greenHue,0)); 
                    }
                    
                }
            }

            backgroundLayer.drawPixel(i, (kMatrixHeight - 1) - capDot[i], CRGB(20,150,255)); 
                  
        }

        backgroundLayer.swapBuffers();
    }
}

uint8_t getVerticalRedHue(int val){

  switch(val){

    case 0: return 24; break;
    case 1: return 32; break;
    case 2: return 40; break;
    case 3: return 48; break;

    case 4: return 56; break;
    case 5: return 64; break;
    case 6: return 72; break;
    case 7: return 80; break;

    case 8: return 88; break;
    case 9: return 96; break;
    case 10: return 104; break;
    case 11: return 112; break;

    case 12: return 120; break;
    case 13: return 128; break;
    case 14: return 136; break;
    case 15: return 144; break;

    case 16: return 152; break;
    case 17: return 160; break;
    case 18: return 168; break;
    case 19: return 176; break;

    case 20: return 184; break;
    case 21: return 192; break;
    case 22: return 200; break;
    case 23: return 208; break;

    case 24: return 216; break;
    case 25: return 224; break;
    case 26: return 232; break;
    case 27: return 240; break;

    case 28: return 248; break;
    case 29: return 255; break;
    case 30: return 255; break;
    case 31: return 255; break;
  }

}

uint8_t getVerticalGreenHue(int val){

  switch(val){

    case 31: return 0; break;
    case 30: return 3; break;
    case 29: return 5; break;
    case 28: return 7; break;

    case 27: return 6; break;
    case 26: return 8; break;
    case 25: return 16; break;
    case 24: return 24; break;

    case 23: return 32; break;
    case 22: return 40; break;
    case 21: return 48; break;
    case 20: return 56; break;

    case 19: return 64; break;
    case 18: return 72; break;
    case 17: return 80; break;
    case 16: return 88; break;

    case 15: return 96; break;
    case 14: return 104; break;
    case 13: return 112; break;
    case 12: return 120; break;

    case 11: return 128; break;
    case 10: return 136; break;
    case 9: return 144; break;
    case 8: return 152; break;

    case 7: return 160; break;
    case 6: return 168; break;
    case 5: return 176; break;
    case 4: return 184; break;

    case 3: return 192; break;
    case 2: return 200; break;
    case 1: return 208; break;
    case 0: return 216; break;
  }
}