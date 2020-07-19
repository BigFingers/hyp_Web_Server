#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <google/template.h>
using namespace rapidjson;
using namespace google;

std::string ReturnHtml();
std::string ReturnJson();
std::string PhraseJson(const std::string& json);