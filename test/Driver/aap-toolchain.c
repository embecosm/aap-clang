// A basic clang -cc1 command-line check.

// RUN: %clang %s -### -no-canonical-prefixes -target aap 2>&1 | FileCheck -check-prefix=CC1 %s
// CC1: clang{{.*}} "-cc1" "-triple" "aap"
