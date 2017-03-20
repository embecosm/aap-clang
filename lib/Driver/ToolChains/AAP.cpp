//===--- AAP.cpp - AAP ToolChain Implementations ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "AAP.h"
#include "CommonArgs.h"
#include "InputInfo.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/ArgList.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

/// AAP Toolchain
AAPToolChain::AAPToolChain(const Driver &D, const llvm::Triple &Triple,
                           const ArgList &Args)
    : Generic_ELF(D, Triple, Args) {}

Tool *AAPToolChain::buildAssembler() const {
  return new tools::AAP::Assembler(*this);
}

Tool *AAPToolChain::buildLinker() const {
  return new tools::AAP::Linker(*this);
}

void AAPToolChain::AddClangSystemIncludeArgs(
    const llvm::opt::ArgList &DriverArgs,
    llvm::opt::ArgStringList &CC1Args) const {
  const Driver &D = getDriver();

  if (DriverArgs.hasArg(options::OPT_nostdinc) ||
      DriverArgs.hasArg(options::OPT_nostdlibinc)) {
    return;
  }

  // standard system includes are disabled, so we add our own
  const std::string InstallPrefix = D.InstalledDir;
  const std::string IncludeDir = InstallPrefix + "/../aap/include";
  StringRef IncludeDirStr(IncludeDir);

  addSystemIncludes(DriverArgs, CC1Args, IncludeDirStr);
}

void AAPToolChain::addClangTargetOptions(
    const llvm::opt::ArgList &DriverArgs,
    llvm::opt::ArgStringList &CC1Args) const {
  CC1Args.push_back("-nostdsysteminc");
  CC1Args.push_back("-dwarf-column-info");
}

void AAP::Assembler::ConstructJob(Compilation &C, const JobAction &JA,
                                  const InputInfo &Output,
                                  const InputInfoList &Inputs,
                                  const ArgList &Args,
                                  const char *LinkingOutput) const {
  ArgStringList CmdArgs;

  // Add input assembly files to command line
  for (InputInfoList::const_iterator it = Inputs.begin(), ie = Inputs.end();
       it != ie; ++it) {
    const InputInfo &II = *it;
    CmdArgs.push_back(II.getFilename());
  }

  const char *Exec =
      Args.MakeArgString(getToolChain().GetProgramPath("aap-as"));

  C.addCommand(llvm::make_unique<Command>(JA, *this, Exec, CmdArgs, Inputs));
}

void AAP::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs, const ArgList &Args,
                               const char *LinkingOutput) const {
  ArgStringList CmdArgs;

  // Add crt0 and libc
  const toolchains::AAPToolChain &ToolChain =
      static_cast<const toolchains::AAPToolChain &>(getToolChain());
  const Driver &D = ToolChain.getDriver();

  const std::string InstallPrefix = D.InstalledDir;
  const std::string LibFilesDir = InstallPrefix + "/../aap/lib";
  const std::string crt0 = LibFilesDir + "/crt0.o";
  const std::string libpath = "-L" + LibFilesDir;

  if (!Args.hasArg(options::OPT_nostdlib) &&
      !Args.hasArg(options::OPT_nostartfiles)) {
    CmdArgs.push_back(Args.MakeArgString(crt0));
  }
  AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

  CmdArgs.push_back(Args.MakeArgString(libpath));
  if (!Args.hasArg(options::OPT_nostdlib)) {
    CmdArgs.push_back("-lc");
    CmdArgs.push_back("-laap");
    CmdArgs.push_back("-lcompiler_rt");

    // This may need to link a second time to resolve interdependencies
  }

  Args.AddAllArgs(CmdArgs, options::OPT_L);

  if (Output.isFilename()) {
    CmdArgs.push_back("-o");
    CmdArgs.push_back(Output.getFilename());
  } else {
    assert(Output.isNothing() && "Input output");
  }

  const char *Exec =
      Args.MakeArgString(getToolChain().GetProgramPath("aap-ld"));
  C.addCommand(llvm::make_unique<Command>(JA, *this, Exec, CmdArgs, Inputs));
}
