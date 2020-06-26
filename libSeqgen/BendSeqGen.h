#ifndef BEND_SEQ_GEN_H_
#define BEND_SEQ_GEN_H_

#include "../libfxtract/include/Model.h"

#include <vector>

std::vector<int> generateBendingSeq(Fxt::Model& model, std::vector<int> initialSeq);

#endif