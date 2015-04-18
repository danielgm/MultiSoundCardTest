#pragma once

#include "ofMain.h"
#include "mmStreamManager.h"

class mmSoundStream : public ofBaseSoundInput, public ofBaseSoundOutput {

public:
	mmSoundStream();
	~mmSoundStream();
	void setup(
      mmStreamManager* streamManager,
      int streamId,
			int deviceId,
			int numOutputChannels,
			int numInputChannels,
			int bufferSize,
			ofMutex* audioProcessingMutex);

	size_t getNumInputChannels();
	size_t getNumOutputChannels();

	size_t getBufferSize();

	float* getInputBufferRef();
	float* getOutputBufferRef();

	void audioReceived(float* input, int bufferSize, int nChannels);
	void audioRequested(float* output, int bufferSize, int nChannels);

private:
  mmStreamManager* _streamManager;
  int _streamId;
	int _deviceId;

	ofMutex* _audioProcessingMutex;
	ofSoundStream _soundStream;

	size_t _numOutputChannels;
	size_t _numInputChannels;

	size_t _bufferSize;

	float* _inputBuffer;
	float* _outputBuffer;
};
