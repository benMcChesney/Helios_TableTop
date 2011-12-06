//
//  Header.h
//  ofxTuio
//
//  Created by Helios on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef _IMAGE_CONTAINER
#define _IMAGE_CONTAINER

#include "ofImage.h"


class ImageContainer : public ofImage
{
    public : 
    
        float x , y , alpha ; 
        float scale ; 
        float startSize ; 
    
        ImageContainer()
        {
            alpha = 1.0f ; 
            scale = 1.0f ; 
            startSize = 50.0f ; 
        } ; 
};

#endif