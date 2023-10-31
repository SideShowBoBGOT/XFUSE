#include "TLink.hpp"

namespace cppfuse {

cppfuse::TLink::TLink(const std::string& name, mode_t mode, const ASharedRwLock<TDirectory>& parent, const std::filesystem::path& linkTo)
    : TFileObjectMixin<TDirectory, NFileType::Link>(name, mode, parent), m_xLinkTo{linkTo} {}

const std::filesystem::path& TLink::LinkTo() const { return m_xLinkTo; }

}