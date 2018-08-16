//===--- AAP.h - AAP Tool and ToolChain Implementations ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_AAP_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_AAP_H

#include "Gnu.h"
#include "InputInfo.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {
namespace toolchains {

class LLVM_LIBRARY_VISIBILITY AAPToolChain : public Generic_ELF {
public:
  AAPToolChain(const Driver &D, const llvm::Triple &Triple,
               const llvm::opt::ArgList &Args);

protected:
  Tool *buildAssembler() const override;
  Tool *buildLinker() const override;

public:
  bool isPICDefault() const override { return false; }
  bool isPIEDefault() const override { return false; }
  bool isPICDefaultForced() const override { return true; }
  bool HasNativeLLVMSupport() const override { return true; }

  bool IsIntegratedAssemblerDefault() const override { return true; }
  bool SupportsProfiling() const override { return false; }
  bool hasBlocksRuntime() const override { return false; }

  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
  void addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                             llvm::opt::ArgStringList &CC1Args,
                             Action::OffloadKind) const override;
};

} // end namespace toolchains

namespace tools {
namespace AAP {

class LLVM_LIBRARY_VISIBILITY Assembler : public Tool {
public:
  Assembler(const ToolChain &TC) : Tool("AAP:Assembler", "aap-as", TC) {}
  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  Linker(const ToolChain &TC) : Tool("AAP::Linker", "aap-ld", TC) {}
  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};
} // end namespace AAP
} // end namespace tools
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_AAP_H
