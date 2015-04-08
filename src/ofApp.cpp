#include "ofApp.h"


void ofApp::setup() {
	ofSoundStream soundStream;
	soundStream.listDevices();

	numOutputChannels = 2;
	numInputChannels = 1;
	bufferSize = 512;

	numSoundStreams = 2;
	soundStreams = new mmSoundStream[numSoundStreams];
	for (int i = 0; i < numSoundStreams; ++i) {
		soundStreams[i].setup(5 + i, numOutputChannels, numInputChannels, bufferSize, &_audioProcessingMutex);
	}
	soundStreams[1].setOutputBufferRef(soundStreams[0].getInputBufferRef());
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);

	for (int i = 0; i < numSoundStreams; ++i) {
		drawSoundStream(&soundStreams[i], 0, i * 150, 512, 150);
	}
}

void ofApp::drawSoundStream(mmSoundStream* ss, int x, int y, int width, int height) {
	float* inputBuffer = ss->getInputBufferRef();

	ofSetColor(255, 255, 255);
	for (int i = 0; i < numInputChannels * bufferSize; ++i) {
		ofLine(x + i, y + height/2, x + i, y + height/2 - inputBuffer[i] * height/2);
	}
}

void ofApp::exit() {
	delete[] soundStreams;
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
