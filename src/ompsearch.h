#pragma once
#include "eval.h"
#include "omp.h"

SearchResult ompSearch(__uint128_t occupied, std::vector<__uint128_t> pieces, float currEval,int depth,
  int turnPlayer);