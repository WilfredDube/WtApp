#ifndef MODEL_APP_H_
#define MODEL_APP_H_

#include <string>

#include "ModelMath.h"
#include "Model.h"
#include "StepProcessor.h"
#include "IgesProcessor.h"
#include "ModelReader.h"

namespace Fxt {

  class Model;

  class ModelApp {
  public:
    ModelApp();
    ~ModelApp() { std::cout << "ModelApp Deleted...." << std::endl; }

    Model run(const char *modelFile);
    Model run(Model& test);
  };
  
}
#endif // MODEL_APP_H_
