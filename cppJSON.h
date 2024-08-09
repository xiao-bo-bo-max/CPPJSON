#ifndef CPPJSON_CPPJSON_H
#define CPPJSON_CPPJSON_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <variant>
#include <algorithm>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::ostream;

/* JSON值的类型 */
# define STRING_TYPE 0
# define INT_TYPE 1
# define FLOAT_TYPE 2
# define BOOL_TYPE 3
# define NULL_TYPE 4
# define JSON_ARRAY_TYPE 5
# define JSON_OBJECT_TYPE 6

class JSONObject;

class JSONArray;

class BaseValue;

class IntValue;

class FloatValue;

class StringValue;

class BoolValue;

class NULLValue;

class JSONValue;

class JSON;

ostream &operator<<(ostream &, const StringValue &);

ostream &operator<<(ostream &, const IntValue &);

ostream &operator<<(ostream &, const FloatValue &);

ostream &operator<<(ostream &, const BoolValue &);

ostream &operator<<(ostream &, const NULLValue &);

ostream &operator<<(ostream &, const JSONObject &);

ostream &operator<<(ostream &, const JSONArray &);

ostream &operator<<(ostream &, const JSONValue &);

ostream &operator<<(ostream &, const JSON &);

shared_ptr<JSONValue> parse_value(const string &, size_t &pos);

bool isEmpty(const JSON &);

size_t size(const JSON &);

class BaseValue {
public:
    explicit BaseValue(int t) : value_type(t) {};

    virtual ~BaseValue() = default;

    int valueType() const { return value_type; }

protected:
    int value_type = -1;    // JSON值的类型
};


/* JSON值为整型 */
class IntValue : public BaseValue {
    friend ostream &operator<<(ostream &, const IntValue &);

public:
    explicit IntValue(const long long &v) : BaseValue(INT_TYPE), value(v) {}

    explicit operator long long() const { return value; }

private:
    long long value;

};

/* JSON值为浮点型 */
class FloatValue : public BaseValue {
    friend ostream &operator<<(ostream &, const FloatValue &);

public:
    explicit FloatValue(const long double &v) : BaseValue(FLOAT_TYPE), value(v) {}

    explicit operator long double() const { return value; }

private:
    long double value;

};

/* JSON值为字符串类型 */
class StringValue : public BaseValue {
    friend ostream &operator<<(ostream &, const StringValue &);

public:
    explicit StringValue(const string &v) : BaseValue(STRING_TYPE), value(v) {}

    explicit StringValue(const char str[]) : StringValue(string(str)) {}

    explicit operator string() const { return value; }

private:
    string value;
};

/* JSON值为布尔类型 */
class BoolValue : public BaseValue {
    friend ostream &operator<<(ostream &, const BoolValue &);

public:
    explicit BoolValue(const bool &v) : BaseValue(BOOL_TYPE), value(v) {}

    explicit operator bool() const { return value; }

private:
    bool value;
};

/* JSON值为NULL类型 */
class NULLValue : public BaseValue {
    friend ostream &operator<<(ostream &, const NULLValue &);

public:
    NULLValue() : BaseValue(NULL_TYPE) {}
};

/* JSON值为object类型 */
class JSONObject : public BaseValue {
    /* 友元函数 */
    friend ostream &operator<<(ostream &, const JSONObject &);

    friend bool isEmpty(const JSON &);

    friend size_t size(const JSON &);

public:
    /* 构造函数 */
    explicit JSONObject(const string &json_string);

    explicit JSONObject(const char str[]) : JSONObject(string(str)) {}

    /* 拷贝控制成员 */
    JSONObject(const JSONObject &cj);

    JSONObject &operator=(const JSONObject &);

    vector<string> keys();

    JSONValue &operator[](const string &);

private:
    vector<string> object_key;     // 保存JSON对象的键
    vector<shared_ptr<JSONValue>> object_value;  // 保存JSON对象的值
};

/* JSON值为array类型 */
class JSONArray : public BaseValue {
    /* 友元函数 */
    friend bool isEmpty(const JSON &);

    friend ostream &operator<<(ostream &, const JSONArray &);

    friend size_t size(const JSON &);

public:
    /* 构造函数 */
    explicit JSONArray(const string &str);

    explicit JSONArray(const char str[]) : JSONArray(string(str)) {}

    /* 拷贝控制成员 */
    JSONArray(const JSONArray &cj);

    JSONArray &operator=(const JSONArray &cj);

    JSONValue &operator[](const size_t &);

    /* 将一个JSON值添加到JSON数组末尾 */
    template<typename T>
    void push_back(const T &);

    void push_back(const string &value);

    void push_back(const char value[]);

    void push_back(const long double &value);

    void push_back(const double &value);

    void push_back(const long long &value);

    void push_back(const int &value);

    void push_back(const bool &value);

    void push_back(std::nullptr_t value);

    void push_back(const JSONValue &value);

    void push_back(const JSON &json);

private:
    vector<shared_ptr<JSONValue>> array_value;    // 保存JSON数组
};

class JSONValue {
    friend shared_ptr<JSONValue> parse_value(const string &, size_t &pos);

    friend JSONValue &JSONObject::operator[](const std::string &);

    friend ostream &operator<<(ostream &out, const JSONObject &json_object);

    friend JSONValue &JSONArray::operator[](const size_t &);

    friend ostream &operator<<(ostream &out, const JSONArray &json_array);

    template<typename T>
    friend void JSONArray::push_back(const T &);

    friend ostream &operator<<(ostream &out, const JSONValue &json_array);

public:
    /* 赋值运算符 */
    JSONValue &operator=(const string &v);

    JSONValue &operator=(const char v[]);

    JSONValue &operator=(const long double &v);

    JSONValue &operator=(const double &v);

    JSONValue &operator=(const long long &v);

    JSONValue &operator=(const int &v);

    JSONValue &operator=(const bool &v);

    JSONValue &operator=(std::nullptr_t v);

    JSONValue &operator=(const JSONValue &json_value);

    JSONValue &operator=(const JSON &json);

    /* 类型转换 */
    operator string() const;

    operator long double() const;

    using Value = std::variant<
            JSONObject,
            JSONArray,
            IntValue,
            FloatValue,
            StringValue,
            BoolValue,
            NULLValue
    >;

    explicit JSONValue(const Value &v) : value(v) {}

private:
    Value value;
};

class JSON {
    friend ostream &operator<<(ostream &out, const JSON &json_value);

    friend bool isEmpty(const JSON &);

    friend size_t size(const JSON &);

    friend void JSONArray::push_back(const JSON &value);

    friend JSONValue &JSONValue::operator=(const JSON &json);

public:
    /* 构造函数 */
    explicit JSON(const string &str);

    explicit JSON(const char str[]) : JSON(string(str)) {}

    /* JSON对象特有的操作 */
    JSONValue &operator[](const string &);

    vector<string> keys();

    /* JSON数组特有的操作*/
    JSONValue &operator[](const size_t &);

    template<typename T>
    void push_back(const T &);

private:
    std::variant<JSONObject, JSONArray> value;
};

template<typename T>
void JSONArray::push_back(const T &value) {
    throw std::runtime_error("Unqualified JSON value");
}

template<typename T>
void JSON::push_back(const T &v) {
    int type = std::visit([](const auto &v) -> int {
        return v.valueType();
    }, value);
    if (type == JSON_ARRAY_TYPE) {
        std::get<JSONArray>(value).push_back(v);
    } else {
        throw std::runtime_error("The object does not have keys() function");
    }
}

#endif //CPPJSON_CPPJSON_H
