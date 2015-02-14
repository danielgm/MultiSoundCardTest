#include "mmSoundStream.h"

mmSoundStream::mmSoundStream() {
}

mmSoundStream::~mmSoundStream() {
	{
		ofMutex::ScopedLock lock(_audioProcessingMutex);
		_soundStream.close();
		
		delete[] _inputBuffer;
		delete[] _outputBuffer;
	}
}

void mmSoundStream::setup(int deviceId, int numOutputChannels, int numInputChannels, int bufferSize) {
	_numOutputChannels = numOutputChannels;
	_numInputChannels = numInputChannels;
	_bufferSize = bufferSize;
	
	_inputBuffer = new float[_numInputChannels * _bufferSize];
	for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
		_inputBuffer[i] = 0;
	}

	_outputBuffer = new float[_numOutputChannels * _bufferSize];
	for (int i = 0; i < _numOutputChannels * _bufferSize; ++i) {
		_outputBuffer[i] = 0;
	}

	_soundStream.setDeviceID(deviceId);
	_soundStream.setup(_numOutputChannels, _numInputChannels, 44100, _bufferSize, 6);
	_soundStream.setInput(this);
	_soundStream.setOutput(this);
}

float* mmSoundStream::getInputBufferRef() {
	return _inputBuffer;
}

float* mmSoundStream::getOutputBufferRef() {
	return _outputBuffer;
}

void mmSoundStream::audioReceived(float* input, int bufferSize, int numChannels) {
	ofMutex::ScopedLock lock(_audioProcessingMutex);
	
	for (size_t i = 0; i < bufferSize; ++i) {
		for (size_t c = 0; c < numChannels && c < _numInputChannels; ++c) {
			_inputBuffer[i * _numInputChannels + c] = input[i * numChannels + c];
		}
	}
}

void mmSoundStream::audioRequested(float* output, int bufferSize, int numChannels) {
	ofMutex::ScopedLock lock(_audioProcessingMutex);

	for (size_t i = 0; i < bufferSize; ++i) {
		for (size_t c = 0; c < _numInputChannels && c < numChannels; ++c) {
			output[i * numChannels + c] = _outputBuffer[i * _numOutputChannels + c];
		}
	}
}
