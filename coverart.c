// #include <GLUT/glut.h>
#define GL_GLEXT_PROTOTYPES
#include "GL/glut.h"

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

void drawRectangles() {
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float allVertices[18 * 6][24];  // 6 rows of 18 bricks

    float darkGreenVertices[] = {
        // positions         // colors
        0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
        0.1f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f,
        0.1f, 0.1f, 0.0f, 0.0f, 0.3f, 0.0f,
        0.0f, 0.1f, 0.0f, 0.0f, 0.3f, 0.0f};

    for (int i = 0; i < 18; i++) {
        float x1 = (i - 9) * 0.1f;  // x coordinate of the left side of the square
        float x2 = x1 + 0.1f;       // x coordinate of the right side of the square
        for (int j = 0; j < 24; j++) {
            allVertices[i][j] = darkGreenVertices[j];
        }
        allVertices[i][0] = allVertices[i][18] = x1;
        allVertices[i][6] = allVertices[i][12] = x2;
    }

    // -------------------------------------------------------------------
    float skyBlueVertices[] = {
        // positions         // colors
        0.0f, 0.1f, 0.0f, 0.43f, 0.85f, 0.85f,
        0.1f, 0.1f, 0.0f, 0.23f, 0.65f, 0.65f,
        0.1f, 0.2f, 0.0f, 0.23f, 0.65f, 0.65f,
        0.0f, 0.2f, 0.0f, 0.23f, 0.65f, 0.65f};

    for (int i = 18; i < 36; i++) {
        float x1 = (i - 27) * 0.1f;  // x coordinate of the left side of the square
        float x2 = x1 + 0.1f;        // x coordinate of the right side of the square
        for (int j = 0; j < 24; j++) {
            allVertices[i][j] = skyBlueVertices[j];
        }
        allVertices[i][0] = allVertices[i][18] = x1;
        allVertices[i][6] = allVertices[i][12] = x2;
    }

    // -------------------------------------------------------------------
    float pinkVertices[] = {
        // positions         // colors
        0.0f, 0.2f, 0.0f, 1.0f, 0.4f, 0.8f,
        0.1f, 0.2f, 0.0f, 0.8f, 0.2f, 0.6f,
        0.1f, 0.3f, 0.0f, 0.8f, 0.2f, 0.6f,
        0.0f, 0.3f, 0.0f, 0.8f, 0.2f, 0.6f};

    for (int i = 36; i < 54; i++) {
        float x1 = (i - 45) * 0.1f;  // x coordinate of the left side of the square
        float x2 = x1 + 0.1f;        // x coordinate of the right side of the square
        for (int j = 0; j < 24; j++) {
            allVertices[i][j] = pinkVertices[j];
        }
        allVertices[i][0] = allVertices[i][18] = x1;
        allVertices[i][6] = allVertices[i][12] = x2;
    }

    // -------------------------------------------------------------------
    float greenVertices[] = {
        // positions         // colors
        0.0f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.1f, 0.3f, 0.0f, 0.0f, 0.8f, 0.0f,
        0.1f, 0.4f, 0.0f, 0.0f, 0.8f, 0.0f,
        0.0f, 0.4f, 0.0f, 0.0f, 0.8f, 0.0f};

    for (int i = 54; i < 72; i++) {
        float x1 = (i - 63) * 0.1f;  // x coordinate of the left side of the square
        float x2 = x1 + 0.1f;        // x coordinate of the right side of the square
        for (int j = 0; j < 24; j++) {
            allVertices[i][j] = greenVertices[j];
        }
        allVertices[i][0] = allVertices[i][18] = x1;
        allVertices[i][6] = allVertices[i][12] = x2;
    }

    // -------------------------------------------------------------------
    float blueVertices[] = {
        // positions         // colors
        0.0f, 0.4f, 0.0f, 0.2f, 0.2f, 0.8f,
        0.1f, 0.4f, 0.0f, 0.0f, 0.0f, 0.6f,
        0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.6f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.6f};

    for (int i = 72; i < 90; i++) {
        float x1 = (i - 81) * 0.1f;  // x coordinate of the left side of the square
        float x2 = x1 + 0.1f;        // x coordinate of the right side of the square
        for (int j = 0; j < 24; j++) {
            allVertices[i][j] = blueVertices[j];
        }
        allVertices[i][0] = allVertices[i][18] = x1;
        allVertices[i][6] = allVertices[i][12] = x2;
    }

    // -------------------------------------------------------------------
    float redVertices[] = {
        // positions         // colors
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.1f, 0.5f, 0.0f, 0.8f, 0.0f, 0.0f,
        0.1f, 0.6f, 0.0f, 0.8f, 0.0f, 0.0f,
        0.0f, 0.6f, 0.0f, 0.8f, 0.0f, 0.0f};

    for (int i = 90; i < 108; i++) {
        float x1 = (i - 99) * 0.1f;  // x coordinate of the left side of the square
        float x2 = x1 + 0.1f;        // x coordinate of the right side of the square
        for (int j = 0; j < 24; j++) {
            allVertices[i][j] = redVertices[j];
        }
        allVertices[i][0] = allVertices[i][18] = x1;
        allVertices[i][6] = allVertices[i][12] = x2;
    }

    // -------------------------------------------------------------------
    float upperLeftWall[] = {
        // positions         // colors
        -1.0f, -0.8f, 0.0f, 0.6f, 0.6f, 0.6f,
        -0.9f, -0.8f, 0.0f, 0.6f, 0.6f, 0.6f,
        -0.9f, 0.8f, 0.0f, 0.6f, 0.6f, 0.6f,
        -1.0f, 0.8f, 0.0f, 0.6f, 0.6f, 0.6f};

    // -------------------------------------------------------------------
    float bottomLeftWall[] = {
        // positions         // colors
        -1.0f, -0.9f, 0.0f, 0.6f, 0.6f, 0.6f,
        -0.9f, -0.9f, 0.0f, 0.6f, 0.6f, 0.6f,
        -0.9f, -1.0f, 0.0f, 0.6f, 0.6f, 0.6f,
        -1.0f, -1.0f, 0.0f, 0.6f, 0.6f, 0.6f};

    // -------------------------------------------------------------------
    float upperRightWall[] = {
        // positions         // colors
        1.0f, -0.8f, 0.0f, 0.6f, 0.6f, 0.6f,
        0.9f, -0.8f, 0.0f, 0.6f, 0.6f, 0.6f,
        0.9f, 0.8f, 0.0f, 0.6f, 0.6f, 0.6f,
        1.0f, 0.8f, 0.0f, 0.6f, 0.6f, 0.6f};

    // -------------------------------------------------------------------
    float bottomRightWall[] = {
        // positions         // colors
        1.0f, -0.9f, 0.0f, 0.6f, 0.6f, 0.6f,
        0.9f, -0.9f, 0.0f, 0.6f, 0.6f, 0.6f,
        0.9f, -1.0f, 0.0f, 0.6f, 0.6f, 0.6f,
        1.0f, -1.0f, 0.0f, 0.6f, 0.6f, 0.6f};

    // -------------------------------------------------------------------
    float topWall[] = {
        // positions         // colors
        -1.0f, 0.7f, 0.0f, 0.6f, 0.6f, 0.6f,
        1.0f, 0.7f, 0.0f, 0.6f, 0.6f, 0.6f,
        1.0f, 0.87f, 0.0f, 0.6f, 0.6f, 0.6f,
        -1.0f, 0.87f, 0.0f, 0.6f, 0.6f, 0.6f};

    // -------------------------------------------------------------------
    float paddleVertices[] = {
        // positions         // colors
        -0.1f, -0.9f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.1f, -0.9f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.1f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.1f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f};

    // -------------------------------------------------------------------
    float ballVertices[] = {
        // positions         // colors
        0.0f, -0.7f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.01f, -0.7f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.01f, -0.72f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, -0.72f, 0.0f, 1.0f, 1.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    int n = 18 * 6 + 7;  // number of rectangles+walls+paddle
    unsigned int VBOs[n], VAOs[n], EBO;
    glGenVertexArrays(n, VAOs);
    glGenBuffers(n, VBOs);
    glGenBuffers(1, &EBO);

    // create squares
    for (int i = 0; i < 108; i++) {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(allVertices[i]), allVertices[i], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // upper left wall ----------------------------------
    glBindVertexArray(VAOs[108]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[108]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(upperLeftWall), upperLeftWall, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bottom left wall -------------------------------
    glBindVertexArray(VAOs[109]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[109]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottomLeftWall), bottomLeftWall, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // upper right wall ---------------------------------
    glBindVertexArray(VAOs[110]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[110]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(upperRightWall), upperRightWall, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bottom right wall -------------------------
    glBindVertexArray(VAOs[111]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[111]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottomRightWall), bottomRightWall, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // top wall -------------------------
    glBindVertexArray(VAOs[112]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[112]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(topWall), topWall, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // paddle -------------------------
    glBindVertexArray(VAOs[113]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[113]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices), paddleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ball -------------------------
    glBindVertexArray(VAOs[114]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[114]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertices), ballVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to
    glUseProgram(shaderProgram);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render bricks
    for (int i = 0; i < n; i++) {
        glBindVertexArray(VAOs[i]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    // create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Brick Breaker - 260899684");
    glutDisplayFunc(drawRectangles);
    glutMainLoop();
    return 0;
}