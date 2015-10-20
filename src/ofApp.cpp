#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(50, 50, 50, 0);
	//we need to call this for textures to work on models
    ofDisableArbTex();
	//this makes sure that the back of the model doesn't show through the front
	ofEnableDepthTest();

	texture.loadImage("texture.jpg");
    objShape.loadObj("data/sphere_06.obj");
    objShape.reverseNormals();
    objShape.setTexture(texture);

    //setupCam();

	//light.enable();
    light.setPosition(-500,0,-100);

    cam.begin();
    cam.setPosition(0,0,50);
    objShape.mapPlannar(cam);
    //cam.enableOrtho();
    cam.end();
}
void ofApp::setupCam(){

	camWidth 		= 640;	// try to grab at this size.
	camHeight 		= 480;

    //we can now get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);

	//videoInverted 	= new unsigned char[camWidth*camHeight*3];
	//videoTexture.allocate(camWidth,camHeight, GL_RGB);
	ofSetVerticalSync(true);
}
//--------------------------------------------------------------
void ofApp::update(){

    //vidGrabber.update();

    //unsigned char * pixels  = vidGrabber.getPixels();
    //texture.setFromPixels(pixels,vidGrabber.getWidth(),vidGrabber.getHeight(),OF_IMAGE_COLOR);
    //texture.resize(1280,720);
    //objShape.setTexture(texture);

    textureBlurred = texture;
    textureBlurred.resize(16,12);
    textureBlurred.resize(640/4,480/4);

}

//--------------------------------------------------------------
void ofApp::draw(){

    textureBlurred.draw(0,0);

    cam.begin();
    cam.setPosition(0,0,50);
    ofRotate(ofGetFrameNum(), 0, 1, 0);
    objShape.show();
    objShape.mapByLight(ofVec3f(1,0,0),textureBlurred);
    cam.end();


    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
