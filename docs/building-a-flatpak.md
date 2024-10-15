# Flatpak nightly builds

We use these builds for testing Flatpak runtimes and to pre-build the binaries we later ship to Flathub.

## Installing a runtime and matching SDK

`flatpak install flathub org.kde.Platform//5.15-23.08 org.kde.Sdk//5.15-23.08`

## Bulding the app

Before you can install and run the app, it first needs to be built and put in a repository:

- `flatpak-builder --repo=repo --force-clean build-dir io.botfather.Nightly.yaml`

## Testing the app

- `flatpak --user install ./repo io.botfather.Nightly`
- `flatpak run io.botfather.Nightly`

## Troubleshooting

### Debug the application

In a nutshell:

Add those build options to the `io.botfather.Nightly.yaml` file to not strip debug info from the build.

```json
"build-options": {
	"no-debuginfo": true,
	"strip": false
}
```

Then start a shell in the sandbox using `flatpak-builder --run <build-dir> <manifest> sh`.
Then use `gdb`, `strace`, `ldd` and so on to debug.

More infos can be found in the [Flatpak documentation on debugging](http://docs.flatpak.org/en/latest/debugging.html).
