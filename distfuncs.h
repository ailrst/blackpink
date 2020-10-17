#ifndef DISTFU
#define DISTFU

struct object new_sphere(struct vec *, double);
struct object new_object(struct vec *, double, double, double (*) (struct vec *),
        struct colour (*)(struct ray *, struct object *));

double sdf_box(struct vec *);
double sdf_hplane(struct vec *);
double sdf_phat_vert_line(struct vec *);
double sdf_sphere(struct vec *);

struct colour yeet_col(struct ray *);

#endif
