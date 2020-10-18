#include "main.h"
#include "types.h"
#include "vect.h"
#include "camera.h"

#define SOFT_LIGHT 1
#define HARD_LIGHT 2
#define WHITE_LIGHT 2

double divid_fp(double a, double b) {
    return a / b;
}

/*
 *
 * float sdEllipsoid( vec3 p, vec3 r )
{
  float k0 = length(p/r);
  float k1 = length(p/(r*r));
  return k0*(k0-1.0)/k1;
}
 *
 */
double sdf_4ellipsoid(struct vec *x) {
    double r = 1;

    struct vec *shape = new_vec4(1.5,3,1,4);
    struct vec *v = copy_vec(x);

    double dim = 0;
    for (int i = 0 ; i < v->dimension; i ++) {
        v->elements[i] = v->elements[i] / shape->elements[i];
    }

    double k0 = magnitude_vec(v);
    free_vec(v);
    v = copy_vec(x);
    for (int i = 0 ; i < v->dimension; i ++) {
        v->elements[i] = v->elements[i] / pow(shape->elements[i], 2);
    }
    double k1 = magnitude_vec(v);

    double result = k0 * (k0 - 1.0) / k1;
    free_vec(v);
    return result;

}


double sdf_3ellipsoid(struct vec *x) {
    double r = 1;

    struct vec *shape = new_vec3(1.5,3,1);
    struct vec *v = copy_vec(x);

    double dim = 0;
    for (int i = 0 ; i < 3; i ++) {
        v->elements[i] = v->elements[i] / shape->elements[i];
    }

    double k0 = magnitude_vec(v);
    free_vec(v);
    v = copy_vec(x);
    for (int i = 0 ; i < 3; i ++) {
        v->elements[i] = v->elements[i] / pow(shape->elements[i], 2);
    }
    double k1 = magnitude_vec(v);

    double result = k0 * (k0 - 1.0) / k1;
    free_vec(v);
    return result;
}


double sdf_sphere(struct vec *x) {
    static const double r = 1.4;
    struct vec *v = copy_vec(x);
    
    // v->elements[2] -= 5;
//    v->elements[1] += (SDL_GetTicks()/1000.0) - 5;
    // v->elements[2] -= 2;
   // v->elements[1] += (SDL_GetTicks()/1000.0) - 5;
    
    double res = magnitude_vec(v) - r;
    free_vec(v);

    return res;
}

double sdf_hplane(struct vec *x) {
    static const double h = -0.5;
    return -(x->elements[1] + h);
}

double clamp(double val, double min, double max) {
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

/*
double sdf_cone(struct vec *x) {
    double height = 3;
    struct vec *v = copy_vec(x);
    double cx = 1;
    double cy = 1;
    static struct vec *cone = NULL;
    static struct vec *w, *a, *b;

    if (!cone) {
        cone = scalar_multiply_vec_ip(new_vec2(cx/cy, -1), height); 
    }
    struct vec *temp;

    double cone2 = 

    struct vec * temp = new_vec2(v->e->x, v->e->z);
    double aaa = magnitude_vec(temp);
    free_vec(temp);
    w = new_vec2(aaa, v->e->y);

    double t2 = clamp((dot_product_vec(w, v) / dot_product_vec(cone, cone)), 0.0, 1.0);
    temp = scalar_multiply_vec(cone, t2);
    a = subtract_vec(w, temp);
    free_vec(temp);

    temp = new_vec2(clamp(w->e->x / cone->e->x, 0.0, 1.0), 1.0);
    scalar_multiply_vec_ip(temp, )



    

    double result = -1;
    free_vec(v);
    return result;
}
        */

double is_pos(double a) {
    if (a > 0) {
        return a;
    }
    return 0;
}

double sdf_phat_vert_line(struct vec *x) {
    static const double h = 2;
    static const double r = 0.5;

    struct vec *v = copy_vec(x);
    
    v->e->y -= clamp(v->e->y, 0.0, h);
    double val = magnitude_vec(v) - r;
    free_vec(v);
    return val;
}

/* https://www.alanzucconi.com/2016/07/01/signed-distance-functions/#part3 */
/* http://mercury.sexy/hg_sdf/ */
double sdf_box(struct vec *x) {

    struct vec *v = copy_vec(x);
    do_on_vec_ip(v, fabs);

    struct vec *  box_shape = new_vec_of(v->dimension, 5);

    subtract_vec_ip(box_shape, v);
    do_on_vec_ip(v, is_pos);
    
    double result = magnitude_vec(v);

    free_vec(v);
    free(box_shape);

    return result;
}


struct colour yeet_pho(struct ray *ray, struct object *o) {
    double specular = 0.8;
    double diffuse = 0.2;
    double ambient = 0.0;
    double shin = 50;

    int light_type = SOFT_LIGHT;

    struct colour light_col = {.r = 200, .g = 0, .b = 100, .sp = CS_RGB};
    struct vec *light = new_vec4(-1, 1, 1, 0);
    //struct vec *colour = new_vec3(o->base_col.r, o->base_col.g, o->base_col.b);

    /* ambient */
    //scalar_multiply_vec_ip(colour, ambient);
    double intensity = ambient;

    struct vec *surf_norm = estimateNormal(ray->pos, &o->sol);
//    struct vec *light_vec = normalise_vec_ip(subtract_vec(ray->pos, light));
    struct vec *light_vec = normalise_vec_ip(subtract_vec_ip(scalar_multiply_vec(light, -1), ray->pos));

    double diffuse_val = dot_product_vec(light_vec, surf_norm) * diffuse;

    
    // r = 2 * (l . n) * n - L

    struct vec *camera_vec = normalise_vec_ip(scalar_multiply_vec(ray->pos, -1));
    struct vec *reflection =  scalar_multiply_vec(surf_norm, 
            2 * dot_product_vec(light_vec, surf_norm));

    subtract_vec_ip(reflection, light_vec);
    double spec_val = pow(clamp(dot_product_vec(reflection, camera_vec),-1,1), shin) * specular;

    struct colour c = get_hsl(o->base_col);
    intensity = ambient + diffuse_val + spec_val;
    light_col = get_hsl(light_col);
    light_col.l = spec_val;
    light_col = get_rgb(light_col);
    c.l = intensity;
    c = get_rgb(c);
    if (1 && spec_val > 0.0) {
        if (light_type == HARD_LIGHT) {
            c.r = light_col.r > c.r ? c.r + (light_col.r - c.r) / 2 : c.r;
            c.g = light_col.g > c.g ? c.g + (light_col.g - c.g) / 2 : c.g;
            c.b = light_col.b > c.b ? c.b + (light_col.b - c.b) / 2 : c.b;
        } else if (light_type == SOFT_LIGHT) {
            c.r = light_col.r > c.r ? light_col.r: c.r;
            c.b = light_col.b > c.b ? light_col.b: c.b;
            c.g = light_col.g > c.g ? light_col.g: c.g;
        }
    }

    free_vec(camera_vec);
    free_vec(reflection);
    free_vec(surf_norm);
    free_vec(light_vec);
    free_vec(light);

    return c;
}




struct colour yeet_col_og(struct ray *ray, struct object *obj) {
    struct vec *l = new_vec4(1,1,1,1);
    struct vec *n = subtract_vec_ip(l, ray->pos);
    struct vec *nl = normalise_vec(l);

    struct colour c = {.r = nl->e->x * 555, .g = nl->e->y * 555, .b = nl->e->z * 555, .a = 255, .sp=CS_RGB};

    free_vec(n);
    return (c);
}

/*
struct object new_sphere(double radius) {
    struct object s;
    
    struct vec * v = new_vec4(0,0,5,0);
    s.base_col = get_random_color();
    s.sol.pos = *v;
    s.sol.op = B_ADD;
    s.col = yeet_pho;
    s.sol.dist = sdf_sphere;


    return s;

*/


/**
 * Creates a new struct object with the given parameters. The position passed
 * in should be a vec4 as this is converted to a vec3 when the object is added
 * to the scene.
 */
struct object
new_object(struct vec* position, double rotation, double scale, 
        double (*dist)(struct vec*), struct colour (*col)(struct ray *, struct object *)) 
{
    struct object new_obj;
    struct colour default_col = {.r = 255, .g=255, .b=255, .a=255, .sp=CS_RGB};
    new_obj.base_col = default_col;

    new_obj.col = col;
    new_obj.sol.dist = dist;
    new_obj.sol.pos = *position;
    new_obj.sol.rotation = rotation;
    new_obj.sol.scale = scale;
    new_obj.sol.op = B_ADD;


    return new_obj;
}

struct object new_sphere(struct vec* position, double rotation, double scale) {
    return new_object(position, rotation, scale, sdf_sphere, yeet_pho);
}

struct object new_ellipse(struct vec* position, double rotation, double scale) {
    return new_object(position, rotation, scale, sdf_3ellipsoid, yeet_pho);
}


struct object new_box(struct vec* position, double rotation, double scale) {
    return new_object(position, rotation, scale, sdf_box, yeet_pho);
}

/* DON'T CALL THIS */
struct object new_cone(struct vec* position, double rotation, double scale) {
    return new_object(position, rotation, scale, NULL, NULL);
}

struct object new_vert_line(struct vec* position, double rotation, double scale) {
    return new_object(position, rotation, scale, sdf_phat_vert_line, yeet_pho);
}
