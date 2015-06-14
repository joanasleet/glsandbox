#include "Object.h"

Object *newObject() {

    Object *obj = NEW(Object);
    return obj;
}

void freeObject(Object *obj) {

    freeMesh(obj->mesh);
    freeState(obj->state);
    freeShader(obj->shader);
    freeMaterial(obj->mats);

    free(obj);
}
