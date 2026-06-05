#include "draw_train.hpp"
#include <iostream>
#include <cmath>

static IndexedMesh *corps = nullptr;
static IndexedMesh *roue = nullptr;
static IndexedMesh *cheminee = nullptr;
static IndexedMesh *wagon = nullptr;

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
}

void drawLocomotive()
{
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.8f, 0.1f, 0.1f);
  myEngine.mvMatrixStack.addTranslation({0.f, 0.f, 2.0f});
  myEngine.mvMatrixStack.addHomothety({8.f, 4.f, 3.f});
  myEngine.updateMvMatrix();
  corps->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Cabine
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.6f, 0.05f, 0.05f);
  myEngine.mvMatrixStack.addTranslation({-1.5f, 0.f, 3.5f});
  myEngine.mvMatrixStack.addHomothety({3.f, 3.5f, 2.f});
  myEngine.updateMvMatrix();
  corps->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Cheminée
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.2f, 0.2f, 0.2f);
  myEngine.mvMatrixStack.addTranslation({0.f, 0.f, 4.5f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({0.6f, 2.5f, 0.6f});
  myEngine.updateMvMatrix();
  cheminee->draw();
  myEngine.mvMatrixStack.popMatrix();

  // 4 Roues
  myEngine.setFlatColor(0.15f, 0.15f, 0.15f);
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
}

void drawWagon(float offsetX)
{
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.mvMatrixStack.addTranslation({offsetX, 0.f, 0.f});

  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.3f, 0.3f, 0.7f); // bleu
  myEngine.mvMatrixStack.addTranslation({0.f, 0.f, 2.0f});
  myEngine.mvMatrixStack.addHomothety({7.f, 4.f, 3.f});
  myEngine.updateMvMatrix();
  wagon->draw();
  myEngine.mvMatrixStack.popMatrix();

  // 4 roues wagon
  myEngine.setFlatColor(0.15f, 0.15f, 0.15f);
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

  drawLocomotive();
  drawWagon(-8.5f);
  drawWagon(-16.f);

  myEngine.mvMatrixStack.popMatrix();
  myEngine.updateMvMatrix();
}