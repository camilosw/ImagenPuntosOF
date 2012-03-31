#include "ImagenPuntos.h"
#include <stdio.h>

//--------------------------------------------------------------
void ImagenPuntos::setup(){  
  ofSetFrameRate(30);

  guiSetup();
  guiMidiSetup();
  gui->setVisible(false);
  guiMidi->setVisible(false);
  
  // Carga la imagen de fondo
  background.loadImage("cielo.jpg");


  // Carga las demás imágenes
  directory.listDir("imagenes/");
  directory.sort();

  if (directory.size()) {
    images.assign(directory.size(), ofImage());
  }

  for(int i = 0; i < (int)directory.size(); i++){
    //images[i].setUseTexture(false);
    images[i].loadImage(directory.getPath(i));
  }

  // Carga el video
  video.loadMovie("videos/negedit.mov");
  //video.play();

  imageNumber = 0;
  particleController = ParticleController(ofGetWindowWidth(), ofGetWindowHeight(), 5, 30, 7);
}
//--------------------------------------------------------------
void ImagenPuntos::guiSetup() {
  vector<string> shapes;
  shapes.push_back("Círculos");
  shapes.push_back("Cuadrados");

  gui = new ofxUICanvas(0, 0, 220, 400);
  gui->setColorBack(ofColor(0, 150));
  gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_SMALL));
  gui->addWidgetDown(new ofxUISpacer(200, 2));

  sliderRadius          = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, RADIO));
  sliderResolution      = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, RESOLUCION));
  sliderRandomRadius    = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, RADIO_ALEATORIO));
  sliderRandomPosition  = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, POSICION_ALEATORIA));
  sliderPosicionImagen  = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, POSICION_IMAGEN));
  sliderTransparencia   = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, TRANSPARENCIA_PUNTOS));
  sliderCrossfadePuntos = (ofxUISlider*)gui->addWidgetDown(new ofxUISlider(200, 8, 0.0, 1.0, 0.5, CROSSFADE_PUNTOS));

  radioShape = (ofxUIRadio*)gui->addWidgetDown(new ofxUIRadio(8, 8, FORMAS, shapes, OFX_UI_ORIENTATION_HORIZONTAL));
  toggleVideo = (ofxUIToggle*)gui->addWidgetDown(new ofxUIToggle(8, 8, false, VIDEO));
  toggleOcultarFondo  = (ofxUIToggle*)gui->addWidgetDown(new ofxUIToggle(8, 8, false, OCULTAR_FONDO));
  gui->addWidgetDown(new ofxUILabelButton(false, BOTON_IMAGEN_ANTERIOR, OFX_UI_FONT_SMALL));
  gui->addWidgetEastOf(new ofxUILabelButton(false, BOTON_IMAGEN_SIGUIENTE, OFX_UI_FONT_SMALL), BOTON_IMAGEN_ANTERIOR);
  gui->loadSettings("GUI/guiSettings.xml");
  ofAddListener(gui->newGUIEvent,this,&ImagenPuntos::guiEvent);

  shapeControl = radioShape->getToggles()[0]->getValue() == true ? Circle : Square;
}
//--------------------------------------------------------------
void ImagenPuntos::guiMidiSetup() {
  midiIn.addListener(this);
  midiIn.setVerbose(true);
  portList = midiIn.getPortList();

  guiMidi = new ofxUICanvas(ofGetWidth() - 220, 0, 220, 500);
  guiMidi->setColorBack(ofColor(0, 150));
  ofxUIDropDownList* puertosMidi =
    (ofxUIDropDownList*)guiMidi->addWidgetDown(new ofxUIDropDownList(200, PUERTOS_MIDI, portList, OFX_UI_FONT_SMALL));
  puertosMidi->setAutoClose(true);

  labelCanalMidi     = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(CANAL_MIDI, OFX_UI_FONT_SMALL));
  labelPitchMidi     = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(PITCH_MIDI, OFX_UI_FONT_SMALL));
  labelVelocidadMidi = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(VELOCIDAD_MIDI, OFX_UI_FONT_SMALL));
  labelControlMidi   = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(CONTROL_MIDI, OFX_UI_FONT_SMALL));
  labelValorMidi     = (ofxUILabel*)guiMidi->addWidgetDown(new ofxUILabel(VALOR_MIDI, OFX_UI_FONT_SMALL));

  guiMidi->addWidgetDown(new ofxUISpacer(200, 2));
  controlRadius = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RADIUS, "Control de radio", OFX_UI_FONT_SMALL));
  controlResolution = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RESOLUTION, "Control de resolución", OFX_UI_FONT_SMALL));
  controlRandomRadius = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RANDOM_RADIUS, "Control de radio aleatorio", OFX_UI_FONT_SMALL));
  controlRandomPosition = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_RANDOM_POSITION, "Control de posición aleatoria", OFX_UI_FONT_SMALL));
  controlPosicionImagen = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_POSITION, "Control de posición", OFX_UI_FONT_SMALL));
  controlTransparencia = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_TRANSPARENCIA, "Control de transparencia", OFX_UI_FONT_SMALL));
  controlCrossfadePuntos = 
    (ofxUITextInput*)guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_CROSSFADE, "Control crossfade", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, CONTROL_FORMA, "Control de forma", OFX_UI_FONT_SMALL));
  guiMidi->addWidgetDown(new ofxUITextInput(200, PITCH_FORMA, "Pitch de forma", OFX_UI_FONT_SMALL));
  guiMidi->loadSettings("GUI/guiMidiSettings.xml");
  ofAddListener(guiMidi->newGUIEvent,this,&ImagenPuntos::guiEvent);

  // Abre el último puerto midi seleccionado
  vector<ofxUILabelToggle *> midiToggles = puertosMidi->getToggles();
  for (int i = 0; i < midiToggles.size(); i++) {
    if (midiToggles[i]->getValue()) {
      string portName = midiToggles[i]->getLabel()->getLabel();
      midiIn.openPort(portName);
      break;
    }
  }

  shapeControlId          = ofToInt(((ofxUITextInput*)guiMidi->getWidget(CONTROL_FORMA))->getTextString());
  shapeControlPitch       = ofToInt(((ofxUITextInput*)guiMidi->getWidget(PITCH_FORMA))->getTextString());
}
//--------------------------------------------------------------
void ImagenPuntos::update(){
  particleController.setRadius(sliderRadius->getScaledValue());
  particleController.setResolution(sliderResolution->getScaledValue());
  particleController.setRandomRadius(sliderRandomRadius->getScaledValue());
  particleController.setRandomPosition(sliderRandomPosition->getScaledValue());
  particleController.setPosition(sliderPosicionImagen->getScaledValue());
  particleController.setShape(shapeControl);
  particleController.setAlpha(sliderTransparencia->getScaledValue());
  particleController.setCrossfade(sliderCrossfadePuntos->getScaledValue());
  if (toggleVideo->getValue()) {
    video.idleMovie();
    particleController.update(video.getPixelsRef(), &video.getTextureReference());
  }
  else {
    particleController.update(images[imageNumber].getPixelsRef(), &images[imageNumber].getTextureReference());
  }
}

//--------------------------------------------------------------
void ImagenPuntos::draw(){
  ofBackground(0);
  ofSetColor(255);
  if (!toggleOcultarFondo->getValue()) {
    background.draw(0, 0);
  }  
  if (images.size() > 0) {
    particleController.draw();
  }
}

//--------------------------------------------------------------
void ImagenPuntos::guiEvent(ofxUIEventArgs &event) {
  string name = event.widget->getName();
  if (name == "Círculos") {
    shapeControl = Circle;
  }
  else if (name == "Cuadrados") {
    shapeControl = Square;
  }
  else if (name == BOTON_IMAGEN_ANTERIOR && ((ofxUIButton*)event.widget)->getValue()) {
    if (imageNumber > 0) imageNumber--;
  }
  else if (name == BOTON_IMAGEN_SIGUIENTE && ((ofxUIButton*)event.widget)->getValue()) {
    if (imageNumber < images.size() - 1) imageNumber++;
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

  if (msg.control == ofToInt(controlRadius->getTextString())) {
    sliderRadius->setValue(ofMap(msg.value, 0, 127, 0, 1));
  }
  else if (msg.control == ofToInt(controlResolution->getTextString())) {
    sliderResolution->setValue(ofMap(msg.value, 0, 127, 0, 1));
  }
  else if (msg.control == ofToInt(controlRandomRadius->getTextString())) {
    sliderRandomRadius->setValue(ofMap(msg.value, 0, 127, 0, 1));
  }
  else if (msg.control == ofToInt(controlRandomPosition->getTextString())) {
    sliderRandomPosition->setValue(ofMap(msg.value, 0, 127, 0, 1));
  }
  else if (msg.control == ofToInt(controlPosicionImagen->getTextString())) {
    sliderPosicionImagen->setValue(ofMap(msg.value, 0, 127, 0, 1));
  }
  else if (msg.control == ofToInt(controlTransparencia->getTextString())) {
    sliderTransparencia->setValue(ofMap(msg.value, 0, 127, 0, 1));
  }
  else if (msg.control == ofToInt(controlCrossfadePuntos->getTextString())) {
    sliderCrossfadePuntos->setValue(ofMap(msg.value, 0, 127, 0, 1));
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
  cout << key << "\n";
  switch (key) {
    case 'g': {
      gui->toggleVisible();
      guiMidi->toggleVisible();
      break;
    }
    case 'v': {
      toggleVideo->toggleValue();
      break;
    }
    case 'b': {
      toggleOcultarFondo->toggleValue();
      break;
    }
    case 's': {
      shapeControl = shapeControl == Circle ? Square : Circle;
      break;
    }
    case 356: { // Flecha izquierda
      if (imageNumber > 0) imageNumber--;
      break;
    }
    case 358: { // Flecha derecha
      if (imageNumber < images.size() - 1) imageNumber++;
      break;
    }
    case 357: { // Flecha arriba
      video.play();
      break;
    }
    case 359: { // Flecha abajo
      video.stop();
      break;
    }
    case '0': {
      sliderRadius->setValue(0);
      sliderResolution->setValue(0);
      sliderRandomRadius->setValue(0);
      sliderRandomPosition->setValue(0);
      sliderPosicionImagen->setValue(0.5);
      sliderTransparencia->setValue(0);
      sliderCrossfadePuntos->setValue(0);
      toggleVideo->setValue(false);    
      toggleOcultarFondo->setValue(true);
      shapeControl = Circle;
      video.setPosition(0);
      video.stop();
      imageNumber = 0;      
      break;
    }
    case '1': {
      sliderRadius->setValue(1);
      sliderResolution->setValue(1);
      sliderRandomRadius->setValue(0);
      sliderRandomPosition->setValue(0);
      sliderPosicionImagen->setValue(0.5);
      sliderTransparencia->setValue(1);
      sliderCrossfadePuntos->setValue(1);
      toggleVideo->setValue(true);    
      toggleOcultarFondo->setValue(true);
      shapeControl = Circle;
      video.setPosition(0);
      video.play();
      imageNumber = 0;      
      break;
    }
    case '2': {
      sliderRadius->setValue(0);
      sliderResolution->setValue(0);
      sliderRandomRadius->setValue(0);
      sliderRandomPosition->setValue(0);
      sliderPosicionImagen->setValue(0.5);
      sliderTransparencia->setValue(1);
      sliderCrossfadePuntos->setValue(0);
      toggleVideo->setValue(false);
      toggleOcultarFondo->setValue(true);
      shapeControl = Circle;
      video.setPosition(0);
      video.stop();
      imageNumber = 0;      

      break;
    }    
    case '3': {
      sliderRadius->setValue(0);
      sliderResolution->setValue(0);
      sliderRandomRadius->setValue(0);
      sliderRandomPosition->setValue(0);
      sliderPosicionImagen->setValue(0.5);
      sliderTransparencia->setValue(0);
      sliderCrossfadePuntos->setValue(0);
      toggleVideo->setValue(false);    
      toggleOcultarFondo->setValue(false);
      shapeControl = Circle;
      video.setPosition(0);
      video.stop();
      imageNumber = 0;      
      break;
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
