#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"
#include "Ripple.h"
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	ofShader shader;
	ofFbo	fbo;
	ofVideoGrabber grabber;
	ofVboMesh mainMesh;
	Particle particle;
	ofEasyCam cam;
	
	
	ofxPanel gui;
    ofxFloatSlider maxHeight;
	ofxToggle enableShader;
	ofxToggle enableRipple;
	ofxToggle enableParticle;
		ofxToggle enableCamera;
	ofxIntSlider mode;
	ofxFloatSlider color[9];
//    ofxFloatSlider cameraSpeed;
//    ofxFloatSlider cameraRollSpeed;
//    ofxButton shouldSaveCameraPoint;
//    ofxToggle currentLockCamera;
//    ofxIntSlider mode;
//    ofxIntSlider vebose;
	Ripple ripple;
	
	
};
