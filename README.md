# 🧊 Software Renderer in C

### Pixels, triangles, and 3D — built from the ground up.

A learning project for building a **CPU-based software renderer** with **C and SDL2**. The goal is to understand how a 3D scene becomes an image by implementing the rendering pipeline ourselves.

**🛠️ C99 · 🪟 SDL2 · 🧠 CPU rendering · 📐 Graphics mathematics**

> 🚧 **Under active development — work in progress.** This is an early learning prototype, not a finished renderer. Features, project structure, and implementation details will evolve as development progresses. The roadmap below describes the intended direction rather than a feature availability checklist.

---

## 🎯 The goal

Build a renderer that transforms 3D geometry into pixels using our own C code:

- 📐 Represent vertices, vectors, matrices, and meshes.
- 🔄 Transform objects and position a camera.
- 🔭 Project 3D coordinates onto a 2D image.
- ✂️ Clip geometry and discard hidden faces.
- 🔺 Rasterize triangles into a framebuffer.
- 🧱 Resolve visibility with a depth buffer.
- 💡 Explore lighting and texture mapping.

**We will not use OpenGL, DirectX, or Vulkan to implement the rendering pipeline.** The CPU will calculate geometry, visibility, and pixel colors. SDL2 will provide the window, input handling, timing, and a way to display the resulting image.

## 🧭 Development roadmap

These areas guide development. Their scope and order may change as the renderer takes shape; this list does not track completion.

- 🔁 Application lifecycle, input handling, and frame timing.
- 🖼️ CPU framebuffer and SDL2 presentation.
- ✏️ Pixel, line, and triangle drawing in C.
- 📐 Vector and matrix mathematics.
- 🧊 3D transformations, camera movement, and projection.
- ✂️ Clipping and back-face culling.
- 🧱 Depth buffering for overlapping geometry.
- 💡 Mesh loading, lighting, and texture mapping.

## 🧰 Requirements

- **GCC** with C99 support.
- **GNU Make**.
- **SDL2 development headers and libraries** — SDL2, rather than SDL3.
- A graphical desktop session to open the window.

The Makefile uses `gcc` and links SDL2 with `-lSDL2`.

## 🚀 Build and run

These commands build and launch the current development prototype.

Run these commands from the project root:

```bash
# 🔨 Compile the source code
make build

# ▶️ Launch the application
make run

# 🧹 Remove the compiled executable
make clean
```

The executable is named `renderer` and is ignored by Git.

## 🔁 Core rendering-loop concept

```text
Initialize SDL and create the window
                  │
                  ▼
         ┌─ Process input ◄──────┐
         │        │             │
         │        ▼             │
         │   Update state       │
         │        │             │
         │        ▼             │
         │   Render a frame     │
         │        │             │
         │        ▼             │
         │   Frame pacing ──────┘
         │
         └─ On exit → release resources
```

**Input** handles user and window events. **Update** advances scene state. **Render** produces and displays a frame. **Frame pacing** controls how frequently the loop repeats. The arrows show the conceptual execution order until shutdown releases application resources.

This diagram explains the loop's responsibilities; timing and implementation details will evolve during development.

## 🧠 The future rendering pipeline

```text
3D mesh → object and camera transforms → clipping and projection
        → triangle rasterization → CPU framebuffer → SDL2 window
```

A **mesh** supplies vertices and triangles. **Transforms** place them relative to the camera. **Clipping and projection** prepare visible geometry for the screen. **Rasterization** determines which pixels each triangle covers. A **framebuffer** stores the calculated colors, and SDL2 displays that image.

This is the intended architecture, not a statement that every stage is implemented.

## 🧪 Validation approach

Build with `make build` and review compiler warnings. Use `make run` in a graphical session to inspect rendering behavior and input handling as features are developed.

Useful validation targets include:

- 📐 Vector and matrix operations with known expected results.
- 🔺 Rasterization edge cases, including degenerate triangles and screen boundaries.
- 🧱 Depth comparisons for overlapping geometry.
- ⏱️ Consistent animation speed across different frame rates.
- 🧹 Resource cleanup and initialization failure handling.

These are validation goals, not a claim of existing automated test coverage.

---

**🎓 Built to learn how rendering works — one pixel at a time.**
