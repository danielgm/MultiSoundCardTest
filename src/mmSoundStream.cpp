#include "mmSoundStream.h"

mmSoundStream::mmSoundStream() {
	_inputBuffer = NULL;
	_outputBuffer = NULL;
}

mmSoundStream::~mmSoundStream() {
  ofMutex::ScopedLock lock(*_audioProcessingMutex);

  _soundStream.close();

  delete[] _inputBuffer;
  delete[] _outputBuffer;

  cout << "mmSoundStream(" << _streamId << ") died." << endl;
}

void mmSoundStream::setup(
    mmStreamManager* streamManager,
    int streamId,
		int deviceId,
		int numOutputChannels,
		int numInputChannels,
		int bufferSize,
		ofMutex* audioProcessingMutex) {
  size_t sampleRate = 44100;

  _streamManager = streamManager;
  _streamId = streamId;
	_deviceId = deviceId;
	_audioProcessingMutex = audioProcessingMutex;

	_numOutputChannels = numOutputChannels;
	_numInputChannels = numInputChannels;
	_bufferSize = bufferSize;

  {
    ofMutex::ScopedLock lock(*_audioProcessingMutex);

    _inputBuffer = new float[_numInputChannels * _bufferSize];
    for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
      _inputBuffer[i] = 0;
    }

    _outputBuffer = new float[_numInputChannels * _bufferSize];
    for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
      _outputBuffer[i] = 0;
    }
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

  cout << "mmSoundStream(" << _streamId << ") created." << endl;
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

void mmSoundStream::audioReceived(float* input, int bufferSize, int numChannels) {
  _streamManager->audioIn(_streamId, input, bufferSize, numChannels);

  {
    ofMutex::ScopedLock lock(*_audioProcessingMutex);

    for (size_t c = 0; c < _numInputChannels && c < numChannels; ++c) {
      for (size_t i = 0; i < bufferSize; ++i) {
        _inputBuffer[c * bufferSize + i] = input[c * bufferSize + i];
      }
    }
  }
}

void mmSoundStream::audioRequested(float* output, int bufferSize, int numChannels) {
  _streamManager->audioOut(_streamId, output, bufferSize, numChannels);

  {
    ofMutex::ScopedLock lock(*_audioProcessingMutex);

    for (size_t c = 0; c < _numOutputChannels; ++c) {
      for (size_t i = 0; i < bufferSize; ++i) {
          _outputBuffer[c * bufferSize + i] = output[i * numChannels + (c % numChannels)];
      }
    }
  }
}
