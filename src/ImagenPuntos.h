#pragma once

#include "ofMain.h"
#include "ParticleController.h"
#include "ofxUI.h"
#include "ofxMidi.h"

#define RADIO "Radio"
#define RESOLUCION "Resoluci�n"
#define RADIO_ALEATORIO "Radio aleatorio"
#define POSICION_ALEATORIA "Posici�n aleatoria"
#define POSICION_IMAGEN "Posici�n imagen"
#define TRANSPARENCIA_PUNTOS "Transparencia puntos"
#define CROSSFADE_PUNTOS "Crossfade puntos"
#define VELOCIDAD_IMAGEN "Velocidad de movimiento de la imagen"
#define FORMAS "Formas"
#define VIDEO "Video (v)"
#define OCULTAR_FONDO "Ocultar fondo (b)"
#define BOTON_IMAGEN_ANTERIOR "<-"
#define BOTON_IMAGEN_SIGUIENTE "->"

#define PUERTOS_MIDI "Puertos midi"
#define CANAL_MIDI "Canal midi"
#define PITCH_MIDI "Pitch midi"
#define VELOCIDAD_MIDI "Velocidad midi"
#define CONTROL_MIDI "Control midi"
#define VALOR_MIDI "Valor midi"
#define CONTROL_RADIUS "Control radio"
#define CONTROL_RESOLUTION "Control resoluci�n"
#define CONTROL_RANDOM_RADIUS "Control radio aleatorio"
#define CONTROL_RANDOM_POSITION "Control posici�n aleatoria"
#define CONTROL_POSITION "Control posici�n"
#define CONTROL_TRANSPARENCIA "Control Transparencia"
#define CONTROL_CROSSFADE "Control Crossfade"
#define CONTROL_VELOCIDAD "Control velocidad"

#define CONTROL_FORMA "Control de forma"
#define PITCH_FORMA "Pitch de forma"
#define CONTROL_VIDEO "Control de video"
#define PITCH_VIDEO "Pitch de video"

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
    ofVideoPlayer video;

    // Atributos de la gui
    ofxUICanvas* gui;
    ofxUICanvas* guiMidi;
    ofxUISlider* sliderRadius;
    ofxUISlider* sliderResolution;
    ofxUISlider* sliderRandomRadius;
    ofxUISlider* sliderRandomPosition;
    ofxUISlider* sliderPosicionImagen;
    ofxUISlider* sliderTransparencia;
    ofxUISlider* sliderCrossfadePuntos;
    ofxUISlider* sliderVelocidad;
    ofxUIRadio* radioShape;
    ofxUIToggle* toggleVideo;
    ofxUIToggle* toggleOcultarFondo;

    ofxUITextInput* controlRadius;
    ofxUITextInput* controlResolution;
    ofxUITextInput* controlRandomRadius;
    ofxUITextInput* controlRandomPosition;
    ofxUITextInput* controlPosicionImagen;
    ofxUITextInput* controlTransparencia;
    ofxUITextInput* controlCrossfadePuntos;
    ofxUITextInput* controlVelocidad;

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
    Shapes shapeControl;
    int shapeControlId;
    int shapeControlPitch;
    int videoControlId;
    int videoControlPitch;

    float posicionMovimiento;
};
