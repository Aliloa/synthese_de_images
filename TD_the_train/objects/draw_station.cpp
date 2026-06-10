#include "draw_station.hpp"
#include <iostream>
#include "glbasimac/glbi_texture.hpp"
#include "tools/stb_image.h"

static IndexedMesh *murs = nullptr;
static StandardMesh *toit = nullptr;
static IndexedMesh *cheminee = nullptr;
static IndexedMesh *pancarte = nullptr;
static IndexedMesh *poteau = nullptr;
static IndexedMesh *porte = nullptr;

glbasimac::GLBI_Texture textureMur;
glbasimac::GLBI_Texture textureToit;
glbasimac::GLBI_Texture textureBriques;
glbasimac::GLBI_Texture textureBois;
glbasimac::GLBI_Texture texturePorte;

void initStation()
{
  murs = basicCube(1.5f);
  toit = basicCone(0.8f, 1.0f);
  cheminee = basicCube(1.0f);
  pancarte = basicCube(1.0f);
  poteau = basicCylinder(1.0f, 1.0f);
  porte = basicCube(1.0f);

  murs->createVAO();
  toit->createVAO();
  cheminee->createVAO();
  pancarte->createVAO();
  poteau->createVAO();
  porte->createVAO();

  // mur
  textureMur.createTexture();
  int w1, h1, n1;
  unsigned char *pixelsMur = stbi_load("../assets/textures/mur.png", &w1, &h1, &n1, 0);
  textureMur.attachTexture();
  textureMur.loadImage(w1, h1, n1, pixelsMur);
  textureMur.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureMur.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureMur.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureMur.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureMur.detachTexture();
  stbi_image_free(pixelsMur);

  // toit
  textureToit.createTexture();
  int w2, h2, n2;
  unsigned char *pixelsToit = stbi_load("../assets/textures/toit.png", &w2, &h2, &n2, 0);
  textureToit.attachTexture();
  textureToit.loadImage(w2, h2, n2, pixelsToit);
  textureToit.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureToit.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureToit.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureToit.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureToit.detachTexture();
  stbi_image_free(pixelsToit);

  // briques
  textureBriques.createTexture();
  int w3, h3, n3;
  unsigned char *pixelsBriques = stbi_load("../assets/textures/briques.png", &w3, &h3, &n3, 0);
  textureBriques.attachTexture();
  textureBriques.loadImage(w3, h3, n3, pixelsBriques);
  textureBriques.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureBriques.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureBriques.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureBriques.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureBriques.detachTexture();
  stbi_image_free(pixelsBriques);

  // bois basique
  textureBois.createTexture();
  int w4, h4, n4;
  unsigned char *pixelsBois = stbi_load("../assets/textures/bois_basique.png", &w4, &h4, &n4, 0);
  textureBois.attachTexture();
  textureBois.loadImage(w4, h4, n4, pixelsBois);
  textureBois.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureBois.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureBois.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureBois.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureBois.detachTexture();
  stbi_image_free(pixelsBois);

  // porte
  texturePorte.createTexture();
  int w5, h5, n5;
  unsigned char *pixelsPorte = stbi_load("../assets/textures/porte.png", &w5, &h5, &n5, 0);
  texturePorte.attachTexture();
  texturePorte.loadImage(w5, h5, n5, pixelsPorte);
  texturePorte.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texturePorte.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texturePorte.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texturePorte.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texturePorte.detachTexture();
  stbi_image_free(pixelsPorte);
}

void drawStation()
{
  const float CASE_SIZE = 10.0f;
  const int N = 10;
  float baseX = (circuit.station.first - N / 2) * CASE_SIZE;
  float baseY = (circuit.station.second - N / 2) * CASE_SIZE;

  myEngine.mvMatrixStack.pushMatrix();
  myEngine.mvMatrixStack.addTranslation({baseX, baseY + 10.f, 0.f});

  // Bâtiment principal
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.55f, 0.35f, 0.15f);
  myEngine.activateTexturing(true);
  textureMur.attachTexture();
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 0.3f});
  myEngine.mvMatrixStack.addHomothety({7.f, 5.f, 7.f});
  myEngine.updateMvMatrix();
  murs->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureMur.detachTexture();
  myEngine.activateTexturing(false);

  // Toit chalet
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.activateTexturing(true);
  textureToit.attachTexture();
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 5.3f});
  myEngine.mvMatrixStack.addRotation(-M_PI / 2, {-1, 0, 0});
  myEngine.mvMatrixStack.addHomothety({9.f, 6.f, 6.f});
  myEngine.updateMvMatrix();
  toit->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureToit.detachTexture();
  myEngine.activateTexturing(false);

  // Auvent avant
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.activateTexturing(true);
  textureBois.attachTexture();
  myEngine.mvMatrixStack.addTranslation({10.5f, 5.0f, 4.5f});
  myEngine.mvMatrixStack.addHomothety({2.0f, 5.0f, 0.2f});
  myEngine.updateMvMatrix();
  porte->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureBois.detachTexture();
  myEngine.activateTexturing(false);

  // Cheminée
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.4f, 0.25f, 0.1f);
  myEngine.activateTexturing(true);
  textureBriques.attachTexture();
  myEngine.mvMatrixStack.addTranslation({5.0f, 4.0f, 9.0f});
  myEngine.mvMatrixStack.addHomothety({0.5f, 0.5f, 2.0f});
  myEngine.updateMvMatrix();
  cheminee->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureBriques.detachTexture();
  myEngine.activateTexturing(false);

  // Poteau pancarte
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.4f, 0.25f, 0.1f);
  myEngine.activateTexturing(true);
  textureBois.attachTexture();
  myEngine.mvMatrixStack.addTranslation({10.5f, 11.f, 1.5f});
  myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});
  myEngine.mvMatrixStack.addHomothety({0.2f, 0.2f, 3.f});
  myEngine.updateMvMatrix();
  poteau->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureBois.detachTexture();
  myEngine.activateTexturing(false);

  // Pancarte
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.95f, 0.90f, 0.6f);
   myEngine.activateTexturing(true);
  textureBois.attachTexture();
  myEngine.mvMatrixStack.addTranslation({10.5f, 11.f, 4.f});
  myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});
  myEngine.mvMatrixStack.addHomothety({3.f, 0.15f, 1.2f});
  myEngine.updateMvMatrix();
  pancarte->draw();
  myEngine.mvMatrixStack.popMatrix();
    textureBois.detachTexture();
  myEngine.activateTexturing(false);


  // Porte
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.activateTexturing(true);
  texturePorte.attachTexture();
  myEngine.mvMatrixStack.addTranslation({10.5f, 5.0f, 1.5f});
  myEngine.mvMatrixStack.addHomothety({0.1f, 2.0f, 3.5f});
  myEngine.updateMvMatrix();
  porte->draw();
  myEngine.mvMatrixStack.popMatrix();
  texturePorte.detachTexture();
  myEngine.activateTexturing(false);

  myEngine.mvMatrixStack.popMatrix();
  myEngine.updateMvMatrix();
}
