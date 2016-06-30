#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxCvFaceRec.h"
#include "face.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);	

		//Methods specifically for this sketch
		void saveScreen();
		void addFace(int label);
		void newPhoto(ofRectangle curFace, int label, int age);
		vector<string> getFilepaths();

		//variables for video and image data
		ofVideoGrabber 			vidGrabber;
		ofImage					colorImg;
		// Variables for display
		bool					fullscreen;

		//Finders for faces and image recognition
        ofxCv::ObjectFinder 	finder;
		ofxCvFaceRec			rec;
		//Variables used during recognition of face
		ofxCvColorImage			color;
		ofxCvGrayscaleImage		gray;

		//Variables for my own way of working
		int 					threshold;
		unsigned long			framecount;
		bool					captured;
		vector<string>			filepaths;
		std::map<int, face>		faces;
};

