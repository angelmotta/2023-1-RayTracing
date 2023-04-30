#include <iostream>
#include "Camara.h"
#include <cmath>

void escena1();
void escena2();

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
    p1->setConstantes(0.9, 0.1);
    p1->ke = 0.1;
    objetos.emplace_back(p1);

    p1 = new Cilindro(vec3(5,4,0), vec3(5,140,0),50, vec3(0,1,1));
    p1->setConstantes(0.7,0.3,8);
    p1->es_transparente = true;
    p1->ior = 1.3;
    objetos.emplace_back(p1);

//    p1 = new Esfera(vec3(5,0,-20), 10, vec3(1,0.1,0.1));
//    p1->setConstantes(0.8, 0.2);
//    p1->ke = 0.9;
//    p1->es_transparente = true;
//    p1->ior=1.5;
//    objetos.emplace_back(p1);

    // Luciernaga [2]
    vec3 posLuz1(5, 30, 0);
    vec3 colorLuz1(1, 1, 0);
    p1 = new Esfera(posLuz1, 3, colorLuz1); // esfera luciernaga
    p1->es_luz = true;
    objetos.emplace_back(p1);

    // Luciernaga [3]
    vec3 posLuz2(15, 70, 10);
    vec3 colorLuz2(1, 1, 0);
    p1 = new Esfera(posLuz2, 3, colorLuz2); // esfera luciernaga
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

    std::vector<Luz*> luces;
//    Luz luz(vec3(30,30,30), vec3(1,1,1));
//    luces.emplace_back(&luz);
    Luz *pluz = new Luz(posLuz1, colorLuz1);
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

    // Begin My Test 1
    /*
    cam.configurar(3, 60, 600, 800,
                   vec3(1, 120, 500),
                   vec3(0, 0, 0),
                   vec3(0, 1, 0));

    cam.renderizar(objetos, luces, 100);
    // Move firefly
    Esfera* firefly = static_cast<Esfera*>(objetos[3]);
    firefly->centro.x = -45;
    // Generar imagen
    cam.renderizar(objetos, luces, 101);

    firefly->centro.x = 45;
    // Generar imagen
    cam.renderizar(objetos, luces, 102);
    */
    // End My Test 1

    // Begin: test circular motion
    Esfera* myFirefly = static_cast<Esfera*>(objetos[2]);
    float r = 20; // used for X and Y axis
    float a = 15; // for Z axis [from 15; -15]

    Esfera* myFirefly2 = static_cast<Esfera*>(objetos[3]);

    std::cout << "\n Init myFirefly: x = " << myFirefly->centro.x << ", y = " << myFirefly->centro.y << ", z = " << myFirefly->centro.z << "\n";
    float t = 0;
    float numVueltas = 0;
    float startY = 30.0;
    for (int y = 1, n = 1; y < 100; y += 2, n++) {
        cam.configurar(3, 60, 600, 800,
                       vec3(1, 180, 400),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        // 1 vuelta
        if (t > 2 * M_PI) {
            std::cout << "una vuelta..\n";
//            numVueltas += 1;
//            break;
        }
        // count vueltas
        numVueltas = t / (2 * M_PI);
        myFirefly->centro.x = r * cos(t);
        myFirefly2->centro.x = r * cos(t);

        myFirefly->centro.y = r * sin(t);
        myFirefly->centro.y += startY;

        myFirefly->centro.z = a * cos(t);
        myFirefly2->centro.z = a * cos(t);
        std::cout << "\nmyFirefly: t = " <<  t << ", x = " << myFirefly->centro.x << ", y = " << myFirefly->centro.y << ", z = " << myFirefly->centro.z << "\n";
        std::cout << "\nmyFirefly2: t = " <<  t << ", x = " << myFirefly2->centro.x << ", y = " << myFirefly2->centro.y << ", z = " << myFirefly2->centro.z << "\n";
        cam.renderizar(objetos, luces, n);

        t += 0.2;
        std::cout << "#vueltas: " << numVueltas << "\n";
    }
    // End: test circular motion
}