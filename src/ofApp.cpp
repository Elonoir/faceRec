#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){

	vidGrabber.setVerbose(true);
    vidGrabber.setup(320,240);

	threshold = 80;

	finder.setup("haarcascade_frontalface_default.xml");

	//set amount of frames before forgetting something.
	finder.getTracker().setPersistence(15);
	// an object can move up to 32 pixels per frame
	finder.getTracker().setMaximumDistance(32);
	
	//Allocate the files needed for the recognition
	gray.allocate(PCA_WIDTH, PCA_HEIGHT); //
	color.allocate(PCA_WIDTH, PCA_HEIGHT); //
	
	//Get path to all the different mugshots
	filepaths = getFilepaths();

	//Setup the face recognition
	rec.learn(filepaths);// Learn facefinder with the possible faces
}

//--------------------------------------------------------------
void ofApp::update(){

	// check for new img in cam
    vidGrabber.update();
	captured = false;

	// if new image check for faces.
	if (vidGrabber.isFrameNew()){
		//Set color image to new frame
        colorImg.setFromPixels(vidGrabber.getPixels()); //img.setFromPixels in example
		
		//Find new faces
		finder.update(colorImg); //finder.update(test_image) in example

		//Check faces and store new photos if needed
		RectTracker& results = finder.getTracker();
		for (size_t i = 0; i < finder.size(); i++)
		{
			//Get label belonging to current places in vector/array
			unsigned int label = results.getLabelFromIndex(i), age = results.getAge(label);

			//If detected face is older than xx frames save an mugshot
			if (age > 16) {
				//If there exist no face for this face create it.
				if (faces.find(label) == faces.end()) addFace(label);

				//if there not yet 5 photos and the frames since last photo is more than 25 store
				if (faces[label].mugshots < 5 && age - faces[label].lastPhoto > 8 * faces[label].mugshots)
					//Create new photo
					newPhoto(finder.getObject(i), label, age);
			}
		}

		//Get number of closest matching photograph // not ID yet
		int person = rec.recognize(gray);
		cout << "Best match: " << person << endl;
	}

	framecount++; // Update the framecount
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
	colorImg.saveImage("/screenshots/" + ofToString(framecount) + ".png");
	captured = true;
}

void ofApp::addFace(int label)
{
	face newFace;
	newFace.mugshots = 0;
	newFace.label = label;
	faces[label] = newFace;
}

void ofApp::newPhoto(ofRectangle curFace, int label, int age)
{
	//Crop the face
	ofImage cropFace;
	cropFace.cropFrom(colorImg, curFace.x, curFace.y, curFace.getWidth(), curFace.getHeight());

	//Store the face
	string savelocation = "/mugshots/" + ofToString(label) + "/" + ofToString(age) + ".png";
	cropFace.saveImage(savelocation);

	//Update the variables
	faces[label].lastPhoto = age;
	faces[label].mugshots++;

	//If non of the photos have a screenshot yet; capture them.
	if (!captured) saveScreen();
}

vector<string> ofApp::getFilepaths()
{
	//Create vector to return with all filepaths
	vector<string> filepaths;
	//some path, may be absolute or relative to bin/data
	ofDirectory dir("\\mugshots\\");
	//populate the directory object
	dir.listDir();
	//go through and print out all the paths
	for (int i = 0; i < dir.size(); i++) {
		ofDirectory dirB(dir.getPath(i));
		//only show png files
		dir.allowExt("png");
		//populate the directory object
		dirB.listDir();
		for (int j = 0; j < dirB.size(); j++) {
			ofLogNotice(dirB.getPath(j));
		}
	}
	return filepaths;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
