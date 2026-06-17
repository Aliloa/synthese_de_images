#include "draw_train.hpp"
#include <iostream>
#include <cmath>
#include "glbasimac/glbi_texture.hpp"
#include "tools/stb_image.h"

static IndexedMesh *corps = nullptr;
static IndexedMesh *roue = nullptr;
static IndexedMesh *cheminee = nullptr;
static IndexedMesh *wagon = nullptr;

glbasimac::GLBI_Texture textureMetalRouge;
glbasimac::GLBI_Texture textureMetalBleu;
glbasimac::GLBI_Texture textureCheminee;
glbasimac::GLBI_Texture textureRoue;

void initTrain()
{
  corps = basicCube(1.0f);
  roue = basicSphere(1.0f);
  cheminee = basicCylinder(1.0f, 1.0f);
  wagon = basicCube(1.0f);

  corps->createVAO();
  roue->createVAO();
  cheminee->createVAO();
  wagon->createVAO();

  // lumiere
  myEngine.switchToPhongShading();

  // lumière
  // Phare du train
  myEngine.addALight({0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
  myEngine.setLightIntensity({2.0f, 1.8f, 1.0f}, 1);

  // textures
  // wagon rouge
  textureMetalRouge.createTexture();
  int w1, h1, n1;
  unsigned char *pixelsMetalRouge = stbi_load("../assets/textures/red_metal.png", &w1, &h1, &n1, 0);
  textureMetalRouge.attachTexture();
  textureMetalRouge.loadImage(w1, h1, n1, pixelsMetalRouge);
  textureMetalRouge.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureMetalRouge.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureMetalRouge.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureMetalRouge.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureMetalRouge.detachTexture();
  stbi_image_free(pixelsMetalRouge);

  // wagon bleu
  textureMetalBleu.createTexture();
  int w2, h2, n2;
  unsigned char *pixelsMetalBleu = stbi_load("../assets/textures/blue_metal.png", &w2, &h2, &n2, 0);
  textureMetalBleu.attachTexture();
  textureMetalBleu.loadImage(w2, h2, n2, pixelsMetalBleu);
  textureMetalBleu.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureMetalBleu.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureMetalBleu.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureMetalBleu.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureMetalBleu.detachTexture();
  stbi_image_free(pixelsMetalBleu);

  // cheminée
  textureCheminee.createTexture();
  int w3, h3, n3;
  unsigned char *pixelsCheminee = stbi_load("../assets/textures/chemine.png", &w3, &h3, &n3, 0);
  textureCheminee.attachTexture();
  textureCheminee.loadImage(w3, h3, n3, pixelsCheminee);
  textureCheminee.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureCheminee.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureCheminee.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureCheminee.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureCheminee.detachTexture();
  stbi_image_free(pixelsCheminee);

  // wheel
  textureRoue.createTexture();
  int w4, h4, n4;
  unsigned char *pixelsRoue = stbi_load("../assets/textures/wheel.png", &w4, &h4, &n4, 0);
  textureRoue.attachTexture();
  textureRoue.loadImage(w4, h4, n4, pixelsRoue);
  textureRoue.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textureRoue.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  textureRoue.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
  textureRoue.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
  textureRoue.detachTexture();
  stbi_image_free(pixelsRoue);
}

void drawLocomotive()
{
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.activateTexturing(true);
  textureMetalRouge.attachTexture();
  // myEngine.setFlatColor(0.8f, 0.1f, 0.1f);
  myEngine.mvMatrixStack.addTranslation({0.f, 0.f, 2.0f});
  myEngine.mvMatrixStack.addHomothety({8.f, 4.f, 3.f});
  myEngine.updateMvMatrix();
  corps->draw();
  myEngine.mvMatrixStack.popMatrix();

  // carré au dessus locomotive
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.6f, 0.05f, 0.05f);
  myEngine.mvMatrixStack.addTranslation({-1.5f, 0.f, 3.5f});
  myEngine.mvMatrixStack.addHomothety({3.f, 3.5f, 2.f});
  myEngine.updateMvMatrix();
  corps->draw();
  myEngine.mvMatrixStack.popMatrix();

  textureMetalRouge.detachTexture();
  myEngine.activateTexturing(false);

  // Cheminée
  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.2f, 0.2f, 0.2f);
  myEngine.activateTexturing(true);
  textureCheminee.attachTexture();
  myEngine.mvMatrixStack.addTranslation({-1.f, 0.f, 4.5f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({0.6f, 2.5f, 0.6f});
  myEngine.updateMvMatrix();
  cheminee->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureCheminee.detachTexture();
  myEngine.activateTexturing(false);

  // 4 Roues
  // myEngine.setFlatColor(0.15f, 0.15f, 0.15f);
  myEngine.activateTexturing(true);
  textureRoue.attachTexture();
  float roueX[2] = {2.f, -2.f};
  float roueY[2] = {-2.3f, 2.3f};
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      myEngine.mvMatrixStack.pushMatrix();
      myEngine.mvMatrixStack.addTranslation({roueX[i], roueY[j], 0.5f});
      myEngine.mvMatrixStack.addRotation(M_PI, {1, 0, 0});
      myEngine.mvMatrixStack.addHomothety({0.8f, 0.5f, 0.8f});
      myEngine.updateMvMatrix();
      roue->draw();
      myEngine.mvMatrixStack.popMatrix();
    }
  }

  textureRoue.detachTexture();
  myEngine.activateTexturing(false);

  // Phare avant
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(1.f, 1.f, 0.2f);
  myEngine.mvMatrixStack.addTranslation({4.f, 0.f, 2.f});
  // position de la lumière
  myEngine.updateMvMatrix();

  myEngine.mvMatrixStack.addHomothety({0.5f, 0.5f, 0.5f});
  myEngine.updateMvMatrix();
  roue->draw();
  myEngine.mvMatrixStack.popMatrix();
}

void drawWagon(float offsetX)
{
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.mvMatrixStack.addTranslation({offsetX, 0.f, 0.f});

  myEngine.mvMatrixStack.pushMatrix();
  // myEngine.setFlatColor(0.3f, 0.3f, 0.7f); // bleu
  myEngine.activateTexturing(true);
  textureMetalBleu.attachTexture();
  myEngine.mvMatrixStack.addTranslation({0.f, 0.f, 2.0f});
  myEngine.mvMatrixStack.addHomothety({7.f, 4.f, 3.f});
  myEngine.updateMvMatrix();
  wagon->draw();
  myEngine.mvMatrixStack.popMatrix();
  textureMetalBleu.detachTexture();
  myEngine.activateTexturing(false);

  // 4 roues wagon
  // myEngine.setFlatColor(0.15f, 0.15f, 0.15f);
  myEngine.activateTexturing(true);
  textureRoue.attachTexture();
  float roueX[2] = {2.f, -2.f};
  float roueY[2] = {-2.3f, 2.3f};
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      myEngine.mvMatrixStack.pushMatrix();
      myEngine.mvMatrixStack.addTranslation({roueX[i], roueY[j], 0.4f});
      myEngine.mvMatrixStack.addRotation(M_PI, {1, 0, 0});
      myEngine.mvMatrixStack.addHomothety({0.8f, 0.5f, 0.8f});
      myEngine.updateMvMatrix();
      roue->draw();
      myEngine.mvMatrixStack.popMatrix();
    }
  }
  textureRoue.detachTexture();
  myEngine.activateTexturing(false);

  myEngine.mvMatrixStack.popMatrix();
}

void drawTrain()
{
  int idx = circuit.train_pos;
  int n = circuit.path.size();
  int next = (idx + 1) % n;

  // Position par rapport au rail
  float posX = circuit.path[idx].first * circuit.size_grid + circuit.size_grid / 2.f;
  float posY = circuit.path[idx].second * circuit.size_grid + circuit.size_grid / 2.f;

  // Calcul de l'angle d'orientation du train
  float dx = circuit.path[next].first - circuit.path[idx].first;
  float dy = circuit.path[next].second - circuit.path[idx].second;
  float angle = atan2f(dy, dx);

  myEngine.mvMatrixStack.pushMatrix();
  myEngine.mvMatrixStack.addTranslation({posX + dx * 12.f, posY + dy * 12.f, 1.f});
  myEngine.mvMatrixStack.addRotation(angle, {0, 0, 1});

  //lumiere
  float trainX = posX + dx * 12.f;
  float trainY = posY + dy * 12.f;

  float phareX = trainX + 4.f * cos(angle);
  float phareY = trainY + 4.f * sin(angle);
  float phareZ = 1.f + 2.f;

  myEngine.setLightPosition({phareX, phareY, phareZ, 1.f}, 1);

  drawLocomotive();
  drawWagon(-8.5f);
  drawWagon(-16.f);

  myEngine.mvMatrixStack.popMatrix();
  myEngine.updateMvMatrix();
}