#ifndef GLOBAL_FOG_H
#define GLOBAL_FOG_H

#include "../ImageEffect.h"

class GlobalFog : public ImageEffect
{
public:
    GlobalFog();
    ~GlobalFog();
    void render(BufferTexture* source, BufferTexture* destination) override;
};

#endif // GLOBAL_FOG_H
