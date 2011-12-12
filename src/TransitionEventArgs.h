//
//  TransitionEventArgs.h
//  Helios_TableTop
//
//  Created by Ben McChesney on 12/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//`

#include "ofEvents.h"

class TransitionEventArgs {

    
public:
    TransitionEventArgs ( int _menuIndex , ofVec2f _position , bool _playSubNode ) 
    {
        menuIndex = _menuIndex ; 
        position = _position ; 
        playSubNode = _playSubNode ; 
    }
    
    int menuIndex ; 
    ofVec2f position ; 
    bool playSubNode ; 
    
    
};