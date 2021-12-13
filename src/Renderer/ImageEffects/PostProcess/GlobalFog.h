#pragma once

#include "PostProcessEffect.h"

class GlobalFog : public PostProcessEffect
{
public:
    GlobalFog();
    ~GlobalFog();
    void render(BufferTexture* source, BufferTexture* destination) override;
};
