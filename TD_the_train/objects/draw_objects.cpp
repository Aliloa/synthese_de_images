#include "draw_objects.hpp"
#include <fstream>  // pour std::ifstream
#include <iostream> // pour les erreurs éventuelles

IndexedMesh *tronc;
StandardMesh *feuillage;

void initObjects()
{
    tronc = basicCylinder(1.0f, 1.0f);
    tronc->createVAO();

    feuillage = basicCone(1.0f, 1.0f);
    feuillage->createVAO();
}

void drawSapin()
{
    // ----------------Tronc
    myEngine.setFlatColor(0.349f, 0.294f, 0.247f);
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
    myEngine.mvMatrixStack.addHomothety({0.8, 2, 0.8});
    myEngine.updateMvMatrix();
    tronc->draw();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.322f, 0.529f, 0.275f);
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
    myEngine.mvMatrixStack.addTranslation({0, 2, 0});
    myEngine.mvMatrixStack.addHomothety({3, 4, 3});
    for (int i = 0; i < 3; i++)
    {
        myEngine.updateMvMatrix();
        feuillage->draw();
        myEngine.mvMatrixStack.addHomothety({0.7, 0.8, 0.7});
        myEngine.mvMatrixStack.addTranslation({0, 0.8, 0});
    }
    myEngine.mvMatrixStack.popMatrix();
}

//Zone ou y a la gare pour éviter de mettre des objets par dessus
bool zoneGare(int x, int z) {
    return (x > -40 && x < 12 && z > -22 && z < 22);
}

void drawRandomSapins()
{
    srand(42);
    float posX = 0;
    float posY = 0;
    for (int i = 0; i < 20; i++)
    {
        posX = (rand() % 95) - 45; // -50 à +50
        posY = (rand() % 95) - 45; // -50 à +50
        if (zoneGare(posX, posY)) { i--; continue; } // réessaie
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({posX, posY, 0});
        drawSapin();
        myEngine.mvMatrixStack.popMatrix();
    }
}