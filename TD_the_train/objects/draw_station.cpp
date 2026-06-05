#include "draw_station.hpp"
#include <iostream>

static IndexedMesh *murs = nullptr;
static StandardMesh *toit = nullptr;
static IndexedMesh *quai = nullptr;
static IndexedMesh *cheminee = nullptr;
static IndexedMesh *pancarte = nullptr;
static IndexedMesh *poteau = nullptr;
static IndexedMesh *avancee = nullptr;

void initStation()
{
  murs = basicCube(2.0f);
  toit = basicCone(0.8f, 1.5f);
  quai = basicCylinder(1.0f, 1.0f);
  cheminee = basicCylinder(1.0f, 1.0f);
  pancarte = basicCube(1.0f);
  poteau = basicCylinder(1.0f, 1.0f);
  avancee = basicCube(1.0f);

  murs->createVAO();
  toit->createVAO();
  quai->createVAO();
  cheminee->createVAO();
  pancarte->createVAO();
  poteau->createVAO();
  avancee->createVAO();
}

void drawStation()
{
  const float CASE_SIZE = 10.0f;
  const int N = 10;
  float baseX = (circuit.station.first - N / 2) * CASE_SIZE;
  float baseY = (circuit.station.second - N / 2) * CASE_SIZE;

  myEngine.mvMatrixStack.pushMatrix();
  myEngine.mvMatrixStack.addTranslation({baseX, baseY + 10.f, 0.f});

  // Quai
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.75f, 0.75f, 0.72f);
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 0.f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({10.f, 0.3f, 10.f});
  myEngine.updateMvMatrix();
  quai->draw();
  myEngine.mvMatrixStack.popMatrix();

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
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 7.3f});
  myEngine.mvMatrixStack.addRotation(-M_PI / 2, {-2, 0, 0});
  myEngine.mvMatrixStack.addHomothety({9.f, 6.f, 5.f});
  myEngine.updateMvMatrix();
  toit->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Auvent avant
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.25f, 0.15f, 0.05f);
  myEngine.mvMatrixStack.addTranslation({5.f, 1.5f, 4.5f});
  myEngine.mvMatrixStack.addHomothety({7.f, 1.5f, 0.3f});
  myEngine.updateMvMatrix();
  avancee->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Cheminée
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.4f, 0.25f, 0.1f);
  myEngine.mvMatrixStack.addTranslation({5.f, 4.f, 9.f});
  myEngine.mvMatrixStack.addHomothety({0.8f, 2.5f, 0.8f});
  myEngine.updateMvMatrix();
  cheminee->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Poteau pancarte
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.4f, 0.25f, 0.1f);
  myEngine.mvMatrixStack.addTranslation({5.f, -1.f, 1.5f});
  myEngine.mvMatrixStack.addHomothety({0.2f, 0.2f, 3.f});
  myEngine.updateMvMatrix();
  poteau->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Pancarte
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.95f, 0.90f, 0.6f);
  myEngine.mvMatrixStack.addTranslation({5.f, -1.f, 4.f});
  myEngine.mvMatrixStack.addHomothety({3.f, 0.15f, 1.2f});
  myEngine.updateMvMatrix();
  pancarte->draw();
  myEngine.mvMatrixStack.popMatrix();

  myEngine.mvMatrixStack.popMatrix();
  myEngine.updateMvMatrix();
}