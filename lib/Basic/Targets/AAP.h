//===--- AAP.h - Declare AAP target feature support -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares AAP TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_AAP_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_AAP_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Compiler.h"

namespace clang {
namespace targets {

// AAP Target
class LLVM_LIBRARY_VISIBILITY AAPTargetInfo : public TargetInfo {
public:
  AAPTargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    NoAsmVariants = true;
    TLSSupported = false;

    BoolWidth = HalfWidth = IntWidth = 16;
    PointerWidth = 16;
    LongWidth = 32;
    FloatWidth = 32;
    LongLongWidth = 64;
    DoubleWidth = 64;

    BoolAlign = HalfAlign = IntAlign = LongAlign = LongLongAlign = 16;
    PointerAlign = 16;
    FloatAlign = DoubleAlign = 16;
    SuitableAlign = 16;

    IntMaxType = Int64Type = SignedLongLong;
    IntPtrType = SignedInt;
    PtrDiffType = SignedInt;
    SizeType = UnsignedInt;

    resetDataLayout("e-m:e-p:16:16-i32:16-i64:16-f32:16-f64:16-n16");
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  ArrayRef<Builtin::Info> getTargetBuiltins() const override { return None; }

  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  const char *getClobbers() const override { return ""; }

  ArrayRef<const char *> getGCCRegNames() const override { return None; }

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    return None;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const override {
    return false;
  }
};
} // namespace targets
} // namespace clang

#endif // LLVM_CLANG_LIB_BASIC_TARGETS_AAP_H
