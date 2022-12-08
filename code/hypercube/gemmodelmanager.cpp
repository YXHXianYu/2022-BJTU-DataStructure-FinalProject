#include "gemmodelmanager.h"

namespace Hypercube {

GemModelManager::GemModelManager(QOpenGLFunctions_4_5_Core* func) {
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_2.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_3.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_4.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_5.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_6.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_7.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_8.obj"));
}

Model* GemModelManager::GetModel(int type) { return models_[type]; }

}  // namespace Hypercube
