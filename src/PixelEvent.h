//
//  PixelEvent.h
//  Helios_TableTop
//
//  Created by Ben McChesney on 12/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PIXELEVENT
#define PIXELEVENT

#include "PixelEventArgs.h"
#include "TransitionEventArgs.h"

class PixelEvent
{
    public : 
        PixelEvent( ) { } 
        virtual ~PixelEvent ( ) { } 
    
    
        static PixelEvent* Instance()
        {
            static PixelEvent inst ; 
            return &inst ; 
        }
    
        ofEvent<PixelEventArgs> pixelDownEvent ; 
        ofEvent<PixelEventArgs> pixelUpEvent ;
    
    
        ofEvent<TransitionEventArgs> playTransitionMovieEvent ; 
  
};

#endif

