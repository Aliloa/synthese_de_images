#include "draw_station.hpp"
#include <iostream>

static IndexedMesh *murs = nullptr;
static StandardMesh *toit = nullptr;
static IndexedMesh *cheminee = nullptr;
static IndexedMesh *pancarte = nullptr;
static IndexedMesh *poteau = nullptr;
static IndexedMesh *porte = nullptr;

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
  myEngine.setFlatColor(0.55f, 0.35f, 0.15f);
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 0.3f});
  myEngine.mvMatrixStack.addHomothety({7.f, 5.f, 7.f});
  myEngine.updateMvMatrix();
  murs->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Toit chalet
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 5.3f});
  myEngine.mvMatrixStack.addRotation(-M_PI / 2, {-1, 0, 0});
  myEngine.mvMatrixStack.addHomothety({9.f, 6.f, 5.f});
  myEngine.updateMvMatrix();
  toit->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Auvent avant
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.mvMatrixStack.addTranslation({10.5f, 5.0f, 4.5f});
  myEngine.mvMatrixStack.addHomothety({2.0f, 5.0f, 0.2f});
  myEngine.updateMvMatrix();
  porte->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Cheminée
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.4f, 0.25f, 0.1f);
  myEngine.mvMatrixStack.addTranslation({5.0f, 4.0f, 9.0f});
  myEngine.mvMatrixStack.addHomothety({0.5f, 0.5f, 2.0f});
  myEngine.updateMvMatrix();
  cheminee->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Poteau pancarte
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.4f, 0.25f, 0.1f);
  myEngine.mvMatrixStack.addTranslation({10.5f, 11.f, 1.5f});
  myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});
  myEngine.mvMatrixStack.addHomothety({0.2f, 0.2f, 3.f});
  myEngine.updateMvMatrix();
  poteau->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Pancarte
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.95f, 0.90f, 0.6f);
  myEngine.mvMatrixStack.addTranslation({10.5f, 11.f, 4.f});
  myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});
  myEngine.mvMatrixStack.addHomothety({3.f, 0.15f, 1.2f});
  myEngine.updateMvMatrix();
  pancarte->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Porte
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.mvMatrixStack.addTranslation({10.5f, 5.0f, 1.5f});
  myEngine.mvMatrixStack.addHomothety({0.1f, 2.0f, 3.5f});
  myEngine.updateMvMatrix();
  porte->draw();
  myEngine.mvMatrixStack.popMatrix();

  myEngine.mvMatrixStack.popMatrix();
  myEngine.updateMvMatrix();
}
