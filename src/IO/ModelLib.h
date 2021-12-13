#ifndef MODEL_LIB_H
#define MODEL_LIB_H

#include <assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "../Renderer/Mesh.h"

using std::cerr;
using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::steady_clock;

namespace ModelLib
{
    struct Model
    {
        std::vector<Mesh*> meshes;

        void cleanup();
    };

    class ModelImporter
    {
    public:
        static void Init();
        static Model LoadFBX(const std::string& filePath);
    private:
        static Assimp::Importer* importerInstance;
    };
}

#endif