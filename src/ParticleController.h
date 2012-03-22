#ifndef PARTICLECONTROLLER_H
#define PARTICLECONTROLLER_H

#include "Particle.h"
#include <list>

class ParticleController
{
  public:
  ParticleController();
  ParticleController(int width, int height, int minResolution, int maxResolution, float maxRadius);
  void update(ofPixels pixels);
  void draw();

  void setResolution(float value);
  void setRadius(float value) { radius = value; }
  void setPosition(float value) { position = value; }
  void setShape(Shapes value) { shape = value; }
  void setRandomRadius(float value) { randomRadius = value; }
  void setRandomPosition(float value) { randomPosition = value; }
  void setAlpha(float value) { alpha = value; }

  Shapes getShape() { return shape; }
  float getRandomRadius() { return randomRadius; }
  float getRandomPosition() { return randomPosition; }

  protected:
  private:
    std::list<Particle> particles;
    int width;
    int height;

    int minResolution;        // Distancia entre las partículas
    int maxResolution;
    int resolution;
    float maxRadius;          // Máximo radio de las partículas
    float radius;             // Radio de las partículas
    float position;           // Posición en x de la imagen
    Shapes shape;             // Forma de las partículas (círculo o cuadrado)
    float randomRadius;       // Determina la magnitud de cambio aleatorio del radio
    float randomPosition;     // Determina la magnitud de cambio aleatorio de la posición
    float alpha;               // Determina el alpha de los puntos
};

#endif // PARTICLECONTROLLER_H
