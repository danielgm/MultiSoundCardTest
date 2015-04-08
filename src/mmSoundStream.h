#pragma once

#include "ofMain.h"

class mmSoundStream : public ofBaseSoundInput, public ofBaseSoundOutput {

public:
	mmSoundStream();
	~mmSoundStream();
	void setup(
			int deviceId,
			int numOutputChannels,
			int numInputChannels,
			int bufferSize,
			ofMutex* audioProcessingMutex);

	float* getInputBufferRef();
	void setOutputBufferRef(float* outputBuffer);

	void audioReceived(float* input, int bufferSize, int nChannels);
	void audioRequested(float* output, int bufferSize, int nChannels);

private:
	int _deviceId;

	ofMutex* _audioProcessingMutex;
	ofSoundStream _soundStream;

	size_t _numOutputChannels;
	size_t _numInputChannels;

	size_t _bufferSize;

	float* _outputBuffer;
	float* _inputBuffer;
};
