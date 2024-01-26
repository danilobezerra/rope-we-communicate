#ifndef MAKE_ME_LAUGH_ENTITY_H
#define MAKE_ME_LAUGH_ENTITY_H

#include <genesis.h>

void Entity_translate(V2f16 *const position, const V2f16 *const translation);
Box Entity_bounding_box(const V2f16 *position, const V2u16 size);
void Entity_draw(const Box *const bb, const u16 color);

#endif //MAKE_ME_LAUGH_ENTITY_H
