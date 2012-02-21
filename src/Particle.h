#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

#define CIRCLE 0
#define SQUARE 1

enum Shapes { Circle, Square };

class Particle
{
  public:
    Particle(ofVec2f location);
    Particle(ofVec2f location, int radius, int x, int y);
    virtual ~Particle();
    void draw();

    void setLocation(ofVec2f value) { location = value; }
    void setRadius(float value) { radius = value; }
    void setColor(ofColor value) { color = value; }
    void setShape(Shapes value) { shape = value; }

    ofVec2f getLocation() { return location; }
    float getRadius() { return radius; }
    ofColor getColor() { return color; }
    Shapes getShape() { return shape; }
    int getX() { return x; }
    int getY() { return y; }

  protected:
  private:
    int x, y;
    ofVec2f location;
    float radius;
    ofColor color;
    Shapes shape;
};

#endif // PARTICLE_H
