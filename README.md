# 2023-1 Proyecto RayTracing: Luciernagas

# Simulación
![](https://github.com/angelmotta/2023-1-RayTracing/blob/main/simulacion/proyecto-740-v2_AngelMotta.gif)

# Descripción:
1. Movimiento de las luciérnagas\
Para modelar este movimiento se utilizó las siguientes ecuaciones paramétricas con la cual 2 luciérnagas realizan un movimiento circular y en el plano XZ y otras 2 realizan un movimiento en el eje Y tal como se muestra en la animación.\
x = 2 cos(t); 
y = 2 sin(t);
z = cos(t)

2. Movimiento de la cámara\
   La cámara realiza un acercamiento hacia el origen de 50 unidades en el eje Z (desde 425 hasta 375) durante los primeros 100 frames.
   Posteriormente la cámara se mantiene estática durante 100 frames y luego empieza a elevarse moviéndose en el eje Y 150 unidades (desde 30 hasta 180)
   Finalmente durante los últimos 240 frames se desplaza 120 unidades hacia la derecha en el X. ([Ver Aqui](https://github.com/angelmotta/2023-1-RayTracing/blob/8da7f54661629aba0893d3a736f24d7329b3f68d/main.cpp#L229)).


3. Generación de esferas aleatorias.\
   Se implementó una función de generación de esferas la cual recibe como parámetro el número de esferas a generar ([Ver Aquí](https://github.com/angelmotta/2023-1-RayTracing/blob/8da7f54661629aba0893d3a736f24d7329b3f68d/main.cpp#L256)).
Estas esferas toman valores aleatorios tanto para los colores como para el radio con un rango entre 3 y 9 unidades. Adicionalmente cada esfera se posiciona de forma aleatoria a una distancia de al menos 50 unidades en el eje X ya sea en dirección negativa o positiva y cada esfera se posiciona cada vez más alejado del origen en el Z partiendo desde -60 para lograr un nivel de uniformidad en la imágen.