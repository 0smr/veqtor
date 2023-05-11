## Veqtor

+ Import statement: `import veqtor 0.1`.
+ Inherits: `QQuickItem`.

### Properties:

+ `src`:  `string`
  Path to the SVG file or any string containing the SVG document.

+ `document`: `Object` *read-only*
  An object that includes a key-value pair of elements based on their `ID`s.

+ `root`:  `svg`
  A pointer to the root SVG element.

+ `sourceSize`:  `size`
  This is the value of the `viewBox` property in the SVG document.

### Signals:

- `svgLoaded`: Fires when the source is loaded.
- `hovered`(**target**: `element`): Fires when the mouse hovers over an element in the SVG document. :warning: