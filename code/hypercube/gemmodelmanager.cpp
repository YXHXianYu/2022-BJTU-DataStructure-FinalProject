#include "gemmodelmanager.h"

namespace Hypercube {

GemModelManager::GemModelManager(QOpenGLFunctions_4_5_Core* func) {
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_2.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_3.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_4.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_5.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_6.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_7.obj"));
    models_.push_back(new Model(func, "../2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_8.obj"));
}

Model* GemModelManager::GetModel(int type) {
    if (type >= models_.size()) {
        return nullptr;
    }
    return models_[type];
}

}  // namespace Hypercube
