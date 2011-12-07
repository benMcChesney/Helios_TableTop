//
//  PixelEventArgs.h
//  Helios_TableTop
//
//  Created by Ben McChesney on 12/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PIXELEVENTARGS
#define PIXELEVENTARGS
#include "ofMain.h"

class PixelEventArgs : public ofEventArgs
{
    public:
    
        PixelEventArgs( int _inputHex ) { inputHex = _inputHex; }
        virtual ~PixelEventArgs( ) { }
    
        //Add whatever you need !
        int inputHex ; 
    
};

#endif