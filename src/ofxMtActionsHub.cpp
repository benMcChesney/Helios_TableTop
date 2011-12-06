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

#include "ofxMtActionsHub.h"
//ofMutex mutex ;
//--------------------------------------------------------------
ofxMtActionsHub::ofxMtActionsHub(){
	mtObjects.clear();
	mtObjectsDisplayStack.clear();
}

//--------------------------------------------------------------
ofxMtActionsHub::~ofxMtActionsHub(){
	mtObjects.clear();
	mtObjectsDisplayStack.clear();
}

//--------------------------------------------------------------
void ofxMtActionsHub::addObject(ofxMtActionsObject* mtObject){
	mtObjects.push_back(mtObject);

	//reorder list of mtActionsObject by Priority Level
	CPriotityCompare pc;
	mtObjects.sort(pc);
}

//--------------------------------------------------------------
void ofxMtActionsHub::removeObject(ofxMtActionsObject* mtObject){
	mtObjects.remove(mtObject);
}

//--------------------------------------------------------------
void ofxMtActionsHub::update(){
	list<ofxMtActionsObject*>::iterator it;
	for (it=mtObjects.begin(); it!=mtObjects.end(); it++) {
		(*it)->update();
	}
}

//--------------------------------------------------------------
void ofxMtActionsHub::renderAsStack() {
	mtObjectsDisplayStack = mtObjects;

	//reorder list of mtActionsObject by zDepth Level
	CLevelCompare lc;
	mtObjectsDisplayStack.sort(lc);

//    mutex.lock() ;
	list<ofxMtActionsObject*>::iterator it;
	for (it=mtObjectsDisplayStack.begin(); it!=mtObjectsDisplayStack.end(); it++) {
		(*it)->render();
	}

//	mutex.unlock() ;

}

//--------------------------------------------------------------
void ofxMtActionsHub::touchDown(float _x, float _y, int touchId, ofxMultiTouchCustomData *data) {
	list<ofxMtActionsObject*>::iterator it;
	list<ofxMtActionsObject*>::iterator nextIt;
	nextIt = mtObjects.begin();
	nextIt++;

	bool touchDownConsumed = false;
	for (it=mtObjects.begin(); it!=mtObjects.end(); it++) {
		(*it)->actionTouchDown(_x, _y, touchId, data);

		if((*it)->ownTouchCursor(((ofxMultiTouchCustomDataSF*)data)->sessionID)) {
			touchDownConsumed = true;
		}

		if (nextIt!=mtObjects.end()) {
			if(touchDownConsumed && (*it)->getPriorityLevel() < (*nextIt)->getPriorityLevel()) {
				//If the touch/tuioCursor is interacting with this object
				//and there is no other object with the same priority level then stop the loop
				return;
			}
			nextIt++;
		} else {
			return;
		}

	}
}

//--------------------------------------------------------------
void ofxMtActionsHub::touchMoved(float _x, float _y, int touchId, ofxMultiTouchCustomData *data) {
	list<ofxMtActionsObject*>::iterator it;

	bool touchDownConsumed = false;
	for (it=mtObjects.begin(); it!=mtObjects.end(); it++) {
		if((*it)->ownTouchCursor(((ofxMultiTouchCustomDataSF*)data)->sessionID)) {
			(*it)->actionTouchMoved(_x, _y, touchId, NULL);
		}
	}
}

//--------------------------------------------------------------
void ofxMtActionsHub::touchUp(float _x, float _y, int touchId, ofxMultiTouchCustomData *data) {
	list<ofxMtActionsObject*>::iterator it;
	for (it=mtObjects.begin(); it!=mtObjects.end(); it++) {
		(*it)->actionTouchUp(_x, _y, touchId, NULL);
	}
}
