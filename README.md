# Raycaster

A WIP old school ray casting program.

![preview](res/previews/preview.png)

![preview-gif](res/previews/rayc.gif)

![preview-resize](res/previews/rayc_resize.gif)

## Compiling

**Note: This project is very much a WIP. It is being developed on OSX El
Capitan (10.11.4) and the `Makefile` has not been configured to locate SDL2
correctly on other platforms. Cross platform support is on my TODO list.**

This project is dependent on SDL2, which must first be installed at https://www.libsdl.org/download-2.0.php. 
Download the runtime binaries for your operating system and install. 

Once SDL2 is available, clone this repository.

```sh
git clone https://github.com/adelciotto/raycaster.git
```

Change into the cloned directory and run make.

```sh
cd raycaster
make
```

Then run the `rayc` executable.

```sh
./rayc
```

## Configuration

The programs settings and map data are stored in JSON format. You can change the
settings by editing `settings.json` located at the root of the repository.
These are the defaults.

```js
{
  "width": 640,
  "height": 360,
  "fullscreen": false,
  "map": "assets/maps/small.json",
  "vsync": true
}
```

## Third-party code

- [SDL2](https://www.libsdl.org/download-2.0.php)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- [picojson](https://github.com/kazuho/picojson)
- [Generic Makefile](https://github.com/mbcrawfo/GenericMakefile) by [mbcrawfo](https://github.com/mbcrawfo)

## Resources

- [lodev.org](http://lodev.org/cgtutor/raycasting.html)
- [permadi.com](http://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- [Chocolate Wolfenstein 3D](https://github.com/fabiensanglard/Chocolate-Wolfenstein-3D)

## License & Legal

This code-base is released under the MIT License.

All dependencies are released under their own respective licenses.

For now some images are taken from ID Sofwares Wolfenstein 3D to use as
placeholders. These are
copyright of ID Software and I claim no ownership for any of these assets.
