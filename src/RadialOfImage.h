//
//  RadialOfImage.h
//  Salesforce_SustainabilityTables_2011
//
//  Created by Helios on 8/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef RADIAL_IMAGE
#define RADIAL_IMAGE


#define HORIZONTAL 19 
#define VERTICAL 21 

#include "ofMain.h"

class RadialOfImage {
	
public:
    
    bool selected ; 
    
    ofImage image_out ; 
    ofImage image_over ; 
    
    float angle ; 
    float angleDistance ; 
    float originalDistance ; 
    
    ofPoint position ; 
    ofPoint stagePos ; 
    
    
    float value ; 
    
    RadialOfImage ( ) 
    {
        selected = false ; 
        angle = ofRandom ( 0 , PI * 2.0f ) ;
        angleDistance = ofRandom ( 5 , 25 ) ;
        value = 25.0f ; 
    }
    
    void draw ( float width , float height , float size ) 
    {
        if ( selected == true ) 
        {
            image_over.draw( -width/2 + position.x +-size/2 , position.y +-size/2 , size , size ) ;    
        }
        else
        {
            image_out.draw( -width/2 + position.x +-size/2 , position.y +-size/2 , size , size ) ;    
        }
        //float nearest = floorf(val * 100 +  0.5) / 100;
     //   ofDrawBitmapString( ofToString( floorf(value * 100 +  0.5) / 100 ) , -10 + position.x , position.y + -size * 0.5f ) ; 
    }
};

#endif