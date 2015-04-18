#pragma once

#include "ofMain.h"
#include "ofxPd.h"
#include "mmSoundStream.h"
#include "mmStreamManager.h"

class ofApp : public ofBaseApp, mmStreamManager {

	public:
	void setup();
	void update();
	void draw();
	void exit();

  void audioIn(int streamId, float* input, int bufferSize, int numChannels);
  void audioOut(int streamId, float* output, int bufferSize, int numChannels);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void drawSoundStream(float* buffer, int bufferSize, int numChannels, int x, int y, int width, int height);

  ofxPd _pd;
  float* _pdInputBuffer;
  float* _pdOutputBuffer;

	ofMutex _audioProcessingMutex;

	int _numInputChannels;
	int _numOutputChannels;
	int _bufferSize;
	int _numSoundStreams;
	mmSoundStream* _soundStreams;
};
