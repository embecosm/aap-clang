//===--- AAP.cpp - Implement AAP target feature support -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements AAP TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "AAP.h"
#include "clang/Basic/MacroBuilder.h"

using namespace clang;
using namespace clang::targets;

void AAPTargetInfo::getTargetDefines(const LangOptions &Opts,
                                     MacroBuilder &Builder) const {
  Builder.defineMacro("__ELF__");
  Builder.defineMacro("__AAP__");
}
