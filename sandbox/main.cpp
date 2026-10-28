#include <rfl/toml.hpp>

#include "common/core.types.h"
#include "common/logging.types.h"
#include "common/math.types.h"
#include "common/functions/hello.fn.h"
#include "common/reflect/math.reflect.h"
#include "common/reflect/tinystring.reflect.h"

// Smoke test for the library: logging, String16, glm swizzle, and a reflect-cpp toml round trip.
struct Config {
    String16  name;
    u8        num;
    glm::vec3 position;
};

int main()  {
    Log::info("Compiled with: {}", Fn::cxxversion());

    const Config config = { "Hello", 30, glm::vec3(1.0f, 2.0f, 3.0f) };
    const String toml = rfl::toml::write(config);
    Log::info("Config as toml:\n{}", toml);

    auto loaded = rfl::toml::read<Config>(toml);
    if (!loaded) {
        Log::error("toml read failed: {}", loaded.error().what());
        return 1;
    }

    const glm::vec2 xy = loaded->position.xy();
    Log::info("Read back: {} {} xy=({}, {})", loaded->name, loaded->num, xy.x, xy.y);

    if (loaded->name != config.name || loaded->num != config.num || loaded->position != config.position) {
        Log::error("toml round trip changed the value");
        return 1;
    }
    Log::info("Sandbox OK");
}
