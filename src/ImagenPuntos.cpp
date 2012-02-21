#include "ImagenPuntos.h"
#include <stdio.h>

//--------------------------------------------------------------
void ImagenPuntos::setup(){
  ofSetFrameRate(30);

  guiSetup();
  guiMidiSetup();

  // Carga la imagen de fondo
  background.loadImage("prueba2.jpg");

  // Carga las demás imágenes
  directory.listDir("imagenes/");
  directory.sort();

  if (directory.size()) {
    images.assign(directory.size(), ofImage());
  }

  for(int i = 0; i < (int)directory.size(); i++){
    images[i].setUseTexture(false);
    images[i].loadImage(directory.getPath(i));
  }

  imageNumber = 0;
  particleController = ParticleController(ofGetWindowWidth(), ofGetWindowHeight(), 5, 30, 7);
}
//--------------------------------------------------------------
void ImagenPuntos::guiSetup() {
  vector<string> shapes;
  shapes.push_back("Círculos");
  shapes.push_back("Cuadrados");

  gui = new ofxUICanvas(0, 0, 220, 320);
  gui->setColorBack(ofColor(0, 150));
  gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_SMALL));
  gui->addWidgetDown(new ofxUISpacer(200, 2));

  sliderRadius         = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, RADIO));
  sliderResolution     = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, RESOLUCION));
  sliderRandomRadius   = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, RADIO_ALEATORIO));
  sliderRandomPosition = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, POSICION_ALEATORIA));

  radioShape = (ofxUIRadio*)gui->addWidgetDown(new ofxUIRadio(8, 8, FORMAS, shapes, OFX_UI_ORIENTATION_HORIZONTAL));
  gui->loadSettings("GUI/guiSettings.xml");
  ofAddListener(gui->newGUIEvent,this,&ImagenPuntos::guiEvent);

  //sliderRadius = (ofxUISlider*)gui->getWidget(RADIO);
  //sliderResolution = (ofxUISlider*)gui->getWidget(RESOLUCION);
  //sliderRandomRadius = (ofxUISlider*)gui->getWidget(RADIO_ALEATORIO);
  //sliderRandomPosition = (ofxUISlider*)gui->getWidget(POSICION_ALEATORIA);

  radiusControl = sliderRadius->getScaledValue();
  resolutionControl = sliderResolution->getScaledValue();
  randomRadiusControl = sliderRandomRadius->getScaledValue();
  randomPositionControl = sliderRandomPosition->getScaledValue();
  shapeControl = ((ofxUIRadio*)gui->getWidget(FORMAS))->getToggles()[0]->getValue() == true ? Circle : Square;
}
//--------------------------------------------------------------
void ImagenPuntos::guiMidiSetup() {
  midiIn.addListener(this);
  midiIn.setVerbose(true);
  portList = midiIn.getPortList();

  guiMidi = new ofxUICanvas(ofGetWidth() - 220, 0, 220, 400);
  guiMidi->setColorBack(ofColor(0, 150));
  ofxUIDropDownList* puertosMidi =
    (ofxUIDropDownList*)guiMidi->addWidgetDown(new ofxUIDropDownList(200, PUERTOS_MIDI, portList, OFX_UI_FONT_SMALL));

  labelCanalMidi     = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(CANAL_MIDI, OFX_UI_FONT_SMALL));
  labelPitchMidi     = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(PITCH_MIDI, OFX_UI_FONT_SMALL));
  labelVelocidadMidi = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(VELOCIDAD_MIDI, OFX_UI_FONT_SMALL));
  labelControlMidi   = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(CONTROL_MIDI, OFX_UI_FONT_SMALL));
  labelValorMidi     = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(VALOR_MIDI, OFX_UI_FONT_SMALL));

  guiMidi->addWidgetDown(new ofxUISpacer(200, 2));
  guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RADIUS, "Control de radio", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RESOLUTION, "Control de resolución", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RANDOM_RADIUS, "Control de radio aleatorio", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RANDOM_POSITION, "Control de posición aleatoria", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_FORMA, "Control de forma", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, PITCH_FORMA, "Pitch de forma", OFX_UI_FONT_SMALL));
  guiMidi->loadSettings("GUI/guiMidiSettings.xml");
  ofAddListener(guiMidi->newGUIEvent,this,&ImagenPuntos::guiEvent);

  //labelCanalMidi = (ofxUILabel*)guiMidi->getWidget(CANAL_MIDI);
  //labelPitchMidi = (ofxUILabel*)guiMidi->getWidget(PITCH_MIDI);
  //labelVelocidadMidi = (ofxUILabel*)guiMidi->getWidget(VELOCIDAD_MIDI);
  //labelControlMidi = (ofxUILabel*)guiMidi->getWidget(CONTROL_MIDI);
  //labelValorMidi = (ofxUILabel*)guiMidi->getWidget(VALOR_MIDI);

  // Abre el último puerto midi seleccionado
  vector<ofxUILabelToggle *> midiToggles = puertosMidi->getToggles();
  for (int i = 0; i < midiToggles.size(); i++) {
    if (midiToggles[i]->getValue()) {
      string portName = midiToggles[i]->getLabel()->getLabel();
      midiIn.openPort(portName);
      break;
    }
  }

  radiusControlId = ofToInt(((ofxUITextInput*)guiMidi->getWidget(CONTROL_RADIUS))->getTextString());
  resolutionControlId = ofToInt(((ofxUITextInput*)guiMidi->getWidget(CONTROL_RESOLUTION))->getTextString());
  randomRadiusControlId = ofToInt(((ofxUITextInput*)guiMidi->getWidget(CONTROL_RANDOM_RADIUS))->getTextString());
  randomPositionControlId = ofToInt(((ofxUITextInput*)guiMidi->getWidget(CONTROL_RANDOM_POSITION))->getTextString());
  shapeControlId = ofToInt(((ofxUITextInput*)guiMidi->getWidget(CONTROL_FORMA))->getTextString());
  shapeControlPitch = ofToInt(((ofxUITextInput*)guiMidi->getWidget(PITCH_FORMA))->getTextString());
}
//--------------------------------------------------------------
void ImagenPuntos::update(){
  particleController.setRadius(radiusControl);
  particleController.setResolution(resolutionControl);
  particleController.setRandomRadius(randomRadiusControl);
  particleController.setRandomPosition(randomPositionControl);
  particleController.setShape(shapeControl);
  particleController.update(images[imageNumber]);
}

//--------------------------------------------------------------
void ImagenPuntos::draw(){
  ofBackground(0);
  ofSetColor(255);
  background.draw(0, 0);
  if (images.size() > 0) {
    particleController.draw();
  }
}

//--------------------------------------------------------------
void ImagenPuntos::guiEvent(ofxUIEventArgs &event) {
  string name = event.widget->getName();
  if (name == RADIO) {
    ofxUISlider *slider = (ofxUISlider *)event.widget;
    radiusControl = slider->getScaledValue();
  }
  else if (name == RESOLUCION) {
    ofxUISlider *slider = (ofxUISlider *)event.widget;
    resolutionControl = slider->getScaledValue();
  }
  else if (name == RADIO_ALEATORIO) {
    ofxUISlider *slider = (ofxUISlider *)event.widget;
    randomRadiusControl = slider->getScaledValue();
  }
  else if (name == POSICION_ALEATORIA) {
    ofxUISlider *slider = (ofxUISlider *)event.widget;
    randomPositionControl = slider->getScaledValue();
  }
  else if (name == "Círculos") {
    shapeControl = Circle;
  }
  else if (name == "Cuadrados") {
    shapeControl = Square;
  }
  else if (name == CONTROL_RADIUS) {
    ofxUITextInput *text = (ofxUITextInput*)event.widget;
    radiusControlId = ofToInt(text->getTextString());
  }
  else if (name == CONTROL_RESOLUTION) {
    ofxUITextInput *text = (ofxUITextInput*)event.widget;
    resolutionControlId = ofToInt(text->getTextString());
  }
  else if (name == CONTROL_RANDOM_RADIUS) {
    ofxUITextInput *text = (ofxUITextInput*)event.widget;
    randomRadiusControlId = ofToInt(text->getTextString());
  }
  else if (name == CONTROL_RANDOM_POSITION) {
    ofxUITextInput *text = (ofxUITextInput*)event.widget;
    randomPositionControlId = ofToInt(text->getTextString());
  }
  else {
    for (int i = 0; i < portList.size(); ++i) {
      if (portList[i] == name) {
        midiIn.openPort(i);
        break;
      }
    }
  }
}

//--------------------------------------------------------------
void ImagenPuntos::newMidiMessage(ofxMidiMessage& msg) {
  labelCanalMidi->setLabel("Canal: " + ofToString(msg.channel));
  labelPitchMidi->setLabel("Pitch: " + ofToString(msg.pitch));
  labelVelocidadMidi->setLabel("Velocidad: " + ofToString(msg.velocity));
  labelControlMidi->setLabel("Control: " + ofToString(msg.control));
  labelValorMidi->setLabel("Valor: " + ofToString(msg.value));

  if (msg.control == radiusControlId) {
    radiusControl = ofMap(msg.value, 0, 127, 0, 1);
    sliderRadius->setValue(radiusControl);
  }
  else if (msg.control == resolutionControlId) {
    resolutionControl = ofMap(msg.value, 0, 127, 0, 1);
    sliderResolution->setValue(resolutionControl);
  }
  else if (msg.control == randomRadiusControlId) {
    randomRadiusControl = ofMap(msg.value, 0, 127, 0, 1);
    sliderRandomRadius->setValue(randomRadiusControl);
  }
  else if (msg.control == randomPositionControlId) {
    randomPositionControl = ofMap(msg.value, 0, 127, 0, 1);
    sliderRandomPosition->setValue(randomPositionControl);
  }
  else if (msg.control == shapeControlId && msg.value > 0) {
    shapeControl = shapeControl == Circle ? Square : Circle;
    //radioShape->activateToggle(shapeControl);
  }
  else if (msg.pitch == shapeControlPitch && msg.velocity > 0) {
    shapeControl = shapeControl == Circle ? Square : Circle;
    //radioShape->activateToggle(shapeControl);
  }
}

//--------------------------------------------------------------
void ImagenPuntos::keyPressed(int key){
  switch (key) {
    case 'g': {
      gui->toggleVisible();
      guiMidi->toggleVisible();
    }
  }
}

//--------------------------------------------------------------
void ImagenPuntos::keyReleased(int key){

}

//--------------------------------------------------------------
void ImagenPuntos::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ImagenPuntos::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ImagenPuntos::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ImagenPuntos::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ImagenPuntos::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ImagenPuntos::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ImagenPuntos::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void ImagenPuntos::exit() {
  gui->saveSettings("GUI/guiSettings.xml");
  guiMidi->saveSettings("GUI/guiMidiSettings.xml");
  delete gui;
  delete guiMidi;
}
