#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QOpenGLFunctions_4_5_Core>

#include "model.h"

namespace Hypercube {

class GemModelManager {
   public:
    GemModelManager(QOpenGLFunctions_4_5_Core* func);

    Model* GetModel(int type);

   private:
    std::vector<Model*> models_;
};

}  // namespace Hypercube

#endif  // MODELMANAGER_H
