/*
 MultitouchActions for openFrameworks
 Copyright (c) 2010 Hugues Bruyère - <http://www.smallfly.com>

 Some parts based on source code by Marek Bereza - <http://www.mrkbrz.com/>

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


/*TODO
- Fix the integration with ofxMtObject and the rotatable object (for the hit test) ...
  for now I have a quick fix, and rotatable object can not be used with 'registerForMultitouchEvents' and 'ofxMtActionsObjectEvents / ofxMtObjectEvents'
- Check the 'stopActionOnRollOut' thing
*/

#include "ofxMtActionsObject.h"

//--------------------------------------------------------------
ofxMtActionsObject::ofxMtActionsObject(bool _draggable, bool _scalable, bool _rotatable){
	priorityLevel = -1;
	draggable = _draggable;
	scalable = _scalable;
	rotatable = _rotatable;

	//INIT
	state = FIXE;
	stopActionOnRollOut = true;
	limitToScreen = true;
	drawTouchCounts = false;

	accelerationSmoothing = 0.5;
	decelerationSmoothing = 0.9;
	rotationalSmoothing = 0.9;

	minSize = 100;
	maxSize = 800;
	scaleSmoothing = 0.9;

	lastNumberOfTouches = 0;

	acceleration = ofxVec2f(0,0);
	rotationalAcceleration = 0.0f;

	contentWidth = 800;
	contentHeight = 600;
	width = 400;
	height = 300;

	highestSessionID = -1;

}

//--------------------------------------------------------------
ofxMtActionsObject::~ofxMtActionsObject(){
}

//--------------------------------------------------------------
void ofxMtActionsObject::update() {
	bool moved = false;

	if(touches.size()>0) {

		// add up all the movements
		int deltaCount = 0;
		centroid = ofxVec2f(0,0);
		ofxVec2f deltaTotal = ofxVec2f(0,0);

		map<int,ofxMtActionTouch>::iterator it;
		// find the centroid and how far it's moved
		for ( it=touches.begin() ; it != touches.end(); it++ ) {
			ofxMtActionTouch t = touches[(*it).first];
			centroid += t;
			ofxVec2f delta = t.delta();

			if(!t.isNew) {
				if(ABS(delta.x) + ABS(delta.y)>0) {
					deltaCount++;
					deltaTotal += delta;
				}
			}
		}

		centroid /= touches.size();

		// see how far we are from the centroid on average, and how far each has rotated about
		float angleDelta = 0;
		float scaleDelta = 0;
		int angleCount = 0;

		//reset last centroid if there's only one touch
		if(touches.size()==1) {
			(*(touches.begin())).second.setLastCentroid(centroid);
		}

		for ( it=touches.begin() ; it != touches.end(); it++ ) {
			touches[(*it).first].setCentroid(centroid);
			if(!touches[(*it).first].isNew) {
				// remember each change in rotation about the centroid
				angleDelta += touches[(*it).first].angleDelta();
				angleCount++;
				// remember each change in distance from the centroid
				scaleDelta += touches[(*it).first].scaleDelta();
			}
		}
		angleDelta /= angleCount;
		scaleDelta /= angleCount;

		if(deltaCount>0) {
			ofxVec2f delta = deltaTotal/deltaCount;
			if(lastNumberOfTouches==1 && touches.size()>1) {
				transform(delta, 0, 1 );
			} else if(scaleDelta==scaleDelta) { // checks for nan, nan!=nan
				transform(delta, -angleDelta, scaleDelta);
			} else {
				transform(delta, -angleDelta, 1);
			}
			moved = true;
		}


		// copy touches to old touches
		for ( it=touches.begin() ; it != touches.end(); it++ ) {
			touches[(*it).first].push();
		}

	}

	if(!moved) {
		transform(ofxVec2f(0,0), 0, 1); // damping
	}

	updateContent();

	if (stopActionOnRollOut) {
		// check other touch to see if they are still on the mtActionsObject
		// after the call to transform()
		map<int,ofxMtActionTouch>::iterator it;
		for ( it=touches.begin() ; it != touches.end(); it++ ) {
			ofxMtActionTouch t = touches[(*it).first];
			if(!actionTouchHitTest(t.x, t.y)) {
				if(it!=touches.end()) {
					touches.erase(it);
				}
			}
		}
	}

	updateState();

	lastNumberOfTouches = touches.size();
}

//--------------------------------------------------------------
void ofxMtActionsObject::updateContent() {}

//--------------------------------------------------------------
void ofxMtActionsObject::drawContent() {
	ofSetHexColor(0x000000);
	ofRect(-width/2, -height/2, width, height);
}

//--------------------------------------------------------------
void ofxMtActionsObject::render(){
	glColor3f(1,1,1);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glRotatef(180.0f*rotation/PI, 0, 0, 1);
		drawContent();
		//ofRect(0, 0, width, height);
		//ofRect(-width/2, -height/2, width, height);
	glPopMatrix();


	if(drawTouchCounts) {
		glColor3f(1,1,1);
		glPushMatrix();
		glTranslatef(x + 30, y, 0);
		ofDrawBitmapString(ofToString((int)touches.size())+ " touches \n"
						   + "priority level : " + ofToString(priorityLevel) + "\n"
						   + "highest sessionId : " + ofToString(highestSessionID)
						   , 0, 15+width/2);
		glPopMatrix();
	}

	if (DEBUG) {
		ofSetColor(255, 255, 255);
		ofRect(centroid.x - 5, centroid.y - 5, 10, 10);
	}


}

//--------------------------------------------------------------
void ofxMtActionsObject::actionTouchDown(float _x, float _y, int touchId, ofxMultiTouchCustomData *data) {
	// see if the touch lands on the mtActionsObject
	if(actionTouchHitTest(_x*ofGetWidth(), _y*ofGetHeight())) {
		// see if it already exists
		map<int,ofxMtActionTouch>::iterator it = touches.find(touchId);
		if(it!=touches.end()) {
			touches[touchId].x = _x*ofGetWidth();
			touches[touchId].y = _y*ofGetHeight();
		} else {
			touches[touchId] = ofxMtActionTouch(((ofxMultiTouchCustomDataSF*)data)->sessionID, _x*ofGetWidth(), _y*ofGetHeight());
		}

		if (highestSessionID < ((ofxMultiTouchCustomDataSF*)data)->sessionID) {
			highestSessionID = ((ofxMultiTouchCustomDataSF*)data)->sessionID;
		}

		updateState();

	} else {
		// check if the touch exists in this object and remove it
		map<int,ofxMtActionTouch>::iterator it = touches.find(touchId);
		if(it!=touches.end()) {
			touches.erase(it);
		}
	}
}

//--------------------------------------------------------------
void ofxMtActionsObject::actionTouchMoved(float _x, float _y, int touchId, ofxMultiTouchCustomData *data) {
	if(state != FIXE) {
		// see if the touch had previously landed on the mtActionsObject
		map<int,ofxMtActionTouch>::iterator it = touches.find(touchId);
		if(it!=touches.end()) {
			touches[touchId].x = _x*ofGetWidth();
			touches[touchId].y = _y*ofGetHeight();
		}
	}
}

//--------------------------------------------------------------
void ofxMtActionsObject::actionTouchUp(float _x, float _y, int touchId, ofxMultiTouchCustomData *data) {
	//(*it)->touchUp(_x, _y, touchId, data);
	// remove the blob
	if(actionTouchHitTest(_x*ofGetWidth(), _y*ofGetHeight())) {
		map<int,ofxMtActionTouch>::iterator it = touches.find(touchId);
		if(it!=touches.end()) {
			touches.erase(it);
		}
	}

	updateState();

}






//--------------------------------------------------------------
void ofxMtActionsObject::setPriorityLevel(int _pLevel){
	priorityLevel = _pLevel;
}

//--------------------------------------------------------------
int ofxMtActionsObject::getSessionID() {
	return highestSessionID;
}

//--------------------------------------------------------------
int ofxMtActionsObject::getPriorityLevel(){
	return priorityLevel;
}

//--------------------------------------------------------------
void ofxMtActionsObject::setIsDraggable(bool _draggable, int* _nbOfTouchForDrag, int arrSize, bool _horizontal, bool _vertical){
	draggableH = _horizontal;
	draggableV = _vertical;
	draggable = _draggable;
	nbOfTouchForDrag = new int[arrSize];
	dragSpecLength = arrSize;

	for (int i=0; i<dragSpecLength; i++) {
		nbOfTouchForDrag[i] = _nbOfTouchForDrag[i];
	}
}

//--------------------------------------------------------------
void ofxMtActionsObject::setIsScalable(bool _scalable, int* _nbOfTouchForScale, int arrSize){
	scalable = _scalable;
	nbOfTouchForScale = new int[arrSize];
	scaleSpecLength = arrSize;

	for (int i=0; i<scaleSpecLength; i++) {
		nbOfTouchForScale[i] = _nbOfTouchForScale[i];
	}
}

//--------------------------------------------------------------
void ofxMtActionsObject::setIsRotatable(bool _rotatable, int* _nbOfTouchForRotate, int arrSize){
	rotatable = _rotatable;
	nbOfTouchForRotate = new int[arrSize];
	rotateSpecLength = arrSize;

	for (int i=0; i<rotateSpecLength; i++) {
		nbOfTouchForRotate[i] = _nbOfTouchForRotate[i];
	}
}

//--------------------------------------------------------------
bool ofxMtActionsObject::isDraggable(){
	return draggable;
}

//--------------------------------------------------------------
bool ofxMtActionsObject::isScalable(){
	return scalable;
}

//--------------------------------------------------------------
bool ofxMtActionsObject::isRotatable(){
	return rotatable;
}

//--------------------------------------------------------------
void ofxMtActionsObject::updateState() {
	if(touches.size() == 0) {
		state = FIXE;
	} else if(touches.size() == 1) {
		state = DRAGGING;
	} else if(touches.size() >= 3) {
		state = DRAGGING;
	} else if(touches.size() == 2) {
		state = ROTATESCALE;
	}


}

//--------------------------------------------------------------
void ofxMtActionsObject::transform(ofxVec2f vec, float angle, float _scale) {
	if(_scale<=0) _scale = 1.0f;

	if(touches.size()==0) {
		if(draggableH)
			acceleration.x = acceleration.x*decelerationSmoothing + vec.x*(1.0f-decelerationSmoothing);
		if(draggableV)
			acceleration.y = acceleration.y*decelerationSmoothing + vec.y*(1.0f-decelerationSmoothing);
	} else {
		if(draggableH)
			acceleration.x = acceleration.x*accelerationSmoothing + vec.x*(1.0f-accelerationSmoothing);
		if(draggableV)
			acceleration.y = acceleration.y*accelerationSmoothing + vec.y*(1.0f-accelerationSmoothing);
	}

	rotationalAcceleration = rotationalAcceleration*rotationalSmoothing + angle*(1.0f - rotationalSmoothing); // lerp

	if(draggable) {
		for (int i=0; i<dragSpecLength; i++) {
			if ((touches.size() == abs(nbOfTouchForDrag[i])) || (nbOfTouchForDrag[i] < 0 && touches.size() >= abs(nbOfTouchForDrag[i]))) {
				if(draggableH)
					x += acceleration.x;

				if(draggableV)
					y += acceleration.y;

				break;
			}
		}
	}

	if(rotatable){
		for (int i=0; i<rotateSpecLength; i++) {
			if ((touches.size() == abs(nbOfTouchForRotate[i])) || (nbOfTouchForRotate[i] < 0 && touches.size() >= abs(nbOfTouchForRotate[i]))) {
				rotation += rotationalAcceleration;
				break;
			}
		}
	}

	if(scalable) {
		for (int i=0; i<scaleSpecLength; i++) {
			if ((touches.size() == abs(nbOfTouchForScale[i])) || (nbOfTouchForScale[i] < 0 && touches.size() >= abs(nbOfTouchForScale[i]))) {
				width = scaleSmoothing*width*_scale + (1.0f - scaleSmoothing)*width;
				height = contentHeight*width/contentWidth;

				if(width<minSize) {
					width = minSize;
					height = contentHeight*width/contentWidth;
				} else if(height<minSize) {
					height = minSize;
					width = contentWidth*height/contentHeight;
				} else if(width>maxSize) {
					width = maxSize;
					height = contentHeight*width/contentWidth;
				} else if(height>maxSize) {
					height = maxSize;
					width = contentWidth*height/contentHeight;
				}

				break;
			}
		}
	}


	if(x<0) x = 0;
	if(x>ofGetWidth()) x = ofGetWidth();
	if(y<0) y = 0;
	if(y>ofGetHeight()) y = ofGetHeight();

}

//--------------------------------------------------------------
bool ofxMtActionsObject::ownTouchCursor(int touchCursorSessionID) {
	map<int,ofxMtActionTouch>::iterator it;
	for ( it=touches.begin() ; it != touches.end(); it++ ) {
		ofxMtActionTouch t = touches[(*it).first];
		if(t.sessionID == touchCursorSessionID) {
			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------
bool ofxMtActionsObject::actionTouchHitTest(float _x, float _y) {
	if (rotatable || rotation != 0) {
		ofxVec2f p = ofxVec2f(_x, _y);
		p.x -= x;
		p.y -= y;
		p.rotateRad(-rotation);
		lastHit = p;
		p.x += x;
		p.y += y;

		return hitTest(p.x + width/2, p.y + height/2);
	} else {
		return hitTest(_x, _y); //
	}

}
