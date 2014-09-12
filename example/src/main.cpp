#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofPtr<ofBaseRenderer> renderer(new ofGLRenderer(false));
	ofSetCurrentRenderer(renderer, true);
	ofSetupOpenGL(800,600,OF_WINDOW);
	ofSetVerticalSync(false);
	ofRunApp(new ofApp());
	return 0;
}
