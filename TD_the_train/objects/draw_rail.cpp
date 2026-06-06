#include "draw_rail.hpp"
#include <fstream>  // pour std::ifstream
#include <iostream> // pour les erreurs éventuelles
#include "glbasimac/glbi_texture.hpp"
#include "tools/stb_image.h"

IndexedMesh *cylinder;
IndexedMesh *cube;
GLBI_Convex_2D_Shape rail_courbe{3};

glbasimac::GLBI_Texture textureBallast;
glbasimac::GLBI_Texture textureMetal;

const float POS_X_RAIL1 = 3.f; // rail1
const float POS_X_RAIL2 = 7.f; // rail 2
const float sr = 0.5f;         // epaisseur du rail
const float sx = 0.7f;         // position des balast
const float rr = 0.3f;         // rayon du ballast

void initRail()
{
    cube = basicCube(1.0f);
    cube->createVAO();

    cylinder = basicCylinder(1.0f, 1.0f);
    cylinder->createVAO();

    // textures
    //ballast
    textureBallast.createTexture();
    int w, h, n;
    stbi_set_flip_vertically_on_load(true); // OpenGL attend l'origine en bas
    unsigned char *pixels = stbi_load("../assets/textures/bois.jpg", &w, &h, &n, 0);
    textureBallast.attachTexture();
    textureBallast.loadImage(w, h, n, pixels);
    textureBallast.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureBallast.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureBallast.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
    textureBallast.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
    textureBallast.detachTexture();
    stbi_image_free(pixels);
}

// RAIL DROIT

void drawRailDroit()
{
    // Rail 1
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({5.f, POS_X_RAIL1, 0.f});
    myEngine.mvMatrixStack.addHomothety({10.0f, sr, sr});
    myEngine.updateMvMatrix();
    myEngine.setFlatColor(0.706f, 0.753f, 0.761f);
    cube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // Rail 2
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({5.0f, POS_X_RAIL2, 0.f});
    myEngine.mvMatrixStack.addHomothety({10.0f, sr, sr});
    myEngine.updateMvMatrix();
    cube->draw();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();

    // Ballasts
    // myEngine.setFlatColor(0.431f, 0.357f, 0.278f);
    myEngine.activateTexturing(true);
    textureBallast.attachTexture();
    for (int i = 0; i < 5; i++)
    {
        float posX = sx + i * (sx * 2.f + rr * 2.f);
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({posX, 2, -sr}); //-sr pour qu'il soit bein en bas du rail
        myEngine.mvMatrixStack.addHomothety({rr, 6, rr});      // rayon rr, hauteur 6
        myEngine.updateMvMatrix();
        cylinder->draw();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.updateMvMatrix();
    }
    textureBallast.detachTexture();
    myEngine.activateTexturing(false);
}

// RAIL COURBE

void drawRailCourbeInt(float rayon)
{

    std::vector<float> haut, bas, gauche, droite;

    for (int i = 0; i <= 10; i++)
    {
        float t = (float)i / 10;
        float angle = M_PI / 2.0f * (1.0f - t); // de 90° à 0°

        float cos_a = cos(angle);
        float sin_a = sin(angle);

        // Les 4 coins de la section à ce point
        float xi = rayon * cos_a;        // intérieur
        float xe = (rayon + sr) * cos_a; // extérieur
        float yi = rayon * sin_a;
        float ye = (rayon + sr) * sin_a;
        float zb = -sr / 2.0f; // bas
        float zh = sr / 2.0f;  // haut

        // Face du dessus  (z = sr)
        haut.insert(haut.end(), {xi, yi, zh, xe, ye, zh});
        // Face du dessous (z = 0)
        bas.insert(bas.end(), {xi, yi, zb, xe, ye, zb});
        // Face intérieure (rayon r)
        gauche.insert(gauche.end(), {xi, yi, zb, xi, yi, zh});
        // Face extérieure (rayon r+sr)
        droite.insert(droite.end(), {xe, ye, zb, xe, ye, zh});
    }

    GLBI_Convex_2D_Shape fHaut(3), fBas(3), fGauche(3), fDroite(3);
    fHaut.initShape(haut);
    fHaut.changeNature(GL_TRIANGLE_STRIP);
    fBas.initShape(bas);
    fBas.changeNature(GL_TRIANGLE_STRIP);
    fGauche.initShape(gauche);
    fGauche.changeNature(GL_TRIANGLE_STRIP);
    fDroite.initShape(droite);
    fDroite.changeNature(GL_TRIANGLE_STRIP);

    myEngine.setFlatColor(0.706f, 0.753f, 0.761f);
    fHaut.drawShape();
    fBas.drawShape();
    fGauche.drawShape();
    fDroite.drawShape();
}

void drawBallastsRailCourbe()
{
    // Ballasts
    // myEngine.setFlatColor(0.431f, 0.357f, 0.278f);
    myEngine.activateTexturing(true);
    textureBallast.attachTexture();
    float angleStart = M_PI / 12; // permiere ballast
    float angleStep = M_PI / 7;   // ecart entre chaque ballast
    for (int i = 0; i < 3; i++)
    {
        float angle = angleStart + i * angleStep;
        // POS_X_RAIL1 - 0.6 pour décaler et un peu centrer la ballast
        float posY = (POS_X_RAIL1 - 0.6) * cos(angle); // position en x d'après le cercle trigo
        float posX = (POS_X_RAIL1 - 0.6) * sin(angle); // position en y

        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({posX, posY, -sr});
        myEngine.mvMatrixStack.addRotation(-angle, {0, 0, 1});
        myEngine.mvMatrixStack.addHomothety({rr, 6, rr}); // rayon rr, hauteur 6
        myEngine.updateMvMatrix();
        cylinder->draw();
        myEngine.mvMatrixStack.popMatrix();
        myEngine.updateMvMatrix();
    }
    textureBallast.detachTexture();
    myEngine.activateTexturing(false);
}

void drawRailCourbe()
{
    drawRailCourbeInt(POS_X_RAIL1);
    drawRailCourbeInt(POS_X_RAIL2);
    drawBallastsRailCourbe();
}

void drawRails()
{
    int n = circuit.path.size();
    for (int i = 0; i < n; i++)
    {
        myEngine.mvMatrixStack.pushMatrix();

        float posX = circuit.path[i].first * circuit.size_grid;  // coordonnée en X
        float posY = circuit.path[i].second * circuit.size_grid; // coordonnée en Y

        myEngine.mvMatrixStack.addTranslation({posX, posY, 1}); // position du rail

        // vérifier si la dirrection a changé
        // Direction entrante (d'où on vient)
        int dirX_A = 0;
        int dirY_A = 0;
        int prevI = (i - 1 + n) % n;
        dirX_A = circuit.path[i].first - circuit.path[prevI].first;
        dirY_A = circuit.path[i].second - circuit.path[prevI].second;

        // Direction sortante (où on va)
        int dirX_B = 0;
        int dirY_B = 0;
        int nextI = (i + 1) % n;
        dirX_B = circuit.path[nextI].first - circuit.path[i].first;
        dirY_B = circuit.path[nextI].second - circuit.path[i].second;

        // normaliser quand il y a des gaps
        if (dirX_A > 1)
            dirX_A = 1;
        if (dirX_A < -1)
            dirX_A = -1;
        if (dirY_A > 1)
            dirY_A = 1;
        if (dirY_A < -1)
            dirY_A = -1;
        if (dirX_B > 1)
            dirX_B = 1;
        if (dirX_B < -1)
            dirX_B = -1;
        if (dirY_B > 1)
            dirY_B = 1;
        if (dirY_B < -1)
            dirY_B = -1;

        // si même direction rail droit, sinon rail courbé
        if (dirX_A == dirX_B && dirY_A == dirY_B)
        {
            if (dirX_A == 0) // si ca avance pas sur X
            {
                myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});           // tourner à 90 degrés
                myEngine.mvMatrixStack.addTranslation({0, -circuit.size_grid, 0}); // pour recentrer le rail
            }
            myEngine.updateMvMatrix();
            drawRailDroit();
        }
        else
        {
            // COURBE
            if (dirX_A == -1 && dirY_B == -1)
            {
                myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});
                myEngine.mvMatrixStack.addTranslation({0, -circuit.size_grid, 0}); // pour recentrer le rail
            }
            else if (dirY_A == -1 && dirX_B == 1)
            {
                myEngine.mvMatrixStack.addRotation(M_PI, {0, 0, 1});
                myEngine.mvMatrixStack.addTranslation({-circuit.size_grid, -circuit.size_grid, 0}); // pour recentrer le rail
            }
            else if (dirX_A == 1 && dirY_B == 1)
            {
                myEngine.mvMatrixStack.addRotation(3 * M_PI / 2, {0, 0, 1});
                myEngine.mvMatrixStack.addTranslation({-circuit.size_grid, 0, 0}); // pour recentrer le rail
            }
            else if (dirY_A == -1 && dirX_B == -1)
            {
                myEngine.mvMatrixStack.addRotation(3 * M_PI / 2, {0, 0, 1});
                myEngine.mvMatrixStack.addTranslation({-circuit.size_grid, 0, 0});
            }
            myEngine.updateMvMatrix();
            drawRailCourbe();
        }

        myEngine.mvMatrixStack.popMatrix();
        myEngine.updateMvMatrix();
    }
}