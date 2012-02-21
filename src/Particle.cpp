#include "Particle.h"
#include "stdio.h"

Particle::Particle(ofVec2f location) {
  this->location = location;
}

Particle::Particle(ofVec2f location, int radius, int x, int y) {
  this->location = location;
  this->radius = radius;
  this->shape = Circle;
  this->x = x;
  this->y = y;
}

void Particle::draw() {
  ofSetColor(color);

  if (shape == Square) {
    float x = location.x - radius;
    float y = location.y - radius;
    float width = radius * 2;
    float height = radius * 2;
    ofRect(x, y, width, height);
  }
  else {
    ofSetCircleResolution(10);
    ofCircle(location.x, location.y, radius);
  }
}

Particle::~Particle() {
    //dtor
}
