//
//  CrossFadeImage.h
//  Helios_TableTop
//
//  Created by Ben McChesney on 12/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "ofMain.h"
#include "Tweenzor.h"

class CrossFadeImage 
{
    public : 
        CrossFadeImage( ) 
        {
            front = NULL ; 
            back = NULL ;  
            inTransition = false ; 
            frontAlpha = 0.0f ; 
            backAlpha = 0.0f ; 
        } 
    
        ofImage * front ; 
        ofImage * back ; 
//        ofImage * main ;            //Image left over after 
        float frontAlpha ; 
        float backAlpha ; 
        bool inTransition ; 
    
        void fadeImages ( ofImage * _front , ofImage * _back , float duration = 0.5f, float delay = 0.0f ) 
        {
            front = _front ; 
            back = _back ; 
            frontAlpha = 0.0f ; 
            backAlpha = 1.0f ; 
            inTransition = true ; 
            Tweenzor::add( &frontAlpha , frontAlpha , 1.0f , delay , duration , EASE_OUT_QUAD ) ;
            Tweenzor::add( &backAlpha , backAlpha , 0.0f , delay , duration , EASE_OUT_QUAD ) ;
            Tweenzor::addCompleteListener ( Tweenzor::getTween( &backAlpha ) , this , &CrossFadeImage::fadeComplete ) ;
        }
    
    
        void fadeComplete ( float * args ) 
        {
            inTransition = false ; 
            Tweenzor::removeCompleteListener( Tweenzor::getTween( &backAlpha ) ) ; 
        }
    
        void draw ( ) 
        {
            if ( inTransition == false && front != NULL ) 
                front->draw( -front->width * 0.5f , -front->height * 0.5f , front->width , front->height ) ;
            else
            {
                
                if ( front != NULL ) 
                {
                    ofSetColor( 255 , 255 , 255 , frontAlpha * 255.0f ) ;
                    front->draw( front->width * -.5 , front->height * -.5 , front->width , front->height ) ; 
                }
                if ( back != NULL ) 
                {
                    ofSetColor( 255 , 255 , 255 , backAlpha * 255.0f ) ;
                    back->draw( back->width * -.5 , back->height * -.5 , back->width , back->height ) ; 
                }
                
            }
        }
        /*
         void ContentItem::fadeImages ( ofImage * image1 , ofImage * image2 , float duration , float delay , bool tier2Fade )
         {
         fadeImage1 = image1 ;
         fadeImage2 = image2 ;
         fadeAlpha1 = 0.0f ;
         fadeAlpha2 = 1.0f ;
         isFading = true ;
         
         Tweenzor::add( &fadeAlpha1 , fadeAlpha1 , 1.0f , delay , duration , EASE_OUT_QUAD ) ;
         Tweenzor::add( &fadeAlpha2 , fadeAlpha2 , 0.0f , delay , duration , EASE_IN_QUAD ) ;
         Tweenzor::addCompleteListener ( Tweenzor::getTween( &fadeAlpha2 ) , this , &ContentItem::fadeComplete ) ; 
         };
         
         void ContentItem::fadeComplete ( float * args )
         {
         Tweenzor::removeCompleteListener( Tweenzor::getTween( &fadeAlpha2 ) ) ; 
         image = * fadeImage1 ;
         isFading = false ;
         };
        */
    
    
};