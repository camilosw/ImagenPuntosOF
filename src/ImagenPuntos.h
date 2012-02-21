#pragma once

#include "ofMain.h"
#include "ParticleController.h"
#include "ofxUI.h"
#include "ofxMidi.h"

#define RADIO "Radio"
#define RESOLUCION "Resolución"
#define RADIO_ALEATORIO "Radio aleatorio"
#define POSICION_ALEATORIA "Posición aleatoria"
#define FORMAS "Formas"

#define PUERTOS_MIDI "Puertos midi"
#define CANAL_MIDI "Canal midi"
#define PITCH_MIDI "Pitch midi"
#define VELOCIDAD_MIDI "Velocidad midi"
#define CONTROL_MIDI "Control midi"
#define VALOR_MIDI "Valor midi"
#define CONTROL_RADIUS "Control radio"
#define CONTROL_RESOLUTION "Control resolución"
#define CONTROL_RANDOM_RADIUS "Control radio aleatorio"
#define CONTROL_RANDOM_POSITION "Control posición aleatoria"
#define CONTROL_FORMA "Control de forma"
#define PITCH_FORMA "Control de forma"

class ImagenPuntos : public ofBaseApp, public ofxMidiListener {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();
		void guiEvent(ofxUIEventArgs &event);
		void newMidiMessage(ofxMidiMessage& eventArgs);

  private:
    void guiSetup();
    void guiMidiSetup();

    ParticleController particleController;
    ofDirectory directory;
    vector<ofImage> images;
    ofImage background;

    // Atributos de la gui
    ofxUICanvas* gui;
    ofxUICanvas* guiMidi;
    ofxUISlider* sliderRadius;
    ofxUISlider* sliderResolution;
    ofxUISlider* sliderRandomRadius;
    ofxUISlider* sliderRandomPosition;
    ofxUIRadio* radioShape;

    // Atributos de la interfaz midi
    ofxMidiIn	midiIn;
    ofxMidiMessage midiMessage;
    vector<string> portList;	// Listado de puertos midi
    ofxUILabel* labelCanalMidi;
    ofxUILabel* labelPitchMidi;
    ofxUILabel* labelVelocidadMidi;
    ofxUILabel* labelControlMidi;
    ofxUILabel* labelValorMidi;

    int imageNumber;
    float radiusControl;
    float resolutionControl;
    float randomRadiusControl;
    float randomPositionControl;
    Shapes shapeControl;
    int radiusControlId;
    int resolutionControlId;
    int randomRadiusControlId;
    int randomPositionControlId;
    int shapeControlId;
    int shapeControlPitch;
};
