#include <CppFuse/Controllers/NSFindFile.hpp>
#include <CppFuse/Controllers/TFileSystem.hpp>
#include <CppFuse/Errors/TFSException.hpp>
#include "TGetFileParameter.hpp"
#include <algorithm>
#include <array>

namespace cppfuse::NSFindFile {

static constexpr std::string_view s_sRootPath = "/";

ASharedFileVariant RecursiveFind(const fs::path& path,
    fs::path::iterator it, const rwl::TRwLockReadGuard<TDirectory>& dirRead) {

    const auto& itName = it->native();
    const auto& files = dirRead->Files;
    const auto childIt = std::ranges::find_if(files,
        [&itName](const auto& f) {
            return std::visit(TGetInfoName{}, f) == itName;
        }
    );
    if(childIt == files.end()) {
        throw TFSException(path.begin(), it, NFSExceptionType::FileNotExist);
    }
    if(std::distance(it, path.end()) == 1) {
        return *childIt;
    }
    if(const auto dir = std::get_if<ASharedRwLock<TDirectory>>(&*childIt)) {
        return RecursiveFind(path, ++it, (*dir)->Read());
    }
    throw TFSException(path.begin(), it, NFSExceptionType::NotDirectory);
}

template<typename T, auto FSExceptionValue>
ASharedRwLock<T> FindGeneral(const fs::path& path) {
    const auto obj = NSFindFile::Find(path);
    if(const auto t = std::get_if<ASharedRwLock<T>>(&obj)) {
        return *t;
    }
    throw TFSException(path.begin(), path.end(), FSExceptionValue);
}

ASharedFileVariant Find(const fs::path& path) {
    const auto& rootDir = TFileSystem::RootDir();
    const auto normalizedPath = path.lexically_normal();
    if(normalizedPath == s_sRootPath) {
        return rootDir;
    }
    return RecursiveFind(normalizedPath, ++normalizedPath.begin(), rootDir->Read());
}

ASharedRwLock<TDirectory> FindDir(const fs::path& path) {
    return FindGeneral<TDirectory, NFSExceptionType::NotDirectory>(path);
}

ASharedRwLock<TLink> FindLink(const fs::path& path) {
    return FindGeneral<TLink, NFSExceptionType::NotLink>(path);
}

ASharedRwLock<TRegularFile> FindRegularFile(const fs::path& path) {
    return FindGeneral<TRegularFile, NFSExceptionType::NotFile>(path);
}

}