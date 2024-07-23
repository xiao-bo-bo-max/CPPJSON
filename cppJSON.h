#ifndef CPPJSON_CPPJSON_H
#define CPPJSON_CPPJSON_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>

using std::string;
using std::vector;
using std::shared_ptr;
using std::pair;
using std::make_shared;

/* JSON值的类型 */
# define string_type 0
# define int_type 1
# define float_type 2
# define bool_type 3
# define null_type 4
# define json_array_type 5
# define json_object_type 6

class JSONObject;

class JSONArray;

class CJSONValue;

template<typename T>
T parse_value(const string &, size_t &pos);


class CJSONValue {
public:
    CJSONValue(int t) : value_type(t) {};

    virtual ~CJSONValue() = default;

protected:
    int value_type = -1;    // JSON值的类型
};


/* JSON值为整型 */
class IntValue : public CJSONValue {
public:
    IntValue(const long long &v) : CJSONValue(int_type), value(v) {}

private:
    long long value;

};

/* JSON值为浮点型 */
class FloatValue : public CJSONValue {
public:
    FloatValue(const long double &v) : CJSONValue(float_type), value(v) {}

private:
    long double value;

};

/* JSON值为字符串类型 */
class StringValue : public CJSONValue {
public:
    StringValue(const string &v) : CJSONValue(string_type), value(v) {}

private:
    string value;
};

/* JSON值为布尔类型 */
class BoolValue : public CJSONValue {
public:
    BoolValue(const bool &v) : CJSONValue(bool_type), value(v) {}

private:
    bool value;
};

/* JSON值为NULL类型 */
class NULLValue : public CJSONValue {
public:
    NULLValue() : CJSONValue(null_type), value("null") {}

private:
    string value;
};


class JSONObject : public CJSONValue {
    /* 友元函数 */
    friend JSONObject &emptyJSONObject();

public:
    /* 构造函数 */
    JSONObject() : CJSONValue(json_object_type), object_key(), object_value() {}   // 创建一个空的JSON对象

    JSONObject(const string &json_string);

    JSONObject(const char str[]) : JSONObject(string(str)) {}

private:
    vector<string> object_key;     // 保存JSON对象的键
    vector<shared_ptr<CJSONValue>> object_value;  // 保存JSON对象的值
};

class JSONArray : public CJSONValue {
    /* 友元函数 */
    friend JSONArray &emptyJSONArray();

public:
    /* 构造函数 */
    JSONArray():CJSONValue(json_array_type),array_value(){}    // 创建一个空的JSON数组

    JSONArray(const string &str);

    JSONArray(const char str[]) : JSONArray(string(str)) {}

    /* 成员函数 */
    void push_back(shared_ptr<CJSONValue>);

private:
    vector<shared_ptr<CJSONValue>> array_value;    // 保存JSON数组
};


#endif //CPPJSON_CPPJSON_H
