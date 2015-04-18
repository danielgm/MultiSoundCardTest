#pragma once

#include "ofMain.h"

class mmStreamManager {

public:
  virtual void audioIn(int streamId, float* input, int bufferSize, int numChannels) {}
  virtual void audioOut(int streamId, float* output, int bufferSize, int numChannels) {}
};
