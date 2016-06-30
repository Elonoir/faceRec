#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1050,1680, OF_WINDOW);			// <-------- setup the GL context

	ofRunApp( new ofApp() );

}
