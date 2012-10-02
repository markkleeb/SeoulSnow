/*
 *  Boid.cpp
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "Boid.h"


Boid::Boid(float x, float y) {

    loc.x = x;
	loc.y = y;   
    
	acc = 0.98;
	
    
    vel.x = ofRandom(-1, 1);
    vel.y = ofRandom(-1, 1);
    debug = false;
    
    
    counter = 0;
    avoidObject = false;
    
}


// Method to update location
void Boid::update() {
    
    
  
    vel += acc;   // Update velocity
    vel.y += 0.04;  // add gravity
	
   
    loc += vel;
    
 
    
    acc *= 0.98;  //update gravity
    
    //wrap borders
    if(loc.x > ofGetWidth()) loc.x = 1;
    if(loc.x < 0)           loc.x = ofGetWidth()-1;
    if(loc.y > ofGetHeight()) loc.y = 1;
    	

}


void Boid::draw() {
    
    // draw the points the slow way
    
    ofSetColor(255);
    ofEllipse(loc.x, loc.y, 1, 1);
       
    
    
	
    
}

void Boid::intersects(ofxCvContourFinder& _cv){
    
   

    
    ofPoint heading = loc + vel*30;  // A vector pointing from the location to where the boid is heading
    
    for ( int i = 0; i < _cv.blobs.size(); i++ ) {
        ofxCvBlob temp = _cv.blobs[i];
        ofPolyline l;
        l.addVertexes(temp.pts);
        
        if(l.inside(heading))
        {   
            counter = 0;
            avoidObject = true;
            ofPoint force = heading - _cv.blobs[i].centroid;
            ofPoint force2 = getNormalPoint(force, heading, _cv.blobs[i].centroid);
            acc += force.normalize()* 1.5;
                      
        } else {
            avoidObject = false;
            counter++;
        }
        
        
    }    
    
    
        
}

ofPoint Boid::getNormalPoint(ofPoint p, ofPoint a, ofPoint b) {
	
    // Vector from a to p
    ofPoint ap = p - a;
	
    // Vector from a to b
    ofPoint ab = b - a;
	
    // Project vector "diff" onto line by using the dot product
       return a + ab;
}






