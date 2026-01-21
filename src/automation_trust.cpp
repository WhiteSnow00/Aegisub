#include "automation_trust.h"

#include <cstddef>

namespace automation_trust {
namespace {
constexpr bool IsAsciiSpace(char c) noexcept {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

std::string_view TrimAscii(std::string_view s) noexcept {
	while (!s.empty() && IsAsciiSpace(s.front()))
		s.remove_prefix(1);
	while (!s.empty() && IsAsciiSpace(s.back()))
		s.remove_suffix(1);
	return s;
}
}

std::set<std::string> ParseTrustedKeys(std::string_view raw) {
	std::set<std::string> out;
	for (size_t pos = 0;;) {
		size_t end = raw.find('|', pos);
		if (end == std::string_view::npos)
			end = raw.size();

		auto tok = TrimAscii(raw.substr(pos, end - pos));
		if (!tok.empty())
			out.emplace(tok.data(), tok.size());

		if (end == raw.size())
			break;
		pos = end + 1;
	}
	return out;
}

std::string SerializeTrustedKeys(std::set<std::string> const& keys) {
	std::string out;
	for (auto const& k : keys) {
		if (!out.empty())
			out += '|';
		out += k;
	}
	return out;
}

std::string MakeProjectDirKey(agi::fs::path const& project_dir) {
	if (project_dir.empty()) return {};
	return "dir:" + agi::fs::Absolute(project_dir).generic_string();
}

std::string MakeScriptFileKey(agi::fs::path const& script_path) {
	if (script_path.empty()) return {};
	return "file:" + agi::fs::Absolute(script_path).generic_string();
}
}

