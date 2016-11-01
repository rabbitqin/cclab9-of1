#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	
	video.setup(320, 240);
	finder.setup("haarcascade_frontalface_default.xml");
	usePreview = false;
    state = false;
	
	previewCamera.setDistance(3.0f);
	previewCamera.setNearClip(0.01f);
	previewCamera.setFarClip(300.0f);
	previewCamera.setPosition(0.2f, 0.2f, 0.4f);
	previewCamera.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
	
	headTrackedCamera.setNearClip(0.01f);
	headTrackedCamera.setFarClip(1000.0f);
	
	//defining the real world coordinates of the window which is being headtracked
	windowWidth = 0.3f;
	windowHeight = 0.2f;
	
	windowTopLeft = ofVec3f(-windowWidth / 2.0f,
							+windowHeight / 2.0f,
							0.0f);
	windowBottomLeft = ofVec3f(-windowWidth / 2.0f,
							   - windowHeight / 2.0f,
							   0.0f);
	windowBottomRight = ofVec3f(+windowWidth / 2.0f,
								-windowHeight / 2.0f,
								0.0f);
	viewerDistance = 0.4f;
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();
	finder.findHaarObjects(video.getPixels());
	
	ofVec3f headPosition(0,0,viewerDistance);
	
	if (finder.blobs.size() > 0) {
		const ofxCvBlob & blob = finder.blobs.front();
		float cameraHeadX = blob.centroid.x;
		float cameraHeadY = blob.centroid.y;
		
		
		//since camera isn't mirrored, high x in camera means -ve x in world
		float worldHeadX = ofMap(cameraHeadX, 0, video.getWidth(), windowBottomRight.x, windowBottomLeft.x);
		
		//low y in camera is +ve y in world
		float worldHeadY = ofMap(cameraHeadY, 0, video.getHeight(), windowTopLeft.y, windowBottomLeft.y);

		headPosition = ofVec3f(worldHeadX, worldHeadY, viewerDistance);
	} else {
		if (!video.isInitialized()) {
			headPosition = ofVec3f(0.5f * windowWidth * sin(ofGetElapsedTimef()), 0.5f * windowHeight * cos(ofGetElapsedTimef()), viewerDistance);
		}
	}

	//these 2 lines of code must be called every time the head position changes
	headTrackedCamera.setPosition(headPosition);
	headTrackedCamera.setupOffAxisViewPortal(windowTopLeft, windowBottomLeft, windowBottomRight);
}

//--------------------------------------------------------------
void ofApp::drawScene(bool isPreview){
	
    ofPushStyle();
	ofNoFill();
	ofColor col(200,100,100);
    if(state){
	for (float z = 0.0f; z > -40.0f; z-= 0.1f){
		ofSetColor(255);
		ofDrawRectangle(-windowWidth / 2.0f, -windowHeight / 2.0f, z, windowWidth, windowHeight);
	}
    }
    else{

            for (float z = 0.0f; z > -40.0f; z-= 0.1f){
//                col.setHue(int(-z * 100.0f + ofGetElapsedTimef() * 10.0f) % 360);
                ofSetColor(255);
                ofDrawCircle(-windowWidth / 2.0f, -windowHeight / 2.0f, z, windowWidth);
            }
    
    }
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor(50), ofColor(0));

	if (usePreview){
		previewCamera.begin();
	}
	else{
		headTrackedCamera.begin();
	}
	
	drawScene(usePreview);
	
	if (usePreview){
		previewCamera.end();
	}
	else{
		headTrackedCamera.end();
	}

	video.draw(0, 0);
	ofPushStyle();
	ofNoFill();
    //draw in the video
    if(state){
        ofDrawBitmapString("You are in Tunnel 1! tap [space] for preview in 3d. click to check Tunnel 2", video.getWidth() + 10, 20);

	for(unsigned int i = 0; i < finder.blobs.size(); i++) {
		ofRectangle cur = finder.blobs[i].boundingRect;
		ofDrawRectangle(cur.x, cur.y, cur.width, cur.height);
	}
    }
    else{
        ofDrawBitmapString("You are in Tunnel 2! tap [space] for preview in 3d. click to check Tunnel 1", video.getWidth() + 10, 20);
        for(unsigned int i = 0; i < finder.blobs.size(); i++) {
            ofRectangle cur = finder.blobs[i].boundingRect;
            ofDrawCircle(cur.x, cur.y, cur.width);
        }
    
    }
	ofPopStyle();

	
	if (usePreview){
		ofRectangle bottomLeft(0, ofGetHeight() - 200.0f, 300.0f, 200.0f);
		
		ofPushStyle();
		ofSetColor(0);
		ofDrawRectangle(bottomLeft);
		ofPopStyle();
		
		headTrackedCamera.begin(bottomLeft);
		drawScene(true);
		headTrackedCamera.end();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
		usePreview = !usePreview;
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
    state = !state;
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
