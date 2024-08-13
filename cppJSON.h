#ifndef CPPJSON_CPPJSON_H
#define CPPJSON_CPPJSON_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <variant>
#include <algorithm>
#include <regex>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::ostream;
using std::istream;

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

class JSON;

/* 重载 << 操作符 */
ostream &operator<<(ostream &, const StringValue &);

ostream &operator<<(ostream &, const IntValue &);

ostream &operator<<(ostream &, const FloatValue &);

ostream &operator<<(ostream &, const BoolValue &);

ostream &operator<<(ostream &, const NULLValue &);

ostream &operator<<(ostream &, const JSONObject &);

ostream &operator<<(ostream &, const JSONArray &);

//ostream &operator<<(ostream &, const JSONValue &);

ostream &operator<<(ostream &, const JSON &);

/* 重载 >> 操作符 */
istream &operator>>(istream &, JSON &);

shared_ptr<JSON> parse_value(const string &, size_t &pos);

bool JSONisEmpty(const JSON &);

size_t JSONSize(const JSON &);

bool removeElement(JSONObject &json_object, const string &str);

bool popElement(JSONArray &json_array, size_t pos);

//void JSONInitialization(JSON &, const JSONValue &);

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

    friend bool JSONisEmpty(const JSON &);

    friend size_t JSONSize(const JSON &);

    friend bool removeElement(JSONObject &json_object, const string &str);

public:
    /* 构造函数 */
    explicit JSONObject(const string &json_string);

    explicit JSONObject(const char str[]) : JSONObject(string(str)) {}

    /* 拷贝控制成员 */
    JSONObject(const JSONObject &cj);

    JSONObject &operator=(const JSONObject &);

    vector<string> keys();

    JSON &operator[](const string &);

private:
    vector<string> object_key;     // 保存JSON对象的键
    vector<shared_ptr<JSON>> object_value;  // 保存JSON对象的值
};

/* JSON值为array类型 */
class JSONArray : public BaseValue {
    /* 友元函数 */
    friend bool JSONisEmpty(const JSON &);

    friend ostream &operator<<(ostream &, const JSONArray &);

    friend size_t JSONSize(const JSON &);

    friend bool popElement(JSONArray &json_array, size_t pos);

public:
    /* 构造函数 */
    explicit JSONArray(const string &str);

    explicit JSONArray(const char str[]) : JSONArray(string(str)) {}

    /* 拷贝控制成员 */
    JSONArray(const JSONArray &cj);

    JSONArray &operator=(const JSONArray &cj);

    JSON &operator[](const int &);

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

    void push_back(const JSON &json);

private:
    vector<shared_ptr<JSON>> array_value;    // 保存JSON数组
};

class JSON {
    friend shared_ptr<JSON> parse_value(const string &, size_t &pos);

    friend ostream &operator<<(ostream &, const JSONObject &);

    friend ostream &operator<<(ostream &out, const JSONArray &json_array);

    friend ostream &operator<<(ostream &out, const JSON &json_value);

    friend istream &operator>>(istream &, JSON &);

    friend bool JSONisEmpty(const JSON &);

    friend size_t JSONSize(const JSON &);

    friend void JSONArray::push_back(const JSON &json);

public:
    /* 构造函数 */
    explicit JSON(const string &str);

    explicit JSON(const char str[]) : JSON(string(str)) {}

    /* JSON对象和JSON数组共有的操作 */
    bool empty() const;

    size_t size();

    JSON &operator=(const string &v);

    JSON &operator=(const char v[]);

    JSON &operator=(const long double &v);

    JSON &operator=(const double &v);

    JSON &operator=(const long long &v);

    JSON &operator=(const int &v);

    JSON &operator=(const bool &v);

    JSON &operator=(std::nullptr_t v);

    JSON &operator=(const JSON &json);

    /* JSON对象特有的操作 */
    JSON &operator[](const string &);

    JSON &operator[](const char str[]);

    vector<string> keys();

    bool remove(const string &str);

    bool remove(const char str[]);

    /* JSON数组特有的操作*/
    JSON &operator[](const int &);

    template<typename T>
    void push_back(const T &);

    bool pop(size_t pos);

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

    explicit JSON(const Value &v) : value(v) {}

private:
    Value value;
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
