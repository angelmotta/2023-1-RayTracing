//
// Created on 30/03/2023.
//
#include <iostream>
#include "Camara.h"
#include "Luz.h"
#include <vector>

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
void Camara::renderizar() {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir;

    pImg = new cimg_library::CImg<BYTE>(w, h, 1, 10);
    cimg_library::CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");

    Esfera esf(vec3(2,0,0), 8, vec3(0,0,1));
    esf.kd = 0.8;

    std::vector<Objeto*> objetos;
    objetos.emplace_back(new Esfera(vec3(10,0,0), 8, vec3(0,0,1), 1));
    objetos.emplace_back(new Esfera(vec3(-10,0,0), 8, vec3(0,1,0), 0.8));
    objetos.emplace_back(new Esfera(vec3(0,10,0), 8, vec3(1,0,0), 0.6));

    Luz luz(vec3(10, 10, 10), vec3(1, 1, 1)); // Luz(posición, color)

    bool hay_interseccion;
    float t, t_tmp;
    vec3 color, normal, normal_tmp;
    Objeto *pObjeto = nullptr;

    for(int x=0;  x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;

            color.set(0,0,0);
            if (esf.intersectar(rayo, t, normal)) {
                color = esf.color;
                vec3 pi = rayo.ori + rayo.dir * t;
                vec3 L = luz.pos - pi;
                L.normalize();
                vec3 luz_ambiente = vec3(1,1,1) * 0.2;
                vec3 luz_difusa = vec3(0,0,0);
                float factor_difuso = normal.punto(L);
                if (factor_difuso > 0)
                    luz_difusa = luz.color * esf.kd * factor_difuso;
                color = esf.color * (luz_ambiente + luz_difusa);

            }

            // Direccion rayo intersecta esf
//            if (esf.intersectar(rayo, t) && !foundIntersec) {
//                std::cout << rayo.dir.x << ", "<< rayo.dir.y << ", " << rayo.dir.z << "\n";
//                auto piEsf = eye + (rayo.dir * t);
//                std::cout << piEsf.x << ", " << piEsf.y << ", " << piEsf.z << "\n";
//                foundIntersec = true;
//            }

            (*pImg)(x,h-1-y,0) = (BYTE)(color.x * 255);
            (*pImg)(x,h-1-y,1) = (BYTE)(color.y * 255);
            (*pImg)(x,h-1-y,2) = (BYTE)(color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    while (!dis_img.is_closed()) {
        dis_img.wait();
    }

}