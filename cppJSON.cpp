#include "cppJSON.h"

/* 分析json值的类型，返回值为指向用json值构造对应的对象的shared_ptr */
shared_ptr<JSONValue> parse_value(const string &str, size_t &pos) {
    while (isspace(str[pos]) || str[pos] == ',') ++pos;
    // 值为string类型
    if (str[pos] == '"') {
        size_t start = pos + 1;
        size_t end = str.find('"', start);
        string value = str.substr(start, end - start);
        pos = end + 1;
        return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<StringValue>, value));
    }
        // 值为数字（整数或浮点数）类型
    else if (isdigit(str[pos]) || str[pos] == '-' || str[pos] == '+') {
        size_t start = pos;
        while (isdigit(str[pos]) || str[pos] == '.') ++pos;
        string value = str.substr(start, pos - start);
        if (value.find('.') == string::npos)
            return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<IntValue>, stoll(value)));
        else
            return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<FloatValue>, stod(value)));
    }
        // 值为布尔类型
    else if (str[pos] == 't' || str[pos] == 'f') {
        pos += (str[pos] == 't') ? 4 : 5;
        if (str[pos] == 't')
            return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<BoolValue>, true));
        else
            return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<BoolValue>, false));
    }
        // 值为null
    else if (str[pos] == 'n') {
        pos += 4;
        return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<NULLValue>));
    }
        // 值为json数组类型
    else if (str[pos] == '[') {
        size_t start = pos;
        int braces = 1;
        ++pos;
        while (braces > 0) {
            if (str[pos] == '[') ++braces;
            if (str[pos] == ']') --braces;
            ++pos;
        }
        string json_str = str.substr(start, pos - start);
        return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<JSONArray>, json_str));
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
        return make_shared<JSONValue>(JSONValue::Value(std::in_place_type<JSONObject>, json_str));
    }
    throw std::runtime_error("Unqualified JSON value");
}

/* 解析一个json字符串，将键值对存储在数据成员中 */
JSONObject::JSONObject(const string &json_str) : BaseValue(JSON_OBJECT_TYPE) {
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

/* 拷贝构造函数，实现值拷贝 */
JSONObject::JSONObject(const JSONObject &cj) : BaseValue(cj.value_type) {
    *this = cj;   // 调用拷贝赋值运算符
}

/* 拷贝赋值运算符，实现值拷贝 */
JSONObject &JSONObject::operator=(const JSONObject &json_object) {
    this->object_value.clear();
    this->object_key = json_object.object_key;
    for (const auto &i: json_object.object_value) {
        this->object_value.push_back(make_shared<JSONValue>(*i));
    }
    return *this;
}

/* 返回JSON对象的键 */
vector<string> JSONObject::keys() {
    return object_key;
}

ostream &operator<<(ostream &out, const JSONObject &json_object) {
    out << "{";
    size_t i = 0;
    for (; i < json_object.object_key.size(); ++i) {
        out << "\"" << json_object.object_key[i] << "\":";
        std::visit([&out](const auto &v) {
            operator<<(out, v);
        }, (*json_object.object_value[i]).value);
        if (i != json_object.object_key.size() - 1) {
            out << ",";
        }
    }
    out << "}";
    return out;
}

JSONValue &JSONObject::operator[](const string &key) {
    auto pos = std::find(object_key.begin(), object_key.end(), key);
    if (pos != object_key.end()) {
        return *object_value[pos - object_key.begin()];
    } else {
        object_key.push_back(key);
        object_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<NULLValue>)));
        return *object_value[object_value.size() - 1];
    }
}

/* 解析一个json字符串，将值存储在数据成员中 */
JSONArray::JSONArray(const string &json_str) : BaseValue(JSON_ARRAY_TYPE) {
    size_t pos = 0;
    while (json_str[pos] != '[') ++pos;
    ++pos;
    while (pos < json_str.size()) {
        if (!isspace(json_str[pos]) && json_str[pos] != ']') {
            array_value.push_back(parse_value(json_str, pos));
        }
        ++pos;
    }
}

/* 拷贝构造函数，实现值拷贝 */
JSONArray::JSONArray(const JSONArray &cj) : BaseValue(cj.value_type) {
    *this = cj;   // 调用拷贝赋值运算符
}

/* 拷贝赋值运算符，实现值拷贝 */
JSONArray &JSONArray::operator=(const JSONArray &cj) {
    this->array_value.clear();
    for (const auto &i: cj.array_value) {
        this->array_value.push_back(make_shared<JSONValue>(*i));
    }
    return *this;
}

void JSONArray::push_back(const string &value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<StringValue>, value)));
}

void JSONArray::push_back(const char value[]) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<StringValue>, value)));
}

void JSONArray::push_back(const long double &value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<FloatValue>, value)));
}

void JSONArray::push_back(const double &value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<FloatValue>, value)));
}

void JSONArray::push_back(const long long &value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<IntValue>, value)));
}

void JSONArray::push_back(const int &value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<IntValue>, value)));
}

void JSONArray::push_back(const bool &value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<BoolValue>, value)));
}

void JSONArray::push_back(std::nullptr_t value) {
    array_value.push_back(make_shared<JSONValue>(JSONValue::Value(std::in_place_type<NULLValue>)));
}

void JSONArray::push_back(const JSONValue &value) {
    array_value.push_back(make_shared<JSONValue>(value));
}

void JSONArray::push_back(const JSON &json) {
    std::visit([this](const auto &v) {
        switch (v.valueType()) {
            case JSON_OBJECT_TYPE: {
                this->array_value.push_back(make_shared<JSONValue>(JSONValue::Value(v)));
                break;
            }
            case JSON_ARRAY_TYPE: {
                this->array_value.push_back(make_shared<JSONValue>(JSONValue::Value(v)));
                break;
            }
            default: {
                throw std::runtime_error("Unrecognized json class");
            }
        }
    }, json.value);
}

ostream &operator<<(ostream &out, const JSONArray &json_array) {
    out << "[";
    size_t i = 0;
    for (; i < json_array.array_value.size(); ++i) {
        std::visit([&out](const auto &v) {
            out << v;
        }, (*json_array.array_value[i]).value);
        if (i != json_array.array_value.size() - 1) {
            out << ",";
        }
    }
    out << "]";
    return out;
}

JSONValue &JSONArray::operator[](const size_t &pos) {
    if (pos < array_value.size()) {
        return *array_value[pos];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

ostream &operator<<(ostream &out, const StringValue &string_value) {
    out << "\"" << string_value.value << "\"";
    return out;
}

ostream &operator<<(ostream &out, const IntValue &int_value) {
    out << int_value.value;
    return out;
}

ostream &operator<<(ostream &out, const FloatValue &float_value) {
    out << float_value.value;
    return out;
}

ostream &operator<<(ostream &out, const BoolValue &bool_value) {
    if (bool_value.value)
        out << "true";
    else
        out << "false";
    return out;
}

ostream &operator<<(ostream &out, const NULLValue &null_value) {
    out << "null";
    return out;
}

ostream &operator<<(ostream &out, const JSONValue &json_value) {
    std::visit([&out](const auto &v) {
        out << v;
    }, json_value.value);
    return out;
}

JSONValue &JSONValue::operator=(const string &v) {
    value.emplace<StringValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(const char v[]) {
    value.emplace<StringValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(const long double &v) {
    value.emplace<FloatValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(const double &v) {
    value.emplace<FloatValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(const long long &v) {
    value.emplace<IntValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(const int &v) {
    value.emplace<IntValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(const bool &v) {
    value.emplace<BoolValue>(v);
    return *this;
}

JSONValue &JSONValue::operator=(std::nullptr_t v) {
    value.emplace<NULLValue>();
    return *this;
}

JSONValue &JSONValue::operator=(const JSONValue &json_value) {
    std::visit([&json_value, this](const auto &v) {
        switch (v.valueType()) {
            case STRING_TYPE: {
                this->value.emplace<StringValue>(std::get<StringValue>(json_value.value));
                break;
            }
            case INT_TYPE: {
                this->value.emplace<IntValue>(std::get<IntValue>(json_value.value));
                break;
            }
            case FLOAT_TYPE: {
                this->value.emplace<FloatValue>(std::get<FloatValue>(json_value.value));
                break;
            }
            case BOOL_TYPE: {
                this->value.emplace<BoolValue>(std::get<BoolValue>(json_value.value));
                break;
            }
            case NULL_TYPE: {
                this->value.emplace<NULLValue>();
                break;
            }
            case JSON_OBJECT_TYPE: {
                this->value.emplace<JSONObject>(std::get<JSONObject>(json_value.value));
                break;
            }
            case JSON_ARRAY_TYPE: {
                this->value.emplace<JSONArray>(std::get<JSONArray>(json_value.value));
                break;
            }
            default: {
                throw std::runtime_error("Unrecognized json_value class");
            }
        }
    }, json_value.value);
    return *this;
}

JSONValue &JSONValue::operator=(const JSON &json) {
    std::visit([this](const auto &v) {
        switch (v.valueType()) {
            case JSON_OBJECT_TYPE: {
                this->value = JSONValue::Value(v);
                break;
            }
            case JSON_ARRAY_TYPE: {
                this->value = JSONValue::Value(v);
                break;
            }
            default: {
                throw std::runtime_error("Unrecognized json_value class");
            }
        }
    }, json.value);
    return *this;
}


JSON::JSON(const string &str) : value(std::in_place_type<JSONObject>, "{}") {
    size_t pos = 0;
    while (isspace(str[pos])) ++pos;
    if (str[pos] == '{') {
        value.emplace<JSONObject>(str);
    } else if (str[pos] == '[') {
        value.emplace<JSONArray>(str);
    } else {
        throw std::runtime_error("Unqualified JSON string");
    }
}

ostream &operator<<(ostream &out, const JSON &json) {
    std::visit([&out](const auto &v) {
        out << v;
    }, json.value);
    return out;
}

JSONValue &JSON::operator[](const string &key) {
    return std::visit([this, key](const auto &v) -> JSONValue & {
        if (v.valueType() == JSON_OBJECT_TYPE) {
            auto &json_object = std::get<JSONObject>(this->value);
            return json_object[key];
        } else {
            throw std::runtime_error("This object cannot be indexed with a string");
        }
    }, value);
}

JSONValue &JSON::operator[](const size_t &pos) {
    return std::visit([this, pos](const auto &v) -> JSONValue & {
        if (v.valueType() == JSON_ARRAY_TYPE) {
            auto &json_array = std::get<JSONArray>(this->value);
            return json_array[pos];
        } else {
            throw std::runtime_error("This object cannot be indexed with a integers");
        }
    }, value);
}

vector<string> JSON::keys() {
    int type = std::visit([](const auto &v) -> int {
        return v.valueType();
    }, value);
    if (type == JSON_OBJECT_TYPE) {
        return std::get<JSONObject>(value).keys();
    } else {
        throw std::runtime_error("The object does not have keys() function");
    }
}

bool isEmpty(const JSON &json) {
    int type = std::visit([](const auto &v) -> int {
        return v.valueType();
    }, json.value);
    if (type == JSON_OBJECT_TYPE) {
        const auto &json_object = std::get<JSONObject>(json.value);
        if (json_object.object_key.empty() && json_object.object_value.empty())
            return true;
        else
            return false;
    } else if (type == JSON_ARRAY_TYPE) {
        const auto &json_array = std::get<JSONArray>(json.value);
        if (json_array.array_value.empty())
            return true;
        else
            return false;
    } else
        throw std::runtime_error("Unrecognized type");
}

size_t size(const JSON &json) {
    int type = std::visit([](const auto &v) -> int {
        return v.valueType();
    }, json.value);
    if (type == JSON_OBJECT_TYPE) {
        const auto &json_object = std::get<JSONObject>(json.value);
        return json_object.object_key.size();
    } else if (type == JSON_ARRAY_TYPE) {
        const auto &json_array = std::get<JSONArray>(json.value);
        return json_array.array_value.size();
    } else
        throw std::runtime_error("Unrecognized type");
}

JSONValue::operator string() const {
    int type = std::visit([](const auto &v) -> int {
        return v.valueType();
    }, value);
    if (type == STRING_TYPE) {
        return static_cast<string>(std::get<StringValue>(value));
    } else {
        throw std::runtime_error("Cannot convert to string type");
    }
}

JSONValue::operator long double() const {
    int type = std::visit([](const auto &v) -> int {
        return v.valueType();
    }, value);
    if (type == FLOAT_TYPE) {
        return static_cast<long double>(std::get<FloatValue>(value));
    } else {
        throw std::runtime_error("Cannot convert to long double type");
    }
}





