#ifndef EFFECTS_H
#define EFFECTS_H


int modRing(int i);
double randto(double range);
void DimRangeGradual(int from, int to, int rDim, int gDim, int bDim, int wDim);

void AddRangeGradual(int from, int to, int rDim, int gDim, int bDim, int wDim);

void SetRange(int from, int to, int r, int g, int b, int w);

void DimRange(int from, int to, int r, int g, int b, int w);

void AddRange(int from, int to, int r, int g, int b, int w);
void CountLeds();

#endif
