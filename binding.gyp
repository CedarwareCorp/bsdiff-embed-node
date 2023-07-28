{
  "targets": [
    {
      "target_name": "bsdiff_embed_node",
      "sources": [
        "bsdiff/bsdiff.c",
        "bsdiff_wrapper.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "bsdiff"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc": [ "-fno-exceptions", "-std=c++20" ],
      "xcode_settings": {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'NO',
        "CLANG_CXX_LIBRARY": "libc++",
        "CLANG_CXX_LANGUAGE_STANDARD":"c++20",
        'MACOSX_DEPLOYMENT_TARGET': '10.14'
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": [ "-std:c++20", ],
        },
      }
    }
  ]
}