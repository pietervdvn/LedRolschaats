#ifndef LIGHTS_H
#define LIGHTS_H

void SetupLights();
void SetLed(int led, unsigned int r, unsigned int g, unsigned int b, unsigned int w);
void AddLed(int i, unsigned int r, unsigned int g, unsigned int b, unsigned int w);
void DimLed(int i, unsigned int r, unsigned int g, unsigned int b, unsigned int w);
void Commit();
#endif
