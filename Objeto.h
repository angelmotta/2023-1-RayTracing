//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_OBJETO_H
#define CG2023_OBJETO_H
#include "vec3.h"
#include "Rayo.h"

class Objeto {
public:
    vec3 color;
    float kd; // constante de difusion

    Objeto(vec3 col, float kd=1):color{col}, kd{kd}{}

    virtual bool intersectar(Rayo ray, float &t, vec3 &normal)=0;
};

class Esfera : public Objeto {
public:
    vec3 centro;
    float radio;

    Esfera(vec3 cen, float r, vec3 col, float kd=1): centro{cen}, radio{r}, Objeto(col, kd) {}

    bool intersectar(Rayo ray, float &t, vec3 &normal) {
        auto _a = ray.dir.punto(ray.dir);
        auto _b = 2*ray.dir.punto(ray.ori-centro);
        auto _c = (ray.ori-centro).punto(ray.ori-centro)-radio*radio;
        auto D = _b*_b-4*_a*_c;
        if(D <= 0) {return false;}  // no hay intersección
        float t1 = (-_b + sqrt(D))/2*_a;
        float t2 = (-_b - sqrt(D))/2*_a;
        t = std::fmin(t1, t2);  // la distancia mas corta de impacto
        if(t <= 0) {return false;}

        // Obtener vector normal del Punto de Intersección (pi)
        vec3 pi = ray.ori + ray.dir * t;
        normal = pi - centro;
        normal.normalize();
        return true;

    }
};


#endif //CG2023_OBJETO_H