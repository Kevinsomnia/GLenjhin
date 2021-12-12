#ifndef GLOBAL_FOG_H
#define GLOBAL_FOG_H

#include "PostProcessEffect.h"

class GlobalFog : public PostProcessEffect
{
public:
    GlobalFog();
    ~GlobalFog();
    void render(BufferTexture* source, BufferTexture* destination) override;
};

#endif // GLOBAL_FOG_H
