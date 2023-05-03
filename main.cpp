#include <iostream>
#include "Camara.h"
#include <cmath>

void escena1();
void escena2();
void genRandomSpheres(std::vector<Objeto*> &objetos, int numSpheres);

int main() {
    // Camara cam;
    // Generate 1 image
    /*cam.configurar(3,60,600,500,
                   vec3(3,5,50),
                   vec3(0,0,0),
                   vec3(0,1,0));
    cam.renderizar();
    */

    // Generate n images
    /*
    for (int x = 0, n=1; x < 10; x++, n++) {
        cam.configurar(3,60,600,800,
                       vec3(x,5,50),
                       vec3(0,0,0),
                       vec3(0,1,0));
        cam.renderizar(n);
    }
    */

    escena2();
    return 0;
}

void escena1() {
    std::vector <Objeto*> objetos;
    Objeto *p1;
    p1 = new Esfera(vec3(10, 0, 0), 8, vec3(0, 0, 1));
    p1->setConstantes(1, 0);
    p1->ke = 1;
    objetos.emplace_back(p1);

    /*
    p1 = new Esfera(vec3(10, 0, 20), 8, vec3(0.1, 0.1, 0.1));
    p1->setConstantes(0.8, 0.2);
    p1->ke = 0.9;
    p1->es_transparente = true;
    p1->ior = 1.5;
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(0, 10, 0), 8, vec3(1, 0, 0));
    p1->setConstantes(0.6, 0.4, 32);
    p1->ke = 0.8;
    p1->es_transparente = false;
    p1->ior = 1.2;
    objetos.emplace_back(p1);
    */

    p1 = new Plano(vec3(0, 1, 0), 1, vec3(0.123, 0.456, 0.789));
    p1->setConstantes(0.9, 0.1);
    p1->ke = 0.1;
    objetos.emplace_back(p1);

    p1 = new Cilindro(vec3(-20, 0, 0), vec3(-10, 10, 0), 5, vec3(0, 1, 1));
    p1->setConstantes(0.7, 0.3, 8);
    objetos.emplace_back(p1);


    std::vector<Luz*> luces;
    Luz luz(vec3(30, 30, 30), vec3(1, 1, 1));
    luces.emplace_back(&luz);

    Camara cam;
    for (int x = 0, n=100; x < 5; x++, n++) {
        cam.configurar(3, 60, 600, 800,
                       vec3(x, 5, 50),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        cam.renderizar(objetos, luces, n);
    }
}

void escena2() {
    std::vector<Objeto*> objetos;
    Objeto *p1;
    p1 = new Plano(vec3(0,1,0), 1, vec3(0.123, 0.456, 0.789));
//    p1 = new Plano(vec3(0,1,0), 1, vec3(0, 0, 0));
    p1->setConstantes(0.9, 0.1);
    p1->ke = 0.3;
    objetos.emplace_back(p1);

//    p1 = new Cilindro(vec3(5,0,0), vec3(5,140,0),50, vec3(0.6549,0.8471,0.8706));
    p1 = new Cilindro(vec3(5,0,0), vec3(5,140,0),50, vec3(0,0,0));
    p1->setConstantes(0.1,0.9,8);
    p1->es_transparente = true;
    p1->ior = 1.52;
    objetos.emplace_back(p1);

    //p1 = new Cilindro(vec3(6,1,0), vec3(6,138,0),48, vec3(0.6549,0.8471,0.8706));
    p1 = new Cilindro(vec3(6,1,0), vec3(6,138,0),48, vec3(0,0,0));
    p1->setConstantes(0.1,0.9,8);
    p1->es_transparente = true;
    p1->ior = 1.52;
    objetos.emplace_back(p1);

//    p1 = new Esfera(vec3(5,0,-20), 10, vec3(1,0.1,0.1));
//    p1->setConstantes(0.8, 0.2);
//    p1->ke = 0.9;
//    p1->es_transparente = true;
//    p1->ior=1.5;
//    objetos.emplace_back(p1);

    // Luciernaga [3]
    vec3 posLuz1(5, 30, 0);
    vec3 colorLuz1(1, 1, 0);
    p1 = new Esfera(posLuz1, 2, colorLuz1); // esfera luciernaga
    p1->es_luz = true;
    p1->setConstantes(1, 1);
    p1->ke = 1;
    objetos.emplace_back(p1);

    // Luciernaga [4]
    vec3 posLuz2(15, 70, 10);
    vec3 colorLuz2(1, 1, 0);
    p1 = new Esfera(posLuz2, 2, colorLuz2); // esfera luciernaga
    p1->es_luz = true;
    objetos.emplace_back(p1);

    // Luciernaga [5]
    vec3 posLuz3(25, 110, 10);
    vec3 colorLuz3(1, 1, 0);
    p1 = new Esfera(posLuz3, 2, colorLuz3); // esfera luciernaga
    p1->es_luz = true;
    objetos.emplace_back(p1);

    // Luciernaga [6]
    vec3 posLuz4(5, 110, 20);
    vec3 colorLuz4(1, 1, 0);
    p1 = new Esfera(posLuz4, 2, colorLuz4); // esfera luciernaga
    p1->es_luz = true;
    objetos.emplace_back(p1);

    // Add Esferas
    p1 = new Esfera(vec3(100, 20, 0), 20, vec3(0, 0, 1));
    p1->setConstantes(1, 0);
    p1->ke = 1;
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(-100, 20, 0), 20, vec3(1, 0, 0));
    p1->setConstantes(1, 0);
    p1->ke = 1;
    objetos.emplace_back(p1);

    // Add random Spheres
    genRandomSpheres(objetos, 20);

    std::vector<Luz*> luces;
//    Luz luz(vec3(30,30,30), vec3(1,1,1));
//    luces.emplace_back(&luz);
    // Begin Test
    Luz *pluz = new Luz(posLuz1, colorLuz1);
//    Luz *pluz = new Luz(vec3(-120, 30, 30), colorLuz1);
    // End Test
    luces.emplace_back(pluz);

    Camara cam;
    /*
    for (int y = 1, n = 1; y < 100; y += 4, n++) {
        cam.configurar(3, 60, 600, 800,
                       vec3(1, y, 500),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        cam.renderizar(objetos, luces, n);
    }
    */

    // Begin: test circular motion
    Esfera* myFirefly = static_cast<Esfera*>(objetos[3]);
    float r = 10; // used for X and Y axis
    float a = 15; // for Z axis [from 15; -15]

    Esfera* myFirefly2 = static_cast<Esfera*>(objetos[4]);
    Esfera* myFirefly3 = static_cast<Esfera*>(objetos[5]);
    Esfera* myFirefly4 = static_cast<Esfera*>(objetos[6]);

    std::cout << "\n Init myFirefly: x = " << myFirefly->centro.x << ", y = " << myFirefly->centro.y << ", z = " << myFirefly->centro.z << "\n";
    float t = 0;
    float numVueltas = 0;
    float startY = 30.0;
    // Initial Setting for camara
    float camEye_x = 0;
    float camEye_y = 30;
    float camEye_z = 425.0;
    cam.configurar(3, 60, 600, 800,
                   vec3(camEye_x, camEye_y, camEye_z),
                   vec3(0, 0, 0),
                   vec3(0, 1, 0));
    for (int y = 1, n = 1; y <= 740; y += 1, n++) {
        // 1 vuelta
        if (t > 2 * M_PI) {
            std::cout << "una vuelta..\n";
//            numVueltas += 1;
//            break;
        }
        // count vueltas
        numVueltas = t / (2 * M_PI);
        myFirefly->centro.x = r * cos(t);
        myFirefly->centro.x += 10;
        myFirefly2->centro.x = r * cos(t);
        myFirefly2->centro.x += 5;

        myFirefly->centro.y = r * sin(t);
        myFirefly->centro.y += startY;
        myFirefly3->centro.y = r * sin(t);
        myFirefly3->centro.y += 50;
        myFirefly4->centro.y = r * sin(t);
        myFirefly4->centro.y += 20;


        myFirefly->centro.z = a * cos(t);
        myFirefly2->centro.z = a * cos(t);
        std::cout << "\nmyFirefly: t = " <<  t << ", x = " << myFirefly->centro.x << ", y = " << myFirefly->centro.y << ", z = " << myFirefly->centro.z << "\n";
        std::cout << "\nmyFirefly2: t = " <<  t << ", x = " << myFirefly2->centro.x << ", y = " << myFirefly2->centro.y << ", z = " << myFirefly2->centro.z << "\n";
        std::cout << "\nmyFirefly3: t = " <<  t << ", x = " << myFirefly3->centro.x << ", y = " << myFirefly3->centro.y << ", z = " << myFirefly3->centro.z << "\n";
        cam.renderizar(objetos, luces, n);

        t += 0.1;
        std::cout << "#vueltas: " << numVueltas << "\n";

        // Change camara position for next iteration
        if (n >= 1 and n <= 100) {
            // Move z axis [425 -> 375]
            camEye_z = camEye_z - 0.5;
            cam.configurar(3, 60, 600, 800,
                           vec3(camEye_x, camEye_y, camEye_z),
                           vec3(0, 0, 0),
                           vec3(0, 1, 0));
        } else if (n >= 200 and n < 500) {
            // Move y axis [30 -> 180]
            camEye_y = camEye_y + 0.5;
            cam.configurar(3, 60, 600, 800,
                           vec3(camEye_x, camEye_y, camEye_z),
                           vec3(0, 0, 0),
                           vec3(0, 1, 0));
        } else if (n >= 500 and n <= 740) {
            // Move x axis [0 -> 120]
            camEye_x = camEye_x + 0.5;
            cam.configurar(3, 60, 600, 800,
                           vec3(camEye_x, camEye_y, camEye_z),
                           vec3(0, 0, 0),
                           vec3(0, 1, 0));
        }
    }
    // End: test circular motion
}

void genRandomSpheres(std::vector<Objeto*> &objetos, int numSpheres) {
    // Begin Generate Random spheres
    srand (time(NULL));
    int magicUnit;
    int startZ = -60.0;
    int factorZ = 4;
    int spaceX = 50;
    for (int i = 0; i < numSpheres; i++) {   // numSpheres = 50
        auto color = vec3((rand() % 100 + 1)/100.0, (rand() % 100 + 1)/100.0, (rand() % 100 + 1)/100.0);
        color.max_to_one();
        int coin = rand() % 2; // coin: [0, 1]

        if (coin == 1) {
            magicUnit = -1 * coin; // set magicUnit = -1
        } else {
            // if coin is 0
            magicUnit = 1;          // set magicUnit = 1
        }
        float theRadio = (rand() % 6) + 3;  // radio: [3, 9]
        // x = 50 + random_number       (puede ser neegativo o positivo)
        // z = un valor random y en cada iteración un numero más negativo para el eje z
        auto p1 = new Esfera(vec3((spaceX*magicUnit) + (rand() % 80)*magicUnit,theRadio,startZ - (i * factorZ)), theRadio, color);
        p1->setConstantes(0.8, 0.8);
        p1->ke = 0.8;
        objetos.emplace_back(p1);
    }
    // End Generate Random Spheres
}