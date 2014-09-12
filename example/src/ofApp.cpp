#include "ofApp.h"
#include "TestCases.h"
#include "ofxTextureCache.h"
#include "ofxShaderCache.h"
#include "ofxShaderProgramCache.h"
#include "ofxBitmapFontCache.h"
//--------------------------------------------------------------
extern Test* current_test;
//--------------------------------------------------------------
void ofApp::setup() {
	new ofxSpriteRenderer;
	new ofxShaderCache;
	new ofxShaderProgramCache;
	new ofxTextureCache;
	new ofxBitmapFontCache;
	ofxRENDERER->SetRenderSize(ofGetWindowWidth(), ofGetWindowHeight());
	current_test->Setup();
	//ofSetFrameRate(60);
	cam = ofxRENDERER->GetCamera();
	//cam->disableMouseMiddleButton();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update() {
	current_test->Update();
	mesh->UpdateCrowd(0.0030f);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(ofColor(0.0f,128.0f,255.0f,255.0f));
	
	cam->begin();
	ofEnableDepthTest();
	render->Render();
	ofDisableDepthTest();
	current_test->Render();
	cam->end();
#ifdef _DEBUG
	ofSetWindowTitle("FPS: "+ofToString(ofGetFrameRate(), 2)+
		" - Sprite: "+ofToString(ofxRENDERER->GetSpriteNumber())+
		" - Draw call: "+ofToString(ofxRENDERER->GetDrawCall())+
		" - Vertices: "+ofToString(ofxRENDERER->GetDrawVertices()));
#endif
}

//--------------------------------------------------------------
#define ZOOM_SPEED 15.0f
#define MOVE_SPEED 15.0f
void ofApp::keyPressed(int key){
	if(key == OF_KEY_PAGE_UP) 
	{
		cam->SetScale(cam->GetScale() - ZOOM_SPEED);
	}
	if(key == OF_KEY_PAGE_DOWN) 
	{
		cam->SetScale(cam->GetScale() + ZOOM_SPEED);
	}
	if(key == OF_KEY_UP) 
	{
		ofxRENDERER->MoveCamera(0, -MOVE_SPEED);
	}
	if(key == OF_KEY_DOWN) 
	{
		ofxRENDERER->MoveCamera(0, MOVE_SPEED);
	}
	if(key == OF_KEY_LEFT) 
	{
		ofxRENDERER->MoveCamera(-MOVE_SPEED, 0);
	}
	if(key == OF_KEY_RIGHT) 
	{
		ofxRENDERER->MoveCamera(MOVE_SPEED, 0);
	}
	if(key == OF_KEY_F1)
	{
		ofxRENDERER->FocusCamera(ofVec2f(0.0f,0.0f));
	}
	if(key == OF_KEY_F2)
	{
		ofxRENDERER->Make2DCamera();
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	ofxRENDERER->SetRenderSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
