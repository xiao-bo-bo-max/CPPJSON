#include "cppJSON.h"


shared_ptr<CJSONValue> parse_value(const string &str, size_t &pos) {
    while (isspace(str[pos])||str[pos]==',') ++pos;
    // 值为string类型
    if (str[pos] == '"') {
        size_t start = pos + 1;
        size_t end = str.find('"', start);
        string value = str.substr(start, end - start);
        pos = end + 1;
        return make_shared<StringValue>(value);
    }
    // 值为数字（整数或浮点数）类型
    else if (isdigit(str[pos]) || str[pos] == '-' || str[pos] == '+') {
        size_t start = pos;
        while (isdigit(str[pos]) || str[pos] == '.') ++pos;
        string value = str.substr(start, pos - start);
        if (value.find('.') == string::npos)
            return make_shared<IntValue>(stoll(value));
        else
            return make_shared<FloatValue>(stod(value));
    }
    // 值为布尔类型
    else if (str[pos] == 't' || str[pos] == 'f') {
        pos += (str[pos] == 't') ? 4 : 5;
        if (str[pos] == 't')
            return make_shared<BoolValue>(true);
        else
            return make_shared<BoolValue>(false);
    }
    // 值为null
    else if (str[pos] == 'n') {
        pos += 4;
        return make_shared<NULLValue>();
    }
    // 值为json数组类型
    else if (str[pos] == '[') {
        ++pos;
        JSONArray array_value;
        while (str[pos] != ']') {
            array_value.push_back(parse_value(str, pos));
        }
        ++pos;
        return make_shared<JSONArray>(array_value);
    }
    // 值为json对象类型
    else if (str[pos] == '{') {
        size_t start = pos;
        int braces = 1;
        ++pos;
        while (braces > 0) {
            if (str[pos] == '{') ++braces;
            if (str[pos] == '}') --braces;
            ++pos;
        }
        string json_str = str.substr(start, pos - start);
        return make_shared<JSONObject>(json_str);
    }
    throw std::runtime_error("Unqualified JSON string");
}


JSONObject::JSONObject(const string &json_str) : CJSONValue(json_object_type) {
    size_t pos = 0;
    while (pos < json_str.size()) {
        if (json_str[pos] == '"') {
            size_t start = pos + 1;
            size_t end = json_str.find('"', start);
            string key = json_str.substr(start, end - start);
            object_key.push_back(key);
            pos = end + 1;

            while (json_str[pos] != ':') ++pos;
            ++pos;

            object_value.push_back(parse_value(json_str, pos));
        }
        ++pos;
    }
}

JSONArray::JSONArray(const string &json_str) : CJSONValue(json_array_type) {
    size_t pos = 0;
    array_value.push_back(parse_value(json_str, pos));
}

void JSONArray::push_back(shared_ptr<CJSONValue> value) {
    array_value.push_back(value);
}



