/*
 * This file is part of hipSYCL, a SYCL implementation based on CUDA/HIP
 *
 * Copyright (c) 2021 Aksel Alpay and contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HIPSYCL_LOOPSPLITTER_HPP
#define HIPSYCL_LOOPSPLITTER_HPP

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"

namespace hipsycl {
namespace compiler {

class LoopSplitAtBarrierPassLegacy : public llvm::LoopPass {
  bool IsO0_;

public:
  static char ID;

  explicit LoopSplitAtBarrierPassLegacy(bool IsO0) : llvm::LoopPass(ID), IsO0_(IsO0) {}

  llvm::StringRef getPassName() const override { return "hipSYCL loop splitting pass"; }

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  bool runOnLoop(llvm::Loop *L, llvm::LPPassManager &LPM) override;
};

class LoopSplitAtBarrierPass : public llvm::PassInfoMixin<LoopSplitAtBarrierPass> {
  bool IsO0_;

public:
  explicit LoopSplitAtBarrierPass(bool IsO0) : IsO0_(IsO0) {}
  
  llvm::PreservedAnalyses run(llvm::Loop &L, llvm::LoopAnalysisManager &AM, llvm::LoopStandardAnalysisResults &AR,
                              llvm::LPMUpdater &LPMU);
  static bool isRequired() { return true; }
};
} // namespace compiler
} // namespace hipsycl

#endif