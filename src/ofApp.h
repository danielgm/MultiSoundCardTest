#pragma once

#include "ofMain.h"

#include "ofxPd.h"
#include "mmSoundStream.h"

class ofApp : public ofBaseApp{

	public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	void drawSoundStream(mmSoundStream* ss, int x, int y, int width, int height);
	
	int numInputChannels;
	int numOutputChannels;
	int bufferSize;
	int numSoundStreams;
	mmSoundStream* soundStreams;
};
