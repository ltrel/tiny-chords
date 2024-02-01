#pragma once
#include <nlohmann/json_fwd.hpp>
#include "section.hpp"
#include "chord.hpp"

using json = nlohmann::ordered_json;

void to_json(json &j, const Chord &chord);
void to_json(json &j, const Section &section);

void from_json(const json &j, Chord &chord);
void from_json(const json &j, Section &section);
