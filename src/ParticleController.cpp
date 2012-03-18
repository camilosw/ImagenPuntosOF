#include "ParticleController.h"
#include <math.h>

ParticleController::ParticleController() {
  //ctor
}

ParticleController::ParticleController(int width, int height, int minResolution, int maxResolution, float maxRadius) {
  this->width = width;
  this->height = height;
  this->minResolution = minResolution;
  this->maxResolution = maxResolution;
  this->maxRadius = maxRadius;
  this->radius = 1;
  int xParticles = width / minResolution;
  int yParticles = height / minResolution;

  for (int y = 0; y < yParticles; y++) {
    for (int x = 0; x < xParticles; x++) {
      float xPos = (x + 0.5f) * minResolution;
      float yPos = (y + 0.5f) * minResolution;
        particles.push_back(Particle(ofVec2f(xPos, yPos), minResolution, x, y));
    }
  }
}

void ParticleController::setResolution(float value) {
  resolution = ofMap(value, 0.0, 1.0, minResolution, maxResolution);

  for(list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
    // Modifica la posición de la partícula según la resolución;
    float xPos = (p->getX() + 0.5f) * resolution;
    float yPos = (p->getY() + 0.5f) * resolution;
    p->setLocation(ofVec2f(xPos, yPos));
  }
}

void ParticleController::update(ofPixels image) {
  float offset = ceil(ofMap(position, 0, 1, -width, width));
  for(list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
    // Determina si la partícula es visible. Si no lo es, continúa con la siguiente
    if ( (p->getLocation().y) > height ||
         (p->getLocation().y) > image.getHeight() ||
         (p->getLocation().x + offset - resolution) > width ||
         (p->getLocation().x + offset) < 0) {
      continue;
    }

    // Obtiene el color de un pixel de la imagen
    //ofVec2f location = p->getLocation();
    ofColor color = image.getColor(p->getLocation().x + offset, p->getLocation().y);
    p->setColor(color);
    p->setRadius(radius * maxRadius + ofRandom(-1, 1) * randomRadius * radius * maxRadius);
    ofVec2f randomVec2f = ofVec2f(ofRandom(-randomPosition * resolution, randomPosition * resolution),
                                  ofRandom(-randomPosition * resolution, randomPosition * resolution));
    p->setLocation(p->getLocation() + randomVec2f);
    p->setShape(shape);
  }
}

void ParticleController::draw() {
  ofPushStyle();
  float offset = ofMap(position, 0, 1, -width, width);
  for(list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
    // Determina si la partícula es visible. Si no lo es, continúa con la siguiente
    if ( (p->getLocation().y) > height ||
         (p->getLocation().y) > width ||
         (p->getLocation().x + offset - resolution) > width ||
         (p->getLocation().x + offset) < 0) {
      continue;
    }
    p->draw();
  }
  ofPopStyle();
}
