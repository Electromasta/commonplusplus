# Common-Plus-Plus
Commonly used Std Extensions Library for C++

I've found the std library is lacking features or is generally slow, so this attempts to consolidate common solutions I use in one place.

Header-only: everything lives under `src/common` and is included as `"common/..."`.

## To Use
Install it through vcpkg from my registry.

`vcpkg-configuration.json`:
```json
{
    "registries": [
        {
            "kind": "git",
            "repository": "https://git.cranberry-box.com/lewisb3/vcpkg-registry.git",
            "baseline": "<registry commit>",
            "packages": ["commonplusplus"]
        }
    ]
}
```
`vcpkg.json`:
```json
{ "dependencies": [ { "name": "commonplusplus", "features": ["reflect"] } ] }
```
- CMake: `find_package(commonplusplus CONFIG REQUIRED)`, then link `commonplusplus::commonplusplus`. Flags and dependencies come with it.
- Makefile: add `-DGLM_FORCE_SWIZZLE -DGLM_ENABLE_EXPERIMENTAL` to your flags so every file sees the same glm settings.

## Features
- Application Looper
- Bidirectional Map
- Singleton
- Resource Registry
- Counter Accumulator
- Timer
- String Utilities and File Utilities
- String16, a 15-character inline string
- Aliasing commonly used c/c++ and std library features in a single include
- Log wrapper for spdlog
- GLM single include, plus reflect-cpp support for glm, String16 and Bitset (`reflect` feature)
- Bitmask Type

## Dependencies
Installed by vcpkg: fmt, spdlog, glm. The `reflect` feature adds reflect-cpp (toml, msgpack).

## Developing
- `make build` / `make run`: builds and runs `sandbox/main.cpp`, a quick smoke test
- `make check`: every header must compile on its own
- `make experimental`: compiles the parked code in `experimental/`

`experimental/` holds code that isn't ready or isn't used yet. It uses the same `common/` include prefix but is never installed. Moving a file into `src/common` is just a `git mv`; includes don't change.
