#include "draw_objects.hpp"
#include <fstream>  // pour std::ifstream
#include <iostream> // pour les erreurs éventuelles

IndexedMesh *tronc;
StandardMesh *feuillage;
IndexedMesh *sphere;
IndexedMesh *bodyCube;
IndexedMesh *bodyCylinder;

void initObjects()
{
    tronc = basicCylinder(1.0f, 1.0f);
    tronc->createVAO();

    feuillage = basicCone(1.0f, 1.0f);
    feuillage->createVAO();

    sphere = basicSphere(1.0f);
    sphere->createVAO();

    bodyCube = basicCube(1.0f);
    bodyCube->createVAO();

    bodyCylinder = basicCylinder(1.0f, 1.0f);
    bodyCylinder->createVAO();
}

void drawSapin()
{
    // Tronc
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

// Zone ou y a la gare pour éviter de mettre des objets par dessus
bool zoneGare(int x, int z)
{
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
        if (zoneGare(posX, posY))
        {
            i--;
            continue;
        } // réessaie
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({posX, posY, 0});
        drawSapin();
        myEngine.mvMatrixStack.popMatrix();
    }
}

//----------------------KYLE

void drawKyle()
{
    // ----------------Tete
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.988f, 0.87f, 0.761f);
    // myEngine.mvMatrixStack.addHomothety({0.8, 2, 0.8});
    myEngine.mvMatrixStack.addTranslation({0, 0, 3.5});
    myEngine.mvMatrixStack.addHomothety({1.1, 1.1, 1.1});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    //--------------------------yeux
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(1.f, 1.f, 1.f);
    myEngine.mvMatrixStack.addTranslation({0.2, 1.05, 3.68});
    myEngine.mvMatrixStack.addRotation(-0.5, {0, 1, 0});
    myEngine.mvMatrixStack.addHomothety({0.28, 0.15, 0.35});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({-0.2, 1.05, 3.68});
    myEngine.mvMatrixStack.addRotation(0.5, {0, 1, 0});
    myEngine.mvMatrixStack.addHomothety({0.28, 0.15, 0.35});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    //--------------------------pupilles
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.f, 0.f, 0.f);
    myEngine.mvMatrixStack.addTranslation({0.2, 1.15, 3.68});
    myEngine.mvMatrixStack.addHomothety({0.07, 0.07, 0.07});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({-0.2, 1.15, 3.68});
    myEngine.mvMatrixStack.addHomothety({0.07, 0.07, 0.07});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    //------------------------------bouche
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.f, 0.f, 0.f);
    myEngine.mvMatrixStack.addTranslation({-0.2, 1.05, 3.});
    myEngine.mvMatrixStack.addRotation(M_PI, {1, 1, 0});
    myEngine.mvMatrixStack.addHomothety({0.02, 0.5, 0.02});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    //--------------------------corps
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(1.f, 0.549f, 0.125f);
    myEngine.mvMatrixStack.addTranslation({0, 0, 1.8});
    myEngine.mvMatrixStack.addHomothety({1.1, 1.1, 1});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({0, 0, 0.8});
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1}); //redresser le cylindre
    myEngine.mvMatrixStack.addHomothety({1.1, 1.1, 1});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    //---------------jambes
    float posX_jambe = -0.5f;
    myEngine.setFlatColor(0.133f, 0.329f, 0.188f);
for (int i = 0; i < 2; i++) {
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({posX_jambe, 0, 0.2});
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
    myEngine.mvMatrixStack.addHomothety({0.5, 0.65, 0.9});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();
    posX_jambe += 1;
}

//----------------pieds
    float posX_pied = -0.5f;
    myEngine.setFlatColor(0.05f, 0.05f, 0.05f);
    for (int i = 0; i < 2; i++) {
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({posX_pied, 0.4, 0.1});
    myEngine.mvMatrixStack.addHomothety({0.9, 2, 0.2});
    myEngine.updateMvMatrix();
    bodyCube->draw();
    myEngine.mvMatrixStack.popMatrix();
    posX_pied += 1;
}

//-------------bras
    float posX_bras = -2.0f;
    myEngine.setFlatColor(1.f, 0.549f, 0.125f);
    for (int i = 0; i < 2; i++) {
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({posX_bras, 0, 2});
    myEngine.mvMatrixStack.addRotation(M_PI, {1, 1, 0});
    myEngine.mvMatrixStack.addHomothety({0.2, 1, 0.2});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();
    posX_bras += 3;
}

//-------------mains
    float posX_main = -2.0f;
    myEngine.setFlatColor(0.145f, 0.741f, 0.216f);
    for (int i = 0; i < 2; i++) {
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({posX_main, 0, 2});
    myEngine.mvMatrixStack.addHomothety({0.4, 0.4, 0.4});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
    posX_main += 4;
}

//---------------chapeau
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.145f, 0.741f, 0.216f);
    myEngine.mvMatrixStack.addTranslation({0, 0, 4.5});
    myEngine.mvMatrixStack.addHomothety({2, 2, 1});
    myEngine.updateMvMatrix();
    bodyCube->draw();
    myEngine.mvMatrixStack.popMatrix();

        //oreille du chapeau
            float posX_chapeau = 1.0f;
            for (int i = 0; i < 2; i++) {
                myEngine.mvMatrixStack.pushMatrix();
                myEngine.mvMatrixStack.addTranslation({posX_chapeau, 0, 4});
                myEngine.mvMatrixStack.addRotation((i == 0 ? -0.5f : 0.5f), {0, 1, 0});
                myEngine.mvMatrixStack.addHomothety({0.4, 2, 1.5});
                myEngine.updateMvMatrix();
                bodyCube->draw();
                myEngine.mvMatrixStack.popMatrix();
                posX_chapeau -= 2;
                }
}