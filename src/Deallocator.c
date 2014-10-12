#include <stdlib.h>

#include "common.h"
#include "Deallocator.h"
#include "Debugger.h"

#define MAX_DEALLOCATEES 32

/*                buffer deallocatees
 * * * * * * * * * * * * * * * * * * * * * * * * * */
int nextBufferSlot = 0;
void* buffers[MAX_DEALLOCATEES];

void storeBuffer(void* buffer) {
    return_guard(nextBufferSlot < MAX_DEALLOCATEES);
    buffers[nextBufferSlot++] = buffer;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * */

/*                vbo deallocatees
 * * * * * * * * * * * * * * * * * * * * * * * * * */
int nextVboSlot = 0;
unsigned int vbos[MAX_DEALLOCATEES];

void storeVbo(unsigned int vbo) {
    vbos[nextVboSlot++] = vbo;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * */

/*                shader deallocatees
 * * * * * * * * * * * * * * * * * * * * * * * * * */
int nextShaderSlot = 0;
unsigned int shaders[MAX_DEALLOCATEES];

void storeShader(unsigned int shader) {
    shaders[nextShaderSlot++] = shader;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * */

/*               free deallocatees
 * * * * * * * * * * * * * * * * * * * * * * * * * */
void deallocStores() {
    void* freeMe;
    for (int i = 0; i < nextBufferSlot; ++i) {
        freeMe = buffers[i];

        /* meditate: does this even happen ?*/
        if (!freeMe) {
            err("Attempt to free NULL buffer: %p", freeMe);
            continue;
        }
        free(freeMe);
    }

    glDeleteBuffers(nextVboSlot, vbos);

    for (int i = 0; i < nextShaderSlot; i++) {
        glDeleteShader(shaders[i]);
    }

}

void printStores() {
    watch("[ Dealloc Stores ]\n");
    watch("buffers { ");
    for (int i = 0; i < nextBufferSlot; ++i) {
        watch("%p ", buffers[i]);
    }
    watch("}\n");
    watch("vbos { ");
    for (int i = 0; i < nextVboSlot; ++i) {
        watch("%d ", vbos[i]);
    }
    watch("}\n\n");
}