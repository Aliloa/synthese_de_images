#include "draw_objects.hpp"
#include <fstream>  // pour std::ifstream
#include <iostream> // pour les erreurs éventuelles
#include "glbasimac/glbi_texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"

IndexedMesh *tronc;
StandardMesh *feuillage;
IndexedMesh *sphere;
IndexedMesh *bodyCube;
IndexedMesh *bodyCylinder;

// textures
glbasimac::GLBI_Texture textureFeuillage;
glbasimac::GLBI_Texture textureTronc;
glbasimac::GLBI_Texture textureMushroom;
glbasimac::GLBI_Texture textureMushroomBottom;

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

    // textures
    textureFeuillage.createTexture();

    // feuilles du sapin
    int w, h, n;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load("../assets/textures/sapin.jpg", &w, &h, &n, 0);
    textureFeuillage.attachTexture();
    textureFeuillage.loadImage(w, h, n, pixels);
    textureFeuillage.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureFeuillage.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureFeuillage.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
    textureFeuillage.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
    textureFeuillage.detachTexture();
    stbi_image_free(pixels);

    // bois
    textureTronc.createTexture();
    int w1, h1, n1;
    unsigned char *pixelsBois = stbi_load("../assets/textures/bois.jpg", &w1, &h1, &n1, 0);
    textureTronc.attachTexture();
    textureTronc.loadImage(w1, h1, n1, pixelsBois);
    textureTronc.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureTronc.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureTronc.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
    textureTronc.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
    textureTronc.detachTexture();
    stbi_image_free(pixelsBois);

    // mushroom
    textureMushroom.createTexture();
    int w2, h2, n2;
    unsigned char *pixelsMushroom = stbi_load("../assets/textures/mushroom.png", &w2, &h2, &n2, 0);
    textureMushroom.attachTexture();
    textureMushroom.loadImage(w2, h2, n2, pixelsMushroom);
    textureMushroom.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureMushroom.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureMushroom.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
    textureMushroom.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
    textureMushroom.detachTexture();
    stbi_image_free(pixelsMushroom);

    // mushroomBottom
    textureMushroomBottom.createTexture();
    int w3, h3, n3;
    unsigned char *pixelsMushroomBottom = stbi_load("../assets/textures/mushroom_bottom.png", &w3, &h3, &n3, 0);
    textureMushroomBottom.attachTexture();
    textureMushroomBottom.loadImage(w3, h3, n3, pixelsMushroomBottom);
    textureMushroomBottom.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureMushroomBottom.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureMushroomBottom.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
    textureMushroomBottom.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
    textureMushroomBottom.detachTexture();
    stbi_image_free(pixelsMushroomBottom);
}

void drawSapin()
{
    // Tronc
    // myEngine.setFlatColor(0.349f, 0.294f, 0.247f);
    myEngine.activateTexturing(true);
    textureTronc.attachTexture();
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
    myEngine.mvMatrixStack.addHomothety({0.8, 2, 0.8});
    myEngine.updateMvMatrix();
    tronc->draw();
    myEngine.mvMatrixStack.popMatrix();
    textureTronc.detachTexture();

    myEngine.mvMatrixStack.pushMatrix();
    // myEngine.setFlatColor(0.322f, 0.529f, 0.275f);
    textureFeuillage.attachTexture();
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

    textureFeuillage.detachTexture();
    myEngine.activateTexturing(false);
    myEngine.mvMatrixStack.popMatrix();
}

void drawMushroom()
{
    // Bas
    // myEngine.setFlatColor(0.941f, 0.894f, 0.792f);
    myEngine.activateTexturing(true);
    textureMushroomBottom.attachTexture();
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
    myEngine.mvMatrixStack.addHomothety({0.9, 1.8, 0.9});
    myEngine.updateMvMatrix();
    tronc->draw();
    myEngine.mvMatrixStack.popMatrix();
    textureMushroomBottom.detachTexture();

    // //Chapeau
    myEngine.mvMatrixStack.pushMatrix();
    // myEngine.setFlatColor(0.871f, 0.251f, 0.169f);
    textureMushroom.attachTexture();
    myEngine.mvMatrixStack.addTranslation({0, 0, 2.2});
    myEngine.mvMatrixStack.addHomothety({1.9, 1.9, 1.2});
    myEngine.updateMvMatrix();
    sphere->draw();

    textureMushroom.detachTexture();
    myEngine.activateTexturing(false);
    myEngine.mvMatrixStack.popMatrix();
}

// Zone ou y a la gare pour éviter de mettre des objets par dessus
bool zoneGare(int x, int y)
{
    return (x > -40 && x < 12 && y > -22 && y < 32);
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

void drawRandomShrooms()
{
    srand(51);
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
        myEngine.mvMatrixStack.addHomothety({0.5, 0.5, 0.5});
        drawMushroom();
        myEngine.mvMatrixStack.popMatrix();
    }
}

// CORP GENERAL
struct Color
{
    float r, g, b;
};

// bras animé gauche
void drawArm(Color body_color, Color hands_color)
{
    myEngine.setFlatColor(body_color.r, body_color.g, body_color.b);
    myEngine.mvMatrixStack.pushMatrix();
    // myEngine.mvMatrixStack.addTranslation({2, 0, 2});
    myEngine.mvMatrixStack.addRotation(M_PI, {1, 1, 0});
    myEngine.mvMatrixStack.addHomothety({0.2, 1, 0.2});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    myEngine.setFlatColor(hands_color.r, hands_color.g, hands_color.b);
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({1, 0, 0});
    myEngine.mvMatrixStack.addHomothety({0.4, 0.4, 0.4});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawAnimatedArm(Color body_color, Color hands_color, float handAnimation)
{
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({0.9, 0, 2});
    myEngine.mvMatrixStack.addRotation(handAnimation, {0, 1, 0});
    drawArm({body_color}, // corps
            {hands_color});
    myEngine.mvMatrixStack.popMatrix();
}

void drawBody(Color body_color, Color collar_color, Color legs_color, Color hands_color)
{
    // Tete
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.988f, 0.87f, 0.761f);
    myEngine.mvMatrixStack.addTranslation({0, 0, 3.5});
    myEngine.mvMatrixStack.addHomothety({1.1, 1.1, 1.1});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    // yeux
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

    // pupilles
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

    // bouche
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.f, 0.f, 0.f);
    myEngine.mvMatrixStack.addTranslation({-0.2, 1.05, 3.});
    myEngine.mvMatrixStack.addRotation(M_PI, {1, 1, 0});
    myEngine.mvMatrixStack.addHomothety({0.02, 0.5, 0.02});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    // corps
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(body_color.r, body_color.g, body_color.b);
    myEngine.mvMatrixStack.addTranslation({0, 0, 1.8});
    myEngine.mvMatrixStack.addHomothety({1.1, 1.1, 1});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({0, 0, 0.8});
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1}); // redresser le cylindre
    myEngine.mvMatrixStack.addHomothety({1.1, 1.1, 1});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    // col
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(collar_color.r, collar_color.g, collar_color.b);
    myEngine.mvMatrixStack.addTranslation({0, 0, 2.5});
    myEngine.mvMatrixStack.addHomothety({0.9, 0.9, 0.2});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    // jambes
    float posX_jambe = -0.5f;
    myEngine.setFlatColor(legs_color.r, legs_color.g, legs_color.b);
    for (int i = 0; i < 2; i++)
    {
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({posX_jambe, 0, 0.2});
        myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
        myEngine.mvMatrixStack.addHomothety({0.5, 0.65, 0.9});
        myEngine.updateMvMatrix();
        bodyCylinder->draw();
        myEngine.mvMatrixStack.popMatrix();
        posX_jambe += 1;
    }

    // pieds
    float posX_pied = -0.5f;
    myEngine.setFlatColor(0.05f, 0.05f, 0.05f);
    for (int i = 0; i < 2; i++)
    {
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({posX_pied, 0.4, 0.1});
        myEngine.mvMatrixStack.addHomothety({0.9, 2, 0.2});
        myEngine.updateMvMatrix();
        bodyCube->draw();
        myEngine.mvMatrixStack.popMatrix();
        posX_pied += 1;
    }

    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({-0.9, 0, 2});
    myEngine.mvMatrixStack.addRotation(M_PI - 0.7, {0, 1, 0});
    drawArm(body_color, hands_color);
    myEngine.mvMatrixStack.popMatrix();

    // bras gauche
    // myEngine.setFlatColor(body_color.r, body_color.g, body_color.b);
    // myEngine.mvMatrixStack.pushMatrix();
    // myEngine.mvMatrixStack.addTranslation({-2, 0, 2});
    // myEngine.mvMatrixStack.addRotation(M_PI, {1, 1, 0});
    // myEngine.mvMatrixStack.addHomothety({0.2, 1, 0.2});
    // myEngine.updateMvMatrix();
    // bodyCylinder->draw();
    // myEngine.mvMatrixStack.popMatrix();

    // // mains
    // myEngine.setFlatColor(hands_color.r, hands_color.g, hands_color.b);
    // myEngine.mvMatrixStack.pushMatrix();
    // myEngine.mvMatrixStack.addTranslation({-2, 0, 2});
    // myEngine.mvMatrixStack.addHomothety({0.4, 0.4, 0.4});
    // myEngine.updateMvMatrix();
    // sphere->draw();
    // myEngine.mvMatrixStack.popMatrix();
}

// HATS
void drawKyleHat()
{
    // chapeau
    // derriere
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.145f, 0.741f, 0.216f);
    myEngine.mvMatrixStack.addTranslation({0, 0, 4.5});
    myEngine.mvMatrixStack.addHomothety({2, 2, 1});
    myEngine.updateMvMatrix();
    bodyCube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // devant
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.153f, 0.612f, 0.208f);
    myEngine.mvMatrixStack.addTranslation({0, 1, 4.2});
    myEngine.mvMatrixStack.addHomothety({1.7, 0.5, 0.5});
    myEngine.updateMvMatrix();
    bodyCube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // oreille du chapeau
    float posX_chapeau = 1.0f;
    myEngine.setFlatColor(0.145f, 0.741f, 0.216f);
    for (int i = 0; i < 2; i++)
    {
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

// KYLE
void drawKyle(float handAnimation)
{
    drawBody({1.f, 0.549f, 0.125f},     // corps
             {0.153f, 0.612f, 0.208f},  // collar
             {0.133f, 0.329f, 0.188f},  // jambes
             {0.145f, 0.741f, 0.216f}); // mains
    drawKyleHat();
    drawAnimatedArm({1.f, 0.549f, 0.125f}, // corps
                    {0.145f, 0.741f, 0.216f}, handAnimation);
}

// STAN

void drawHat(Color full_color, Color pompom_color)
{
    // bonnet
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(full_color.r, full_color.g, full_color.b);
    myEngine.mvMatrixStack.addTranslation({0, 0, 4.4});
    myEngine.mvMatrixStack.addHomothety({1.15, 1.15, 0.8});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();

    // bas
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(pompom_color.r, pompom_color.g, pompom_color.b);
    myEngine.mvMatrixStack.addTranslation({0, 0, 4});
    myEngine.mvMatrixStack.addRotation(M_PI, {0, 1, 1});
    myEngine.mvMatrixStack.addHomothety({1.15, 0.3, 1.15});
    myEngine.updateMvMatrix();
    bodyCylinder->draw();
    myEngine.mvMatrixStack.popMatrix();

    // pompom
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({0, 0, 5.4});
    myEngine.mvMatrixStack.addHomothety({0.4, 0.4, 0.4});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawStan(float handAnimation)
{
    drawBody({0.616f, 0.369f, 0.314f},  // corps
             {0.851f, 0.141f, 0.255f},  // collar
             {0.31f, 0.365f, 0.627f},   // jambes
             {0.851f, 0.141f, 0.255f}); // mains
    drawHat({0.31f, 0.365f, 0.627f},    // chapeau
            {0.851f, 0.141f, 0.255f});
    drawAnimatedArm({0.616f, 0.369f, 0.314f}, // corps
                    {0.851f, 0.141f, 0.255f}, // main
                    handAnimation);
}

// CARTMAN
void drawCartman(float handAnimation)
{
    myEngine.mvMatrixStack.addHomothety({1.5, 1.2, 1});
    drawBody({0.843f, 0.129f, 0.255f}, // corps
             {0.843f, 0.129f, 0.255f}, // collar
             {0.486f, 0.298f, 0.231f}, // jambes
             {0.98f, 0.882f, 0.016f}); // mains
    drawHat({0.275f, 0.698f, 0.725f},  // chapeau
            {0.976f, 0.867f, 0.004f});

    drawAnimatedArm({0.843f, 0.129f, 0.255f}, // corps
                    {0.98f, 0.882f, 0.016f},  // main
                    handAnimation);
}

void drawSoleil()
{
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(1.f, 0.95f, 0.2f);
    myEngine.mvMatrixStack.addTranslation({30.f, 30.f, 25.f});
    myEngine.mvMatrixStack.addHomothety({4.f, 4.f, 4.f});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawLune()
{
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.9f, 0.9f, 0.85f);
    myEngine.mvMatrixStack.addTranslation({30.f, 30.f, 25.f});
    myEngine.mvMatrixStack.addHomothety({3.f, 3.f, 3.f});
    myEngine.updateMvMatrix();
    sphere->draw();
    myEngine.mvMatrixStack.popMatrix();
}