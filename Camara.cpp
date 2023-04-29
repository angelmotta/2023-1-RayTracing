//
// Created on 30/03/2023.
//
#include <iostream>
#include "Camara.h"
#include "Luz.h"
#include <vector>
#include <iostream>

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

    p1 = new Esfera(vec3(10,0,20), 8, vec3(0.1,0.1,0.1));
    p1->setConstantes(0.8, 0.2);
    p1->ke = 0.9;
    p1->es_transparente = true;
    p1->ior = 1.5;
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(0,10,0), 8, vec3(1,0,0));
    p1->setConstantes(0.6, 0.4, 32);
    p1->ke = 0.8;
    p1->es_transparente = false;
    p1->ior = 1.2;
    objetos.emplace_back(p1);

    p1 = new Plano(vec3(0,1,0), 1, vec3(0.123, 0.456, 0.789));
    p1->setConstantes(0.9, 0.1);
    p1->ke = 0.5;
    objetos.emplace_back(p1);

    p1 = new Cilindro(vec3(-10, 0, 0), vec3(-10, 10, 0), 5, vec3(0, 1, 1));
    p1->setConstantes(0.7, 0.3, 8);
    objetos.emplace_back(p1);
    // Begin Generate Random spheres
    /*
    srand (time(NULL));
    int magicUnit;
    for (int i = 0; i < 50; i++) {
        auto color = vec3((rand() % 100 + 1)/100.0, (rand() % 100 + 1)/100.0, (rand() % 100 + 1)/100.0);
        color.max_to_one();
        int coin = rand() % 2; // coin: [0, 1]

        if (coin == 1) {
            magicUnit = -1 * coin; // set magicUnit = -1
        } else {
            // if coin is 0 set magicUnit to 1
            magicUnit = 1;
        }
        p1 = new Esfera(vec3((rand() % 50)*magicUnit,(rand() % 30)-20,(rand() % 70)-50), (rand() % 6)+2, color);
        p1->setConstantes(0.8, 0.8);
        p1->ke = 0.8;
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
    float t_tmp, t = 1000000000;
    bool update_pObjeto = false;
    int idxObj = 0;

    for (auto pObj : objetos) {
        if (pObj->intersectar(rayo, t_tmp, normal_tmp)) {
            hay_interseccion = true;
            if (t_tmp < t) {
                t = t_tmp;
                normal = normal_tmp;
                pObjeto = pObj;
                update_pObjeto = true;
            }
        }
        idxObj++;
    }

    if (hay_interseccion and pObjeto->es_luz) {
        color = pObjeto->color;
    } else if (hay_interseccion) {
        vec3 pi = rayo.ori + rayo.dir * t; // pi: Punto de Interseccion
        vec3 V = -rayo.dir;
        if (pObjeto->es_transparente) {
            // kr kt
            vec3 refractionColor(0, 0, 0);
            // compute fresnel
            float kr;
            fresnel(rayo.dir, normal, pObjeto->ior, kr);
            bool outside = rayo.dir.punto(normal) < 0;
            vec3 bias = 0.005 * normal;
            // Compute refraction if it is not a case of total internal reflection
            if (kr < 1) {
                vec3 refractionDirection = refract(rayo.dir, normal, pObjeto->ior);
                refractionDirection.normalize();
                vec3 refractionRayOrig = outside ? pi - bias : pi + bias;
                Rayo rayo_refraccion(refractionRayOrig, refractionDirection);
                refractionColor = calcular_color(rayo_refraccion, objetos, luces, prof + 1);
            }

            vec3 reflectionDirection = 2 * (V.punto(normal)) * normal - V; // reflect(dir, normal).normalize();
            reflectionDirection.normalize();
            vec3 reflectionRayOrig = outside ? pi + bias : pi - bias;
            Rayo rayo_reflexivo(reflectionRayOrig, reflectionDirection);
            vec3 reflectionColor = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);

            // mix the two
            color = reflectionColor * kr + refractionColor * (1 - kr);
        } else {
            // Verficar si es posible continuar con la recursion
            if (pObjeto->ke > 0 and prof + 1 <= prof_max) {
                // rayos reflexivos
                Rayo rayo_reflexivo;
                rayo_reflexivo.ori = pi + 0.0005 * normal;
                rayo_reflexivo.dir = 2 * (V.punto(normal)) * normal - V;
                rayo_reflexivo.dir.normalize();
                // Recursive call
                vec3 color_reflexivo = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);
                color = pObjeto->ke * color_reflexivo;
            }
        }

        vec3 L = luces[0]->pos - pi;
        float distancia = L.modulo();
        L.normalize();
        vec3 luz_ambiente = vec3(1,1,1) * 0.2;
        // Determinar si hay sombra
        bool hay_sombra = false;
        Rayo rayo_sombra(pi + 0.0005 * normal, L);    // rayo en direccion hacia la luz
        for (auto pObj : objetos) {
            if ((not pObj->es_luz)
            and (not pObj->es_transparente)
            and pObj->intersectar(rayo_sombra, t_tmp, normal_tmp)
            and t_tmp < distancia
            ) {
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
            float factor_especular = R.punto(V);
            if (factor_especular > 0) {
                luz_especular = luces[0]->color * pObjeto->ks * pow(factor_especular, pObjeto->n);
            }
            color = color + pObjeto->color * (luz_ambiente + luz_difusa + luz_especular);
            color.max_to_one();
        } else {
            // Hay sombra
            color = color + pObjeto->color * (luz_ambiente);    // poner sombra
            color.max_to_one();
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

vec3 Camara::refract(vec3 &I, vec3 &N, float &ior) {
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    vec3 n = N;
    if (cosi < 0) {
        cosi = -cosi;
    } else {
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3(0, 0, 0) : eta * I + (eta * cosi - sqrt(k)) * n;
}

void Camara::fresnel(vec3 &I, vec3 &N, float &ior, float &kr) {
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    if (cosi > 0) {
        std::swap(etai, etat);
    }
    // Compute sini using Snell's Law
    float sint = etai / etat * sqrt(std::max(0.f, 1 - cosi * cosi));
    // Total Internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
}

void Camara::renderizar(std::vector<Objeto*> &objetos, std::vector<Luz*> &luces, int num) {
    pImg = new cimg_library::CImg<BYTE>(w, h, 1, 10);
    cimg_library::CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde Camara Pinhole");
    Rayo rayo(eye);
    vec3 color, dir;
    for(int x = 0;  x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (x == 190 and y == h - 398) {
                std::cout << "stop";
            }
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;
            color = calcular_color(rayo, objetos, luces, 1);

            (*pImg)(x,h - 1 - y,0) = (BYTE)(color.x * 255);
            (*pImg)(x,h - 1 - y,1) = (BYTE)(color.y * 255);
            (*pImg)(x,h - 1 - y,2) = (BYTE)(color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    std::string nombre_archivo = "imagen" + std::to_string(num) + ".bmp";
    pImg->save(nombre_archivo.c_str());
}