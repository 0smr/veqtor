# Veqtor
<p><img src="https://img.shields.io/github/v/tag/smr76/veqtor?sort=semver&label=version&labelColor=0bd&color=07b" alt="version tag">
<img src="https://img.shields.io/github/license/smr76/veqtor?color=36b245" alt="license">
<a href="https://www.blockchain.com/bch/address/bitcoincash:qrnwtxsk79kv6mt2hv8zdxy3phkqpkmcxgjzqktwa3">
<img src="https://img.shields.io/badge/BCH-Donate-f0992e?logo=BitcoinCash&logoColor=f0992e" alt="BCH donate"></a></p>

Veqtor is an *SVG parser/renderer* that uses [qnanopainter](https://github.com/QUItCoding/qnanopainter/tree/master) as a render backend to allow for simple access and manipulation of *SVG* elements at runtime.

The lack of painting components in the *QML language* led me to create an *SVG* renderer component with complete access to its *elements*.

*If you liked, please consider givin a star :star2:.*

## Documentation
Full [documentation](docs/README.md) can be found in the `docs` directory.

## Quick intro
Simple use is as follows: set the `src` attribute to an *SVG string* or a *file address*, and you're done.
```qml
Veqtor { src: '<svg><path d="m10,10l30,30,l20,20z"/></svg>'; }
```
It is also possible to initialize *SVG* elements based on their `id`.<br>
For example, if you have `elmId` as the *path's element id*, you can access *that* element using the `elmId` property; updating `elmId` properties will also change the *path element's attributes*.
```qml
Veqtor {
    property var elmId: { "d": "m 20 0l.5-10" }
    src: '<svg><path id="elmId" d="m10,10l30,30,l20,20z"/></svg>';
}
```
Path may also be accessed using the `veqtor`'s `document` property.<br>
Which means that if I call the `foo` function in the code below, it will modify the *path `fill` property*.
```qml
Veqtor {
    id: veq
    src: '<svg><path id="elmId" d="m10,10l30,30,l20,20z"/></svg>';
}

function foo() {
    veq.document.pathId.fill = 'red';
}
```
## How to use
+ Clone the repository.
    ```bash
    git clone "https://github.com/SMR76/veqtor.git"
    ```
+ Include `Veqtor.pri` file.
 <sub>[see example-1](example/example-1/example-1.pro#L11)</sub><br>
    ```make
    include("path/to/Veqtor.pri")
    ```
+ Import the `Veqtor` module. <sub>[see example-1](example/example-1/main.qml#L6)</sub><br>
    ```qml
    import Veqtor 0.1
    ```

## To-Do
Setting external css and implementing all elements are future plans.

## Issues
Please file an issue on [issues page](https://github.com/SMR76/veqtor/issues) if you have any problems.