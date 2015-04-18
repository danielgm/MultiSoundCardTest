#include "ofApp.h"


void ofApp::setup() {
	ofSoundStream soundStream;
	soundStream.listDevices();

  size_t sampleRate = 44100;
  size_t ticksPerBuffer = 32;

	_numOutputChannels = 2;
	_numInputChannels = 1;
	_bufferSize = ticksPerBuffer * 64;

	_numSoundStreams = 2;
	_soundStreams = new mmSoundStream[_numSoundStreams];
	for (int i = 0; i < _numSoundStreams; ++i) {
		_soundStreams[i].setup(this, i, 2 + i, _numOutputChannels, _numInputChannels, _bufferSize, &_audioProcessingMutex);
	}

  if (!_pd.init(
      _numSoundStreams * _numOutputChannels,
      _numSoundStreams * _numInputChannels,
      sampleRate,
      ticksPerBuffer)) {
    OF_EXIT_APP(1);
  }

  _pd.openPatch("pdtest.pd");
  _pd.start();

  {
    ofMutex::ScopedLock lock(_audioProcessingMutex);

    _pdInputBuffer = new float[_numSoundStreams * _numInputChannels * _bufferSize];
    for (int i = 0; i < _numInputChannels * _bufferSize; ++i) {
      _pdInputBuffer[i] = 0;
    }

    _pdOutputBuffer = new float[_numSoundStreams * _numOutputChannels * _bufferSize];
    for (int i = 0; i < _numOutputChannels * _bufferSize; ++i) {
      _pdOutputBuffer[i] = 0;
    }
  }
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);

	for (int i = 0; i < _numSoundStreams; ++i) {
		drawSoundStream(_soundStreams[i].getInputBufferRef(), _bufferSize, _numInputChannels, 0, i * 150, 512, 150);
    drawSoundStream(_soundStreams[i].getOutputBufferRef(), _bufferSize, _numOutputChannels, 512, 25 + i * 150, 512, 150);
  }
}

void ofApp::drawSoundStream(float* buffer, int bufferSize, int numChannels, int x, int y, int width, int height) {
  ofMutex::ScopedLock lock(_audioProcessingMutex);

  int len = numChannels * bufferSize;
  float w = (float)width / len;

  ofSetLineWidth(0);
	ofNoFill();
	ofSetColor(128);
	ofLine(x + 2, y + height/2, x + width - 2, y + height/2);
	ofRect(x + 1, y + 1, width - 2, height - 2);

  ofFill();
	ofSetColor(255);
	for (int i = 0; i < len; ++i) {
		ofRect(x + i * w, y + height/2, w, -buffer[i] * height/2);
	}
}

void ofApp::exit() {
  delete[] _soundStreams;
}

void ofApp::audioIn(int streamId, float* input, int bufferSize, int numChannels) {
  ofMutex::ScopedLock lock(_audioProcessingMutex);

  for (int c = 0; c < numChannels; ++c) {
    for (int i = 0; i < bufferSize; ++i) {
      _pdInputBuffer[(streamId * _numInputChannels + c) * bufferSize + i] =
        input[c * bufferSize + i];
    }
  }

  _pd.audioIn(_pdInputBuffer, bufferSize, _numSoundStreams * _numInputChannels);
}

void ofApp::audioOut(int streamId, float* output, int bufferSize, int numChannels) {
  ofMutex::ScopedLock lock(_audioProcessingMutex);

  if (streamId == 0) {
    _pd.audioOut(_pdOutputBuffer, bufferSize, _numSoundStreams * _numOutputChannels);
  }

  for (int c = 0; c < numChannels; ++c) {
    for (int i = 0; i < bufferSize; ++i) {
      output[i * numChannels + c] =
        _pdOutputBuffer[i * _numSoundStreams * _numOutputChannels + streamId * _numOutputChannels + c];
    }
  }
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y ) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {

}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::windowResized(int w, int h) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}
