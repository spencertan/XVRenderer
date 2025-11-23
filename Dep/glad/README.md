# GLAD - OpenGL Function Loader

GLAD is required for OpenGL support in XVRenderer.

## Setup Instructions

1. Visit https://glad.dav1d.de/
2. Configure GLAD with the following settings:
   - **Language**: C/C++
   - **Specification**: OpenGL
   - **API - gl**: Version 3.3 (or higher)
   - **Profile**: Core
   - **Extensions**: (leave default or add as needed)
   - **Options**: Check "Generate a loader"

3. Click "GENERATE" and download the zip file

4. Extract the contents:
   - Copy `include/glad/glad.h` to `Dep/glad/include/glad/`
   - Copy `include/KHR/khrplatform.h` to `Dep/glad/include/KHR/`
   - Copy `src/glad.c` to `Dep/glad/src/`

## Directory Structure

After setup, your directory should look like:
```
Dep/glad/
├── include/
│   ├── glad/
│   │   └── glad.h
│   └── KHR/
│       └── khrplatform.h
└── src/
    └── glad.c
```

## Alternative: Pre-configured Files

If you have GLAD already configured for OpenGL 3.3 Core, you can place the files in the structure above.

## Integration

GLAD is automatically integrated into the build system via premake5.lua when the files are present.
