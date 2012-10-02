#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
   
    //ofSetFrameRate(60);
	
    ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
    	
	
	nearThreshold = 230;
	farThreshold = 90;
    kinectOn = false;
	
    
	framenum=0;

   
    for(int i=0; i<400000; i++){
        
        boids.push_back(new Boid(ofRandomWidth(), ofRandomHeight()));
        
    }
	
    /*
    ofSetColor(255);
    vbo.setVertexData(&boids[0]->loc, (int)boids.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_POINTS, 0, (int)boids.size());
    */
    

}

//--------------------------------------------------------------
void testApp::update(){
    
    ofBackground(0, 0, 0);
    
    kinect.update();
    
    // there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} 
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 100, (kinect.width*kinect.height)/2, 20, false);
        
	}

    

    for(int i=0; i<boids.size(); i++)
	{
	
        boids[i]->intersects(contourFinder);
		boids[i]->update();

      
    }
    
    boids.push_back(new Boid(ofRandomWidth(), ofRandomHeight()));

}

//--------------------------------------------------------------
void testApp::draw(){
    
  
    
    ofSetColor(255, 255, 255);
    
	kinectImage(); 
    
   
    
	
	for(int i=0; i<boids.size(); i++) {
    
      
        boids[i]->draw();
        
         }
              

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
            cout<<"Threshold: "<<farThreshold<<"\n";
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
            
            cout<<"Threshold: "<<farThreshold<<"\n";
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
        
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
            
        case 'd':
            for(int i = 0; i < boids.size(); i++)
            {
                kinectOn = !kinectOn;
            }
			break;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
   
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::capture() {
	if(doCapture && ofGetFrameNum() % 4 == 0)
	{
		char filename[255];
		sprintf(filename, "frame%05d.png", framenum++);
		ofSaveScreen(filename);
	}
}



void testApp::kinectImage(){

    
    if(kinectOn) {
        contourFinder.draw(0,0);    
        
        // draw instructions
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl
        << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
        << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
        << "set near threshold " << nearThreshold << " (press: + -)" << endl
        << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
        << ", fps: " << ofGetFrameRate() << endl
        << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
        << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;
        
        ofDrawBitmapString(reportStream.str(),20,790);

        
    }
    
}



