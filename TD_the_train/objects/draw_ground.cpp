#include "draw_ground.hpp"
#include <fstream>  // pour std::ifstream
#include <iostream> // pour les erreurs éventuelles

GLBI_Convex_2D_Shape ground{3};

void initGround()
{
    //  case 10x10
    std::vector<float> baseCarre{
        0.0, 0.0, 0.0,
        10.0, 0.0, 0.0,
        10.0, 10.0, 0.0,
        0.0, 10.0, 0.0};

    ground.initShape(baseCarre);
    ground.changeNature(GL_TRIANGLE_FAN);
}

void drawGround()
{
    myEngine.setFlatColor(0.31, 0.459, 0.267);
    const int N = 10;
    const float CASE_SIZE = 10.0f;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            myEngine.mvMatrixStack.pushMatrix();
            // decalage des cases
            myEngine.mvMatrixStack.addTranslation({(i - N / 2) * CASE_SIZE,
                                                   (j - N / 2) * CASE_SIZE,
                                                   0.0f});
            myEngine.updateMvMatrix();

            // cases qui alternent entre deux verts
            if ((i + j) % 2 == 0)
            {
                myEngine.setFlatColor(0.31f, 0.459f, 0.267f); // vert clair
            }
            else
            {
                myEngine.setFlatColor(0.267, 0.412, 0.227); // vert foncé
            }
            ground.drawShape();

            myEngine.mvMatrixStack.popMatrix();
            myEngine.updateMvMatrix();
        }
    }
}