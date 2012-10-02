/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef BOID_H
#define BOID_H

#include "ofMain.h"
#include "Blob.h"
#include <list>
#include "ofxCvBlob.h"
#include "ofxCvContourFinder.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"



class Boid {
    
    int objAvoidScalar;
    ofImage i;
    int counter;
    bool avoidObject;
    
public:
	Boid(float x, float y);
	
	void update();
	void draw();
	
	ofVec2f loc;
	ofVec2f vel;
	ofVec2f acc;
    
    ofVec2f normal;
    ofVec2f target;
    ofVec2f predictLoc;
    ofVec2f dir;
	float r;
	float maxforce;
	float maxspeed;
    float wandertheta;
    float record;
    
  
    ofPoint getNormalPoint(ofPoint a, ofPoint b, ofPoint c);
    
    bool projected;
    bool debug;
    

    
    
    vector<ofPoint> p;
    
 
    
    ofPoint steer(ofPoint target, bool slowdown);
    
    
    void intersects(ofxCvContourFinder& _cf);
};

#endif