#include "mmSoundStream.h"

mmSoundStream::mmSoundStream() {
	_inputBuffer = NULL;
	_outputBuffer = NULL;
}

mmSoundStream::~mmSoundStream() {
	{
		ofMutex::ScopedLock lock(*_audioProcessingMutex);
		_soundStream.close();

		delete[] _inputBuffer;
	}
}

void mmSoundStream::setup(
		int deviceId,
		int numOutputChannels,
		int numInputChannels,
		int bufferSize,
		ofMutex* audioProcessingMutex) {
	_deviceId = deviceId;
	_audioProcessingMutex = audioProcessingMutex;

	_numOutputChannels = numOutputChannels;
	_numInputChannels = numInputChannels;
	_bufferSize = bufferSize;

	_inputBuffer = new float[_numInputChannels * _bufferSize];
	for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
		_inputBuffer[i] = 0;
	}

	_soundStream.setDeviceID(deviceId);
	_soundStream.setup(_numOutputChannels, _numInputChannels, 44100, _bufferSize, 6);
	_soundStream.setInput(this);
	_soundStream.setOutput(this);
}

float* mmSoundStream::getInputBufferRef() {
	return _inputBuffer;
}

void mmSoundStream::setOutputBufferRef(float* outputBuffer) {
	_outputBuffer = outputBuffer;
}

void mmSoundStream::audioReceived(float* input, int bufferSize, int numChannels) {
	ofMutex::ScopedLock lock(*_audioProcessingMutex);

	for (size_t i = 0; i < bufferSize; ++i) {
		for (size_t c = 0; c < _numInputChannels && c < numChannels; ++c) {
			_inputBuffer[i * _numInputChannels + c] = input[i * numChannels + c];
		}
	}
}

void mmSoundStream::audioRequested(float* output, int bufferSize, int numChannels) {
	ofMutex::ScopedLock lock(*_audioProcessingMutex);

	// FIXME: Okay, output is actually 2 channels and input is only 1 channel so bad sound.
	// Can change numOutputChannels to 1 instead of 2 (in ofApp.cpp) to quickfix. Need to
	// handle differing numbers of channels.
	if (_outputBuffer != NULL) {
		for (size_t i = 0; i < bufferSize; ++i) {
			for (size_t c = 0; c < _numOutputChannels && c < numChannels; ++c) {
				output[i * numChannels + c] = _outputBuffer[i * _numOutputChannels + c];
			}
		}
	}
}
