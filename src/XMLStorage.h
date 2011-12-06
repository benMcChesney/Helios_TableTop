//
//  XMLStorage.h
//  ofxTuio
//
//  Created by Helios on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef _XML_STORAGE
#define _XML_STORAGE



class XMLStorage
{
    public :


    XMLStorage () {
        startValue = 0 ;
        curValue = 0 ;
    } ;

    XMLStorage ( string _xmlPath )
    {
        startValue = 0 ;
        curValue = 0 ;
        xmlPath = _xmlPath ;
    };

    XMLStorage( int _startValue , string _xmlPath )
    {
        startValue = _startValue ;
        curValue = _startValue ;
        xmlPath = _xmlPath ;
        cout << xmlPath << " XMLStoage initiated @" << startValue << endl ;
    } ;

    int startValue ;
    int curValue ;
    string xmlPath ;

};

#endif
