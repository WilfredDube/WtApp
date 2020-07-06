#ifndef BEND_SEQ_GEN_H_
#define BEND_SEQ_GEN_H_

#include "../libfxtract/include/Model.h"

#include "Sequence.h"
#include <vector>

class Sequence;

Sequence generateBendingSeq(Fxt::Model& model, std::vector<int> initialSeq);

#endif