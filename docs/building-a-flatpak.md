# Building a Flatpak (on Linux)

Runtimes provide the basic dependencies that are used by applications. Each application must be built against a runtime, and this runtime must be installed on a host system in order for the application to run (Flatpak can automatically install the runtime required by an application). Multiple different runtimes can be installed at the same time, including different versions of the same runtime.

### Step 1. Install a runtime and the matching SDK

- `flatpak install flathub org.kde.Platform//5.11 org.kde.Sdk//5.11`

### Step 2.  Build the application

- `cd flatpak`
- `flatpak-builder build-dir io.botfather.Botfather.yaml`

### Step 3. Test the build

To verify that the build was successful, run the following:

- `flatpak-builder --run build-dir io.botfather.Botfather.yaml botfather`

### Step 4: Put the app in a repository

Before you can install and run the app, it first needs to be put in a repository. This is done by passing the `--repo` argument to `flatpak-builder`:

- `flatpak-builder --repo=repo --force-clean build-dir io.botfather.Botfather.yaml`

This does the build again, and at the end exports the result to a local directory called `repo`. Note that `flatpak-builder` keeps a cache of previous builds in the `.flatpak-builder` subdirectory, so doing a second build like this is very fast.

This second time we passed in `--force-clean`, which means that the previously created `build-dir` directory was deleted before the new build was started.

### Step X: Todo

- Add how to sign with gpg
- Add how to deploy

## Troubleshooting

#### Debug the application

[See flatpaks documentation on that](http://docs.flatpak.org/en/latest/debugging.html)

##### In a nutshell:

Add those build options to the `io.botfather.Botfather.yaml` file to not strip debug info from the build.

```json
"build-options": {
	"no-debuginfo": true,
	"strip": false
}
```

Then start a shell in the sandbox using `flatpak-builder --run <build-dir> <manifest> sh`.
Then use `gdb`, `strace`, `ldd` and so on to debug.