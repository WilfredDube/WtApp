#pragma once

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "SheetMetal.h"

std::string save(const std::shared_ptr<Fxt::SheetMetalComponent::SheetMetal>& sheetMetalFeatureModel);

std::shared_ptr<Fxt::SheetMetalComponent::SheetMetal> restore(std::string restoreStr);