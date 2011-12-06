#ifndef _MYMARKERBUTTON
#define _MYMARKERBUTTON

#include "ofMain.h"
#include "ofxMarkerObject.h"

//example marker button, you could also use the multiTouch callbacks here if you wanted
//you can basically build anything ontop of the ofxMarkerObjet base class!
class myMarkerButton : public ofxMarkerObject{
	
public:
	myMarkerButton(){
		col.r = 0;
		col.g = 0;
		col.b = 0;
	}
	
	void onMarkerDown(int _sid, float _x, float _y, float _ang, ofxMarkerCustomData *data = NULL){
		col.r = ofRandom(0,1);
		col.g = ofRandom(0,1);
		col.b = ofRandom(0,1);
	};
	void onMarkerRollOver(int _sid, float _x, float _y, float _ang, ofxMarkerCustomData *data = NULL){
		col.r = ofRandom(0,1);
		col.g = ofRandom(0,1);
		col.b = ofRandom(0,1);
	};
	void onMarkerMoved(int _sid, float _x, float _y, float _ang, ofxMarkerCustomData *data = NULL){
		angle = _ang;
	};
	void onMarkerUp(int _sid, float _x, float _y, float _ang, ofxMarkerCustomData *data = NULL){
		col.r = 0;
		col.g = 0;
		col.b = 0;
	};
	void onMarkerRollOut(int _sid, float _x, float _y, float _ang, ofxMarkerCustomData *data = NULL){
		col.r = 0;
		col.g = 0;
		col.b = 0;
	};
	
	void render(){
		glColor3f(col.r, col.g, col.b);
		glPushMatrix();
		glTranslatef(x+width/2, y+height/2, 0);
		glRotatef(angle, 0,0,1);
		ofRect(-width/2,-height/2, width, height);
		glColor3f(1,1,1);
		ofLine(0,0,0,-height/2);
		glPopMatrix();
		ofDrawBitmapString("I am a Marker Button", x, y+height+15);
	};
	
protected:
	ofColor col;
	float angle;
};

#endif
