# Perspective projection illustrations

Read these images in order:

1. `01-camera-and-projection-plane.png`: the eye is A on the right, looking left. The main diagram is an outside top view of the x-z slice; the inset represents the camera image. Up on the main diagram means +x, not +y.
2. `02-two-slices-one-camera.png`: the x-z slice determines horizontal position; the y-z slice determines vertical position. These are two views of one camera setup.
3. `03-code-camera-and-cube.png`: the camera is at world (0, 0, -5), looking along +z toward the cube near the origin. The lower panel illustrates the image seen by the camera.

## Mapping to the code

- `src/main.c`, `update()`: subtracting cameraPosition.z converts world depth to camera-relative depth. World z=0 becomes camera z=5.
- `project()`: `(640*x/z, 640*y/z)` gives pixel offsets from the image center. The variable named `fov` acts as a focal length in pixels.
- `render()`: adding half the window dimensions places the projection origin at the window center.
- Positive screen y points down in this code. A geometry sketch with +y upward must be interpreted with that screen convention in mind.

## Reading the diagrams accurately

These are conceptual illustrations, not scale drawings or screenshots of the renderer. Numeric labels and formulas define the geometry; do not measure image lengths to infer coordinates. The plane one unit in front of the camera is virtual, represented by the projection math rather than explicitly created in the scene.

The camera-image panel simplifies the cube into nested square point grids to explain perspective shrinking. The actual setup samples coordinates from -1 through 0.75, and renders interior points without depth hiding, so the output will differ from the illustration.

Generated with the built-in image_gen tool. The full generation prompts are saved in `generation-prompts.txt`.
