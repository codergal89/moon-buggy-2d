# Moon Buggy 2D

Moon Buggy 2D is a 2D reimplementation of the classic [moon buggy](https://www.seehuhn.de/pages/moon-buggy) terminal game, using the [Godot](https://godotengine.org) game engine.

![Main Menu](doc/main_menu.png)

## Development

Moon Buggy 2D uses the well-known *Godot* engine and *Godot C++* to implement it's scripts in *C++20*. Dependencies are handled using the *Conan* package manager.
The following tools are required to build the scripts:

- Git
- A C++20 capable compiler
- Conan

### Compiling via Conan

Install the dependencies and prepare a CMake configuration using the following command line from the project root:

```bash
$ conan install . -u -pr:h ./.conan/profiles/default
```

Afterwards, the scripts library can be built via:

```bash
$ conan build . -pr:h ./.conan/profiles/default
```

### Compiling and Developing via Visual Studio Code

First install the dependencies as described above. Afterwards, source the generated environment and start Visual Studio Code using these commands from the project root:

```bash
$ . build/generators/conanbuild.sh
$ code .
```

Install and enable the suggested extensions. Afterwards, the scripts library can be built by pressing `F7`.

## Asset Licenses

All assets used in this project are licensed as follows:

| Asset Group      | Author                                                       | License Identifier                                                    |
|------------------|--------------------------------------------------------------|-----------------------------------------------------------------------|
| Buggy Animations | [Selina Capol](https://www.selinacapol.com/)                 | [CC-BY-NC-SA-4.0](https://spdx.org/licenses/CC-BY-NC-SA-4.0.html)     |
| Ground Tiles     | [Selina Capol](https://www.selinacapol.com/)                 | [CC-BY-NC-SA-4.0](https://spdx.org/licenses/CC-BY-NC-SA-4.0.html)     |
| Background       | [Selina Capol](https://www.selinacapol.com/)                 | [CC-BY-NC-SA-4.0](https://spdx.org/licenses/CC-BY-NC-SA-4.0.html)     |
| monogram Font    | [datagoblin](https://datagoblin.itch.io/)                    | [CC0-1.0](https://spdx.org/licenses/CC0-1.0.html)                     |
