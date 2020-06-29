#ifndef BEND_SEQ_GEN_H_
#define BEND_SEQ_GEN_H_

#include "../libfxtract/include/Model.h"

#include "GaIndividual.h"
#include <vector>

class Individual;

Individual generateBendingSeq(Fxt::Model& model, std::vector<int> initialSeq);

#endif