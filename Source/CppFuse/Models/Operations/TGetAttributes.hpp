#ifndef CPPFUSE_TGETATTRIBUTES_HPP
#define CPPFUSE_TGETATTRIBUTES_HPP

#include "CppFuse/Models/Objects/ASharedFileVariant.hpp"
#include <sys/stat.h>

namespace cppfuse {

class TGetAttributes {
    public:
    explicit TGetAttributes(struct stat* st);

    public:
    void operator()(const ASharedFileVariant& var);
    void operator()(const ASharedRwLock<SDirectory>& var);
    void operator()(const ASharedRwLock<SFile>& var);
    void operator()(const ASharedRwLock<SLink>& var);

    protected:
    void UpdateAttr(const auto& var);

    protected:
    struct stat* m_pSt = nullptr;
};

}

#endif //CPPFUSE_TGETATTRIBUTES_HPP
