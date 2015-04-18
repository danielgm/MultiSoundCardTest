#include "ofApp.h"


void ofApp::setup() {
	ofSoundStream soundStream;
	soundStream.listDevices();

	numOutputChannels = 2;
	numInputChannels = 1;
	bufferSize = 512;

	numSoundStreams = 3;
	soundStreams = new mmSoundStream[numSoundStreams];
	for (int i = 0; i < numSoundStreams; ++i) {
		soundStreams[i].setup(2 + i, numOutputChannels, numInputChannels, bufferSize, &_audioProcessingMutex);
	}
	soundStreams[1].setOutputStream(&soundStreams[0]);
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);

	for (int i = 0; i < numSoundStreams; ++i) {
		drawSoundStream(soundStreams[i].getInputBufferRef(), numInputChannels, bufferSize, 0, i * 150, 512, 150);
		drawSoundStream(soundStreams[i].getOutputBufferRef(), numOutputChannels, bufferSize, 512, i * 150, 512, 150);
		}
}

void ofApp::drawSoundStream(float* buffer, int numChannels, int bufferSize, int x, int y, int width, int height) {
	ofNoFill();
	ofSetColor(128, 0, 0);
	ofLine(x, y + height/2, x + width, y + height/2);
	ofSetColor(128);
	ofRect(x + 1, y + 1, width - 2, height - 2);
	ofSetColor(255, 255, 255);
	for (int i = 0; i < numChannels * bufferSize; ++i) {
		ofLine(x + i, y + height/2, x + i, y + height/2 - buffer[i] * height/2);
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
