
#define MACH_PRE_MACOS14_GNU        0   // also default when OS not macos (i.e ubuntu or windows)
#define MACH_PRE_MACOS14_CLANG      1
#define MACH_MACOS14_ARM_GNU        2
#define MACH_MACOS14_ARM_CLANG      3
#define MACH_MACOS15_ARM_GNU        4
#define MACH_MACOS15_ARM_CLANG      5

#define UBUNTU_X86_64  0
#define UBUNTU_ARM64   1

// https://sourceforge.net/p/predef/wiki/Architectures/
// https://sourceforge.net/p/predef/wiki/OperatingSystems/
// https://web.archive.org/web/20191012035921/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
  const unsigned indexOS = 2;
  #define  TEMPLATE_UID  103
  #define MACH_MACOS_ARRAY_IDX     MACH_PRE_MACOS14_GNU
  #define UBUNTU_ARRAY_IDX UBUNTU_X86_64
  #if defined(COMPILER_GNU) && \
      ( defined(BUILD_ENV_MINGW) || defined(BUILD_ENV_MSYS) || defined(BUILD_ENV_CYGWIN) )
    // bash environment on windows while using compiler MSVC and ClangCL, defines MSYSTEM=MINGW64
    // Hence we consider MSYSTEM value only when COMPILER=GNU
    #if defined(MSYSTEM_MINGW64) || defined(MSYSTEM_UCRT64) || defined(MSYSTEM_CLANG64) || defined(MSYSTEM_CLANGARM64)
      #define BUILD_ENV_MSYS2_GNU
    #else
      #error "Not a hosted implementation : Windows with MSYSTEM not being set"
    #endif
  #else
    #define BUILD_ENV_WINDOWS_MSVC_OR_CLANG
  #endif

#elif defined(__APPLE__) && defined(__MACH__)
  const unsigned indexOS = 1;
  #if defined(__clang_major__)
    // when compiler is AppleClang.
    #define  TEMPLATE_UID  3
    #define  MACH_COMPILER 1
  #else
    // when compiler is GNU.
    // macOS does not support 'std::from_chars()' and 'std::to_chars()'.
    // The fall back functions induces variations in results when compared to other OS's.
    #define  TEMPLATE_UID  103
    #define  MACH_COMPILER 0
  #endif

  #if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && \
      __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 140000
    #if    __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ == 140000
      #define MACH_MACOS_ARRAY_IDX     (MACH_MACOS14_ARM_GNU + MACH_COMPILER)
    #else //if  __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ == 150000
      #define MACH_MACOS_ARRAY_IDX     (MACH_MACOS15_ARM_GNU + MACH_COMPILER)
    #endif
  #else
    #define MACH_MACOS_ARRAY_IDX     (MACH_PRE_MACOS14_GNU + MACH_COMPILER)
  #endif
  #define UBUNTU_ARRAY_IDX UBUNTU_X86_64

#elif defined(__linux__) // ubuntu
  const unsigned indexOS = 0;
  #define  TEMPLATE_UID  103
  #define MACH_MACOS_ARRAY_IDX     MACH_PRE_MACOS14_GNU
  #if defined(__aarch64__)
    // ARM64
    #define UBUNTU_ARRAY_IDX UBUNTU_ARM64
  #else
    #define UBUNTU_ARRAY_IDX UBUNTU_X86_64
  #endif

#else // other OS's
  const unsigned indexOS = 0;
  #define  TEMPLATE_UID  103
  #define MACH_MACOS_ARRAY_IDX     MACH_PRE_MACOS14_GNU
  #define UBUNTU_ARRAY_IDX UBUNTU_X86_64
#endif
