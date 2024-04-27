# Flatpak nightly builds

We use these builds for testing Flatpak runtimes and to pre-build the binaries we later ship to Flathub.

## Installing a runtime and matching SDK

`flatpak install flathub org.kde.Platform//5.15-22.08 org.kde.Sdk//5.15-22.08`

## Building the application

`flatpak-builder build-dir --force-clean io.botfather.Nightly.yaml`

## Testing the build

To verify that the build was successful, run the following:

`flatpak-builder --run build-dir io.botfather.Nightly.yaml botfather`

## Putting the app in a repository

Before you can install and run the app, it first needs to be put in a repository. This is done by passing the `--repo` argument to `flatpak-builder`:

- `flatpak-builder --repo=repo --force-clean build-dir io.botfather.Nightly.yaml`

This does the build again, and at the end exports the result to a local directory called `repo`. Note that `flatpak-builder` keeps a cache of previous builds in the `.flatpak-builder` subdirectory, so doing a second build like this is very fast.

This second time we passed in `--force-clean`, which means that the previously created `build-dir` directory was deleted before the new build was started.

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
