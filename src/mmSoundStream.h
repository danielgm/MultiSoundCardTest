#pragma once

#include "ofMain.h"

class mmSoundStream : public ofBaseSoundInput, public ofBaseSoundOutput {

public:
	mmSoundStream();
	~mmSoundStream();
	void setup(int deviceId, int numOutputChannels, int numInputChannels, int bufferSize);
	
	float* getInputBufferRef();
	float* getOutputBufferRef();

	void audioReceived(float* input, int bufferSize, int nChannels);
	void audioRequested(float* output, int bufferSize, int nChannels);
	
private:
	ofMutex _audioProcessingMutex;
	ofSoundStream _soundStream;
	
	size_t _numOutputChannels;
	size_t _numInputChannels;
	
	size_t _bufferSize;
	
	float* _outputBuffer;
	float* _inputBuffer;
};
