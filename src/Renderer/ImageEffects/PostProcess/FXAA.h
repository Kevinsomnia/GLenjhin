#pragma once

#include "PostProcessEffect.h"

class FXAA : public PostProcessEffect
{
public:
    FXAA();
    ~FXAA();
    void lazyInitialize(Camera* camera);
};
