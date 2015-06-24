#include "Util.h"
#include "Object.h"

Object *newObject() {

    Object *obj = alloc( Object, 1 );
    return obj;
}

void freeObject(Object *obj) {

    freeMesh(obj->mesh);
    freeState(obj->state);
    freeShader(obj->shader);
    freeMaterial(obj->mats);

    free(obj);
}
