#include "draw_station.hpp"
#include <iostream>

static IndexedMesh *murs = nullptr;
static StandardMesh *toit = nullptr;
static IndexedMesh *quai = nullptr;
static IndexedMesh *cheminee = nullptr;

void initStation()
{
  murs = basicCylinder(1.0f, 1.0f);
  toit = basicCone(1.0f, 1.0f);
  quai = basicCylinder(1.0f, 1.0f);
  cheminee = basicCylinder(1.0f, 1.0f);

  murs->createVAO();
  toit->createVAO();
  quai->createVAO();
  cheminee->createVAO();
}

void drawStation()
{
  const float CASE_SIZE = 10.0f;
  const int N = 10;
  float baseX = (circuit.station.first - N / 2) * CASE_SIZE;
  float baseY = (circuit.station.second - N / 2) * CASE_SIZE;

  myEngine.mvMatrixStack.pushMatrix();
  myEngine.mvMatrixStack.addTranslation({baseX, baseY, 0.f});

  // Quai
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.75f, 0.75f, 0.72f);
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 0.f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({10.f, 0.3f, 10.f});
  myEngine.updateMvMatrix();
  quai->draw();
  myEngine.mvMatrixStack.popMatrix();

  // bâtiment
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.85f, 0.72f, 0.55f);
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 0.3f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({7.f, 5.f, 7.f});
  myEngine.updateMvMatrix();
  murs->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Toit
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.6f, 0.15f, 0.15f);
  myEngine.mvMatrixStack.addTranslation({5.f, 5.f, 5.3f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({8.f, 3.5f, 8.f});
  myEngine.updateMvMatrix();
  toit->draw();
  myEngine.mvMatrixStack.popMatrix();

  // Cheminée
  myEngine.mvMatrixStack.pushMatrix();
  myEngine.setFlatColor(0.3f, 0.3f, 0.3f);
  myEngine.mvMatrixStack.addTranslation({5.f, 4.f, 8.f});
  myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
  myEngine.mvMatrixStack.addHomothety({0.8f, 2.5f, 0.8f});
  myEngine.updateMvMatrix();
  cheminee->draw();
  myEngine.mvMatrixStack.popMatrix();

  myEngine.mvMatrixStack.popMatrix();
  myEngine.updateMvMatrix();
}