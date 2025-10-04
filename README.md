# Iris — A Software Rasterizer in C

**Iris** is a fully functional software rasterizer written in C.  
It implements the fundamentals of a 3D rendering pipeline without relying on external graphics libraries or hardware acceleration.

> ⚠️ **Status:** Work in Progress (WIP)  
> This project is under active development. Many features are incomplete or experimental.

---

## Features (Planned and Implemented)

- [x] Perspective-correct projection using projection matrices
- [x] Geometry and Models
- [x] Camera and Transformations
- [x] Loading and parsing OBJ files (vertices, faces, textures)
- [x] Back-face culling
- [x] Translation, scaling, and rotation matrices for world transforms
- [x] Lighting and Shading
- [x] Flat shading
- [x] Texture loading and mapping (PNG loading using [uPNG](https://github.com/elanthis/upng))
- [x] Perspective-correct interpolation of UV coordinates
- [ ] Depth-based rendering using a Z-buffer
- [ ] Camera space and Movement
- [ ] Frustum clipping
- [ ] Rendering multiple meshes simultaneously

---

## TODO

🚧 This section lists upcoming changes and improvements targeted for the next minor version 🚧

- [ ] Refactoring and Code comments
- [x] Texture mapping
- [x] Implement PNG decoding
- [x] Textured OBJ files
- [ ] Update painter's algorithm to Z-buffering

---

## Project Structure

```
iris/
├── assets/             # 3D models, textures, and other resource files
├── build/              # Compiled binaries (ignored by Git)
├── src/                # All source code for the renderer
├── .clang-format       # Code style configuration
├── .gitignore          # Untracked files
├── LICENSE             # Project license
├── Makefile            # Build script
└── README.md           # This file
```

---

## License

This project is licensed under the [MIT License](LICENSE).
