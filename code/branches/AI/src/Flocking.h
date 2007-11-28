//
//
//      TODO: testing orxonox -flocking interface
//            testing algorithm

// ueberpruefen ob vektoren relativ richtig berechnet werden
//
//My Flocking Class

#ifndef Flocking_Class
#define Flocking_Class

#include <Ogre.h>
#include <OgreVector3.h>

#include <iostream>


#endif

using namespace std;
using namespace Ogre;

class Element // An element that flocks
{

  public:
    Vector3 location;  // locationvector of the element
    Vector3 speed;  // speedvector of the element
    Vector3 acceleration;  // accelerationvector of the element

  Element() {
    acceleration = (0,0,0);
    speed = (0,0,0);
    location = (0,0,0);
  }

  Element(Vector3 location_, Vector3 speed_, Vector3 acceleration_) {
    acceleration = acceleration_;
    speed = speed_;
    location = location_;
  }

  void setValues(Vector3 location_, Vector3 speed_, Vector3 acceleration_) {
    acceleration = acceleration_;
    speed = speed_;
    location = location_;
  }

  //calculates the distance between the element and an other point given by temp
  float getDistance(Element temp) {
    Vector3 distance = temp.location-location;  //this doesn't work
    return distance.length();
  }

//EINF[GEN DES ELEMENTS
  void update(Element arrayOfElements[], const FrameEvent& time) {
    calculateAcceleration(arrayOfElements);  //updates the acceleration
    calculateSpeed(time);  //updates the speed
    calculateLocation(time);  //updates the location
  }

//EINF[GEN DES ELEMENTS
  void calculateAcceleration(Element arrayOfElements[]) {
  //calculates the accelerationvector based on the steeringvectors of
  //separtion, alignment and cohesion.
  acceleration = separation(arrayOfElements) + alignment(arrayOfElements) + cohesion(arrayOfElements);
  }

  void calculateSpeed(const FrameEvent& time) {
    speed = speed + acceleration*time.timeSinceLastFrame;
  }

  void calculateLocation(const FrameEvent& time) {
    location = location + speed*time.timeSinceLastFrame;
  }


  Vector3 separation(Element arrayOfElements[]) {
    Vector3* steering = new Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //go through all elements
    for(int i=0; i<3; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
//DUMMY SEPERATION DETECTION DISTANCE =100
      if ((distance > 0) && (distance<100)) {  //do only if actual is inside detectionradius
        Vector3 inverseDistance = actual.location-location;  //calculate the distancevector heading towards this
        inverseDistance = inverseDistance.normalise(); //does this work correctly?  //normalise the distancevector
        inverseDistance = inverseDistance/*/distance*/;  //devide distancevector by distance (the closer the bigger gets the distancevector -> steeringvector)
        *steering = *steering + inverseDistance;  //add up all significant steeringvectors
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) {
    *steering = *steering / (float)numberOfNeighbour;  //devide the sum of steeringvectors by the number of elements -> separation steeringvector
    }
    return *steering;
  }

  Vector3 alignment(Element arrayOfElements[]) {
    Vector3* steering = new Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //go through all elements
    for(int i=0; i<3; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
//DUMMY ALIGNMENT DETECTION DISTANCE = 1000
      if ((distance > 0) && (distance<1000)) {  //check if actual element is inside detectionradius
        *steering = *steering + actual.speed;  //add up all speedvectors inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
    }
    if(numberOfNeighbour > 0) {
    *steering = *steering / (float)numberOfNeighbour;  //devide the sum of steeringvectors by the number of elements -> alignment steeringvector
    }
    return *steering;
  }

  Vector3 cohesion(Element arrayOfElements[]) {
    Vector3* steering = new Vector3(0,0,0); //steeringvector
    int numberOfNeighbour = 0;  //number of observed neighbours
    //go through all elements
    for(int i=0; i<3; i++) {  //just working with 3 elements at the moment
      Element actual = arrayOfElements[i];  //get the actual element
      float distance = getDistance(actual);  //get distance between this and actual
// DUMMY COHESION DETECTION DISTANCE = 1000
      if ((distance > 0) && (distance<1000)) {  //check if actual element is inside detectionradius
        *steering = *steering + actual.location;  //add up all locations of elements inside the detectionradius
        numberOfNeighbour++;  //counts the elements inside the detectionradius
      }
     }
    if(numberOfNeighbour > 0) {
    *steering = *steering  / (float)numberOfNeighbour;  //devide the sum steeringvector by the number of elements -> cohesion steeringvector
    }
    return *steering;
  }
 
};



//End of My Flocking Class
