#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){

	vidGrabber.setVerbose(true);
    vidGrabber.setup(320,240);
    colorImg.allocate(320,240);

	threshold = 80;

	finder.setup("haarcascade_frontalface_default.xml");

	//set amount of frames before forgetting something.
	finder.getTracker().setPersistence(15);
	// an object can move up to 32 pixels per frame
	finder.getTracker().setMaximumDistance(32);
}

//--------------------------------------------------------------
void ofApp::update(){
	//Draw background
	ofBackground(100,100,100);

	// check for new img in cam
    vidGrabber.update();
	// if new image check for faces.
	if (vidGrabber.isFrameNew()){

        colorImg.setFromPixels(vidGrabber.getPixels());

		finder.update(colorImg);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	// draw the webcam img
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);

	// draw the whole finder
	ofPushMatrix();
		//ofset drawing position
		ofTranslate(20, 20);
		// No parameters in draw function
		ofSetColor(255);
		//finder.draw();

		// or, instead we can draw each blob individually from the blobs vector,
		for (int i = 0; i < finder.size(); i++){
			ofRectangle tempRect = finder.getObject(i);
			ofDrawRectangle(tempRect);

			int label = finder.getLabel(i);
			int age = finder.getTracker().getAge(label);
			ofPoint center = tempRect.getCenter();
			ofSetColor(255, 0, 0);
			ofDrawBitmapString(ofToString(label), center.x, center.y);
		}

	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
