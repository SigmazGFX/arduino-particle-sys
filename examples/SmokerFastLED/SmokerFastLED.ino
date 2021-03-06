/* 
 * Copyright (C) 2013 Gilad Dayagi.  All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

 /*
  * An example for the Arduino particle system library
  * Creates a colored smoke effect
  */

#include <FastLED.h>

#define LED_WIDTH 8
#define LED_HEIGHT 8

// set ledSerpentineLayout to true if your matrix looks wrong ;) - see Example "XYMatrix" in FastLED library.
const bool ledSerpentineLayout = false;  
// translates from x, y into an index into the LED array
uint16_t XY( uint8_t x, uint8_t y) {
  if(y >= LED_HEIGHT) { y = LED_HEIGHT - 1; }
  if(x >= LED_WIDTH) { x = LED_WIDTH - 1; }
  uint16_t ret;
  if(ledSerpentineLayout == false) {
    ret = (y * LED_WIDTH) + x;
  } else {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t revX = (LED_WIDTH - 1) - x;
      ret = (y * LED_WIDTH) + revX;
    } else {
      // Even rows run forwards
      ret = (y * LED_WIDTH) + x;
    }
  }
  return ret;
}

// Particle System

#include <ParticleSys.h>

#define NUM_LEDS    (LED_WIDTH * LED_HEIGHT)
#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define BRIGHTNESS  32
CRGB leds[ NUM_LEDS ];

const byte numParticles = 25;

ParticleSysConfig g(LED_WIDTH, LED_HEIGHT);
Particle_Std particles[numParticles];
Particle_Attractor source(g.center_x, g.center_y);
Emitter_Fountain emitter(0, 0, 5, &source);
ParticleSys pSys(&g, numParticles, particles, &emitter);
FastLEDRenderer renderer(&g);

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS );

  randomSeed(analogRead(0));
  
  renderer.reset(leds);
  
  source.vx = 3;
  source.vy = 1;
  source.x = random(50)+100;
  source.y = random(10)+1;
  source.atf = 2;
  emitter.minLife = 100;
  emitter.maxLife = 200;
  pSys.perCycle = 2;
  
}

void loop() {
    pSys.update();
    renderer.reset(leds);
    renderer.render(&g, particles, numParticles, leds);
    FastLED.show();
    delay(30);
}

