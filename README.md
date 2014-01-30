MeshRenderer
============

Enter multiple files as command line arguments for more meshes.

Multiple Lights and material properties are supported, but curently must be hard-coded into main().

Timer and progress appears while ray tracing in console window


'q' resets polygon to inital state.

'aswd' rotate or translate depending on current mode, in scale mode, w and s perform y scaling, a andd d perform x scaling

'z' in scale mode performs scaling in all directions

'x' same as x but reverse

'r' sets rotate mode

't' sets translate mode

'c' sets scale mode

'i' raytraces with phong illumination (is not optimised for multiple inputs)

'o' (for multiple inputs) changes the polygon in "scope"; the one that transformations are applied to.

'p' (for multiple inputs) changes the polygon in "scope" in the other direction.

'k' subdivides current poly, will cycle through subdivisions ( only works with the first polygon input when using multiple inputs )
