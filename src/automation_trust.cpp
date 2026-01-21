#include "automation_trust.h"

#include <boost/algorithm/string.hpp>

#include <vector>

namespace automation_trust {
std::set<std::string> ParseTrustedKeys(std::string_view raw) {
	std::set<std::string> out;
	std::vector<std::string> toks;
	std::string s(raw);
	boost::split(toks, s, boost::is_any_of("|"), boost::token_compress_off);
	for (auto& tok : toks) {
		boost::algorithm::trim(tok);
		if (!tok.empty()) out.emplace(tok);
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

