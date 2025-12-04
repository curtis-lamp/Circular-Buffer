# Circular Buffer

## Development

Create a python virtual environment. UV is used here, but any mechanism to create a virtual environment will do. Then install the build tools in requirements.txt.

```bash
$ uv venv --python=3.14 .venv
$ source .venv/bin/activate
$ uv pip install -r requirements.txt
```

Next install the conan dependencies.

```
$ conan profile detect # If no default profile is set up
$ conan install . --output-folder=build --build=missing
```

Then configure cmake.

```
$ cmake --preset conan-release
```

Finally, build the project and run the tests.

```
$ cmake --build --preset conan-release && ctest --preset conan-release
```
