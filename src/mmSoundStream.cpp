#include "mmSoundStream.h"

mmSoundStream::mmSoundStream() {
	_inputBuffer = NULL;
	_outputBuffer = NULL;
	_outputStream = NULL;
}

mmSoundStream::~mmSoundStream() {
	{
		ofMutex::ScopedLock lock(*_audioProcessingMutex);
		_soundStream.close();

		delete[] _inputBuffer;
		delete[] _outputBuffer;
	}
}

void mmSoundStream::setup(
		int deviceId,
		int numOutputChannels,
		int numInputChannels,
		int bufferSize,
		ofMutex* audioProcessingMutex) {
	size_t sampleRate = 44100;
	size_t ticksPerBuffer = 8;

	_deviceId = deviceId;
	_audioProcessingMutex = audioProcessingMutex;

	_numOutputChannels = numOutputChannels;
	_numInputChannels = numInputChannels;
	_bufferSize = bufferSize;

	_inputBuffer = new float[_numInputChannels * _bufferSize];
	for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
		_inputBuffer[i] = 0;
	}

	_outputBuffer = new float[_numInputChannels * _bufferSize];
	for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
		_outputBuffer[i] = 0;
	}

	_soundStream.setDeviceID(deviceId);
	_soundStream.setup(
			_numOutputChannels,
			_numInputChannels,
			sampleRate,
			_bufferSize,
			2 * (_numOutputChannels + _numInputChannels));
	_soundStream.setInput(this);
	_soundStream.setOutput(this);
}

size_t mmSoundStream::getNumOutputChannels() {
	return _numOutputChannels;
}

size_t mmSoundStream::getNumInputChannels() {
	return _numInputChannels;
}

size_t mmSoundStream::getBufferSize() {
	return _bufferSize;
}

float* mmSoundStream::getInputBufferRef() {
	return _inputBuffer;
}

float* mmSoundStream::getOutputBufferRef() {
	return _outputBuffer;
}

void mmSoundStream::setOutputStream(mmSoundStream* outputStream) {
	_outputStream = outputStream;
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

	if (_outputStream != NULL) {
		// TODO: Handle different buffer sizes.
		float* inputBuffer = _outputStream->getInputBufferRef();
		size_t numInputChannels = _outputStream->getNumInputChannels();
		for (size_t i = 0; i < bufferSize; ++i) {
			for (size_t c = 0; c < numChannels || c < _numOutputChannels; ++c) {
				if (c < numChannels) {
					output[i * numChannels + c] = inputBuffer[i * numInputChannels + (c % numInputChannels)];
				}
				if (c < _numOutputChannels) {
					_outputBuffer[i * _numOutputChannels + c] = inputBuffer[i * numInputChannels + (c % numInputChannels)];
				}
			}
		}
	}
}
