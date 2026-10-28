#pragma once
#include <filesystem>
#include <unordered_set>
#include <vector>
#include "common/types/tinystring.h"
#include "common/functions/string.fn.h"

using File = std::filesystem::path;
using Files = std::vector<File>;
using Extensions = std::unordered_set<String16>;

namespace Fn::file {
    using std::filesystem::exists;
    using std::filesystem::rename;
    using std::filesystem::copy;
    using std::filesystem::remove;

    inline Files getFiles(const File & path) {
        Files files;
        if (!std::filesystem::exists(path)) return files;

        for (const auto & entry : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied)) {
            if (!entry.is_regular_file()) continue;
            files.push_back(entry.path());
        }

        return files;
    }

    inline Files filter(Files files, const Extensions & extensions)   {
        std::erase_if(files, [&](const File & file) {
            return !extensions.contains(String16(Fn::string::toLower(file.extension().string())));
        });
        return files;
    }

    inline String16 makeKey(const File & file)  {
        return String16(file.stem().string().substr(0, 15));
    }

    inline std::string getExtension(const File & path) {
        return Fn::string::toLower(path.extension().string());
    }
}

