/*
 MultitouchActions for openFrameworks 
 Copyright (c) 2010 Hugues Bruyère - <http://www.smallfly.com>
  
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OFX_MTACTIONSHUB
#define _OFX_MTACTIONSHUB

#include "ofxMtActionsObject.h"
#include "ofxMtActionTouch.h"


class CPriotityCompare {
	//Use to order the mtActionsObject by priority level
	public: 
		bool operator() (ofxMtActionsObject* mtObjectA, ofxMtActionsObject* mtObjectB){
			if (mtObjectA->getPriorityLevel() > mtObjectB->getPriorityLevel()) {
				return false;
			} else {
				return true;
			}
		}
};

class CLevelCompare {
	//Use to order the mtActionsObject by zDepth level
public: 
	bool operator() (ofxMtActionsObject* mtObjectA, ofxMtActionsObject* mtObjectB){
		if (mtObjectA->getSessionID() > mtObjectB->getSessionID()) {
			return false;
		} else {
			return true;
		}
	}
};

class ofxMtActionsHub {
	
public:
	
	ofxMtActionsHub();
	~ofxMtActionsHub();
	
	void addObject(ofxMtActionsObject* mtObject);
	void removeObject(ofxMtActionsObject* mtObject);
	
	void update();
	void renderAsStack();
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	
private:
	
	list<ofxMtActionsObject*> mtObjects;
	list<ofxMtActionsObject*> mtObjectsDisplayStack;
	
};

#endif