#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"

#include "Mesh.h"
#include "State.h"
#include "Shader.h"
#include "Material.h"

typedef struct {

    Mesh *mesh;

    State *state;

    Shader *shader;

    Material *mats;

} Object;

Object *newObject();
void freeObject(Object *obj);

#endif