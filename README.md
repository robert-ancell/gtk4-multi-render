Test program to show we can have multiple windows in GTK4 with a
shared GL context - this is how Flutter wants to render multiple windows.

To use
```
$ meson setup _build
$ meson compile -C _build
$ ./_build/gtk4-multi-render
```
