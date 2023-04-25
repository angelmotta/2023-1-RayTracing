//
// Created on 30/03/2023.
//
#include <iostream>
#include "Camara.h"
#include "Luz.h"
#include <vector>
#include <time.h>

void Camara::configurar(float _near, float fov, int ancho, int alto,
                vec3 pos_eye, vec3 center, vec3 up) {
    f = _near;
    w = ancho;
    h = alto;
    a = 2 * f * tan(fov * M_PI/360);
    b = w / h * a;
    eye = pos_eye;
    ze = eye - center;
    ze.normalize();
    xe = up.cruz(ze);
    xe.normalize();
    ye = ze.cruz(xe);
}
void Camara::renderizar(int num) {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir;

    pImg = new cimg_library::CImg<BYTE>(w, h, 1, 10);
    cimg_library::CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");

    Esfera esf(vec3(2,0,0), 8, vec3(0,0,1));
    esf.kd = 0.8;

    std::vector<Objeto*> objetos;
    Objeto *p1;

    p1 = new Esfera(vec3(10,0,0), 8, vec3(0,0,1));
    p1->setConstantes(1, 0); // (kd, ks)
    p1->ke = 1; // cte espejo (cte reflexion)
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(-10,-10,-10), 8, vec3(0,1,0));
    p1->setConstantes(0.8, 0.2);
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(0,10,0), 1, vec3(0.123,0.456,0.789));
    p1->setConstantes(0.9, 0.1);
    objetos.emplace_back(p1);

    p1 = new Plano(vec3(0,1,0), 1, vec3(0.123, 0.456, 0.789));
    p1->setConstantes(0.9, 0.1);
    p1->ke = 0.5;
    objetos.emplace_back(p1);

    // Generate Random spheres
    /*
    srand (time(NULL));
    int magicUnit;
    for (int i = 0; i < 50; i++) {
        auto color = vec3((rand() % 100 + 1)/100.0, (rand() % 100 + 1)/100.0, (rand() % 100 + 1)/100.0);
        color.max_to_one();
        int coin = rand() % 2;

        if (coin == 1) {
            magicUnit = -1 * coin;
        } else {
            // if coin is 0 set magicUnit to 1
            magicUnit = 1;
        }
        p1 = new Esfera(vec3((rand() % 50)*magicUnit,(rand() % 30)-20,(rand() % 70)-50), (rand() % 6)+2, color);
        p1->setConstantes(0.8, 0.8, 8);
        objetos.emplace_back(p1);
    }
    */
    // End Generate Random Spheres

    Luz luz(vec3(30, 30, 30), vec3(1, 1, 1)); // Luz(posición, color)
    std::vector<Luz*> luces;
    luces.emplace_back(&luz);

    vec3 color;
    for(int x=0;  x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;

            // Calcular Color
            color = calcular_color(rayo, objetos, luces, 1);

            (*pImg)(x,h-1-y,0) = (BYTE)(color.x * 255);
            (*pImg)(x,h-1-y,1) = (BYTE)(color.y * 255);
            (*pImg)(x,h-1-y,2) = (BYTE)(color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    /*
    while (!dis_img.is_closed()) {
        dis_img.wait();
    }
    */
    std::string nombre_archivo = "image" + std::to_string(num) + ".bmp";
    pImg->save(nombre_archivo.c_str());
}

vec3 Camara::calcular_color(Rayo rayo, std::vector<Objeto*> objetos, std::vector<Luz*> luces, int prof) {
    vec3 color(0,0,0);
    vec3 normal, normal_tmp;
    Objeto *pObjeto = nullptr;
    bool hay_interseccion = false;
    float t_tmp;
    float t = 1000000000;

    for (auto pObj : objetos) {
        if (pObj->intersectar(rayo, t_tmp, normal_tmp)) {
            hay_interseccion = true;
            if (t_tmp < t) {
                t = t_tmp;
                normal = normal_tmp;
                pObjeto = pObj;
            }
        }
    }

    if (hay_interseccion) {
        vec3 pi = rayo.ori + rayo.dir * t; // pi: Punto de Interseccion
        vec3 L = luces[0]->pos - pi;
        L.normalize();
        vec3 luz_ambiente = vec3(1,1,1) * 0.2;
        // Determinar si hay sombra
        bool hay_sombra = false;
        Rayo rayo_sombra;
        rayo_sombra.ori = pi + 0.0005 * normal;
        rayo_sombra.dir = L;    // rayo en direccion hacia la luz
        for (auto pObj : objetos) {
            if (pObj->intersectar(rayo_sombra, t_tmp, normal_tmp)) {
                hay_sombra = true;
            }
        }
        // End Determinar si hay sombra
        if (!hay_sombra) {
            vec3 luz_difusa = vec3(0,0,0);
            float factor_difuso = normal.punto(L);
            if (factor_difuso > 0) {
                luz_difusa = luces[0]->color * pObjeto->kd * factor_difuso;
            }

            // Luz especular
            vec3 luz_especular = vec3(0, 0, 0);
            vec3 R = 2 * (L.punto(normal)) * normal - L;
            vec3 V = -rayo.dir;
            float factor_especular = R.punto(V);
            if (factor_especular > 0) {
                luz_especular = luces[0]->color * pObjeto->ks * pow(factor_especular, pObjeto->n);
            }
            color = pObjeto->color * (luz_ambiente + luz_difusa + luz_especular);

            // Verficar si es posible continuar con la recursion
            if (pObjeto->ke > 0 and prof + 1 <= prof_max) {
                // rayos reflexivos
                Rayo rayo_reflexivo;
                rayo_reflexivo.ori = pi + 0.0005 * normal;
                rayo_reflexivo.dir = 2 * (V.punto(normal)) * normal - V;
                // Recursive call
                vec3 color_reflexivo = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);
                color = color + color_reflexivo;
            }

            color.max_to_one();
        } else {
            // Hay sombra
            color = pObjeto->color * (luz_ambiente);    // poner sombra
        }
    }
    return color;
    // Direccion donde el rayo intersecta en esfera
//            if (esf.intersectar(rayo, t) && !foundIntersec) {
//                std::cout << rayo.dir.x << ", "<< rayo.dir.y << ", " << rayo.dir.z << "\n";
//                auto piEsf = eye + (rayo.dir * t);
//                std::cout << piEsf.x << ", " << piEsf.y << ", " << piEsf.z << "\n";
//                foundIntersec = true;
//            }
}