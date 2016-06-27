#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){

	vidGrabber.setVerbose(true);
    vidGrabber.setup(320,240);
    //colorImg.allocate(320,240);

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
	ofBackground(100, 100, 100);

	// check for new img in cam
    vidGrabber.update();
	captured = false;
	
	cout << "map " << faces.size() << endl;

	// if new image check for faces.
	if (vidGrabber.isFrameNew()){

        colorImg.setFromPixels(vidGrabber.getPixels());

		finder.update(colorImg);
		RectTracker& results = finder.getTracker();

		for (size_t i = 0; i < finder.size(); i++)
		{
			//Get label belonging to current index
			int label = results.getLabelFromIndex(i);
			unsigned int age = results.getAge(label);

			//If detected face is older than xx frames save an mugshot
			if (age > 16) {
				//If there exist no face for this face create it.
				if (faces.find(label) == faces.end()) {
					face newFace;
					newFace.mugshots = 0;
					newFace.label = label;
					faces[label] = newFace;
				}
				//if there not yet 5 photos and the frames since last photo is more than 25
				else if(faces[label].mugshots < 5 && age - faces[label].lastPhoto > 8 * faces[label].mugshots) {
					//Get facedata
					ofRectangle curFace = finder.getObject(i);
					
					//Create a new photo
					newPhoto(curFace, label, age);
				}

				//If non of the photos have a screenshot yet; capture them.
				if (!captured) saveScreen();
			}
		}
	}

	frameCounter++;
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofPushMatrix();
		////ofset drawing position
		ofTranslate(20, 20);
		ofScale(4, 4);

		// draw the webcam img
		ofSetHexColor(0xffffff);
		colorImg.draw(0,0);

		// draw the whole finder
		// No parameters in draw function
		//finder.draw();

		// or, instead we can draw each blob individually from the blobs vector,
		for (int i = 0; i < finder.size(); i++){
			ofRectangle curFace = finder.getObject(i);
			ofSetColor(255);
			ofNoFill();
			ofDrawRectangle(curFace);

			int label = finder.getLabel(i);
			int age = finder.getTracker().getAge(label);
			ofPoint center = curFace.getCenter();

			//Draw marker
			ofFill();
			ofDrawRectangle(center.x - 5, center.y - 15, 20, 20);
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

void ofApp::saveScreen()
{
	colorImg.saveImage("/screenshots/" + ofToString(frameCounter) + ".png");
	captured = true;
}

void ofApp::newPhoto(ofRectangle curFace, int label, int age)
{
	//Crop the face
	ofImage cropFace;
	cropFace.cropFrom(colorImg, curFace.x, curFace.y, curFace.getWidth(), curFace.getHeight());

	//Store the face
	string savelocation = "/mugshots/" + ofToString(label) + "/ " + ofToString(age) + ".png";
	cropFace.saveImage(savelocation);

	//Update the variables
	faces[label].lastPhoto = age;
	faces[label].mugshots++;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
