## Veqtor

*Veqtor* is an SVG renderer library for the QML side, which enables you to load, parse, modify, and render SVG files.

The library makes use of *qnanopainter*, an OpenGL rendering library on the C++ side.

### Comparison to `Shape` and `Image`

`Shape `is a useful tool on the QML side that enables painting custom lines and shapes in QML, though it has some problems when used in an application.

1. you have to write code to use them. You can't design shapes in Illustrator or Inkscape and export them as a QML `Shape`.
2. they are not common to find. For example, if you want an icon for your buttons and only find SVG or PNG files, there is no way to convert them to a `Shape` component.
3. there is a small issue with QML's `Shape` that rendered output is not perfect. The example below shows a huge difference in the rendered output of a circle using `Shape` and Shader.

There is also the `Image` option, but its *properties* and *attributes* cannot be modified. This would be frustrating if you only want to change the color of an SVG icon.

### Supported Features

supported features are listed in [*feature-support.md*](./feature-support.md).

