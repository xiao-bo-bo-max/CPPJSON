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

/**
 * @brief 重载输出流操作符，用于打印StringValue对象
 *
 * 将StringValue对象的值以JSON字符串格式输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param string_value 要打印的StringValue对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const StringValue &string_value);

/**
 * @brief 重载输出流操作符，用于打印IntValue对象
 *
 * 将IntValue对象的值输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param int_value 要打印的IntValue对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const IntValue &int_value);

/**
 * @brief 重载输出流操作符，用于打印FloatValue对象
 *
 * 将FloatValue对象的值输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param float_value 要打印的FloatValue对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const FloatValue &float_value);

/**
 * @brief 重载输出流操作符，用于打印BoolValue对象
 *
 * 将BoolValue对象的值（true或false）输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param bool_value 要打印的BoolValue对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const BoolValue &bool_value);

/**
 * @brief 重载输出流操作符，用于打印NULLValue对象
 *
 * 将字符串"null"输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param null_value 要打印的NULLValue对象的常量引用 (未使用)
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const NULLValue &null_value);

/**
 * @brief 重载输出流操作符，用于打印JSONObject对象
 *
 * 将JSONObject对象以JSON对象格式输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param json_object 要打印的JSONObject对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const JSONObject &json_object);

/**
 * @brief 重载输出流操作符，用于打印JSONArray对象
 *
 * 将JSONArray对象以JSON数组格式输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param json_array 要打印的JSONArray对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const JSONArray &json_array);

/**
 * @brief 重载输出流操作符，用于打印JSON对象
 *
 * 将JSON对象的值（可能是JSONObject, JSONArray, StringValue等）输出到指定的输出流。
 *
 * @param out 输出流对象的引用
 * @param json 要打印的JSON对象的常量引用
 * @return 输出流对象的引用
 */
ostream &operator<<(ostream &out, const JSON &json);

/**
 * @brief 重载输入流操作符，用于从输入流解析JSON对象
 *
 * 从输入流读取字符串，并尝试将其解析为JSON对象。
 * 根据输入字符串的内容，可能将其解析为JSON对象、JSON数组、数字、字符串或布尔值。
 *
 * @param in 输入流对象的引用
 * @param json 用于存储解析结果的JSON对象的引用
 * @return 输入流对象的引用
 */
istream &operator>>(istream &in, JSON &json);

/**
 * @brief 重载相等操作符，用于比较两个JSON对象是否相等
 *
 * 将两个JSON对象转换为字符串表示形式进行比较。
 *
 * @param json1 第一个JSON对象的常量引用
 * @param json2 第二个JSON对象的常量引用
 * @return 如果两个JSON对象相等则返回true，否则返回false
 */
bool operator==(const JSON &json1, const JSON &json2);

/**
 * @brief 重载不等操作符，用于比较两个JSON对象是否不相等
 *
 * 利用重载的相等操作符进行判断。
 *
 * @param json1 第一个JSON对象的常量引用
 * @param json2 第二个JSON对象的常量引用
 * @return 如果两个JSON对象不相等则返回true，否则返回false
 */
bool operator!=(const JSON &json1, const JSON &json2);

/**
 * @brief 解析JSON字符串中的一个值
 *
 * 根据当前位置的字符判断值的类型（字符串、数字、布尔、null、数组、对象），
 * 并递归调用自身或创建相应的JSON值对象。
 *
 * @param str 包含JSON数据的字符串常量引用
 * @param pos 当前解析位置的引用，函数执行后会更新到下一个值的起始位置
 * @return 指向新创建的JSON对象的shared_ptr
 * @throw std::runtime_error 如果遇到不合格的JSON值
 * @note 此函数会跳过值前的空格和逗号。
 */
shared_ptr<JSON> parse_value(const string &str, size_t &pos);

/**
 * @brief 检查JSON对象（JSONObject或JSONArray）是否为空
 *
 * 对于JSONObject，检查其键和值是否都为空。
 * 对于JSONArray，检查其值是否为空。
 *
 * @param json 要检查的JSON对象的常量引用
 * @return 如果JSON对象为空则返回true，否则返回false
 * @throw std::runtime_error 如果JSON对象的类型无法识别
 */
bool JSONisEmpty(const JSON &json);

/**
 * @brief 获取JSON对象（JSONObject或JSONArray）的大小
 *
 * 对于JSONObject，返回键值对的数量。
 * 对于JSONArray，返回元素的数量。
 *
 * @param json 要获取大小的JSON对象的常量引用
 * @return JSON对象的大小
 * @throw std::runtime_error 如果JSON对象的类型无法识别
 */
size_t JSONSize(const JSON &json);

/**
 * @brief 从JSONObject中移除指定的键值对
 *
 * 查找并移除JSONObject中与给定键匹配的键值对。
 *
 * @param json_object 要操作的JSONObject对象的引用
 * @param str 要移除的键的字符串常量引用
 * @return 如果成功移除元素则返回true，否则（键不存在）返回false
 */
bool removeElement(JSONObject &json_object, const string &str);

/**
 * @brief 从JSONArray中移除指定位置的元素
 *
 * 移除JSONArray中指定索引位置的元素。
 *
 * @param json_array 要操作的JSONArray对象的引用
 * @param pos 要移除的元素的位置（索引）
 * @return 如果成功移除元素则返回true，否则（索引越界）返回false
 */
bool popElement(JSONArray &json_array, int pos);

/**
 * @brief JSON值的基类
 *
 * 存储JSON值的类型，并提供获取该类型的方法。
 * 所有具体的JSON值类型（如IntValue, StringValue）都继承自此类。
 */
class BaseValue {
public:
    /**
     * @brief BaseValue的构造函数
     * @param t 值的类型，使用预定义的宏（如INT_TYPE, STRING_TYPE等）
     */
    explicit BaseValue(int t) : value_type(t) {};

    /**
     * @brief BaseValue的虚析构函数
     *
     * 确保派生类的析构函数能够被正确调用。
     */
    virtual ~BaseValue() = default;

    /**
     * @brief 获取JSON值的类型
     * @return 表示JSON值类型的整数
     */
    int valueType() const { return value_type; }

protected:
    int value_type = -1;    // JSON值的类型
};

/**
 * @brief JSON整数值类
 *
 * 继承自BaseValue，用于表示JSON中的整数类型。
 */
class IntValue : public BaseValue {
    friend ostream &operator<<(ostream &, const IntValue &);

public:
    /**
     * @brief IntValue的构造函数
     * @param v long long类型的整数值
     */
    explicit IntValue(const long long &v) : BaseValue(INT_TYPE), value(v) {}

    /**
     * @brief 类型转换操作符，将IntValue转换为long long
     * @return 存储的long long整数值
     */
    explicit operator long long() const { return value; }

    /**
     * @brief 类型转换操作符，将IntValue转换为int
     * @return 存储的整数值（可能发生截断）
     */
    explicit operator int() const { return static_cast<int>(value); }

private:
    long long value;
};

/**
 * @brief JSON浮点值类
 *
 * 继承自BaseValue，用于表示JSON中的浮点数类型。
 */
class FloatValue : public BaseValue {
    friend ostream &operator<<(ostream &, const FloatValue &);

public:
    /**
     * @brief FloatValue的构造函数
     * @param v long double类型的浮点数值
     */
    explicit FloatValue(const long double &v) : BaseValue(FLOAT_TYPE), value(v) {}

    /**
     * @brief 类型转换操作符，将FloatValue转换为long double
     * @return 存储的long double浮点数值
     */
    explicit operator long double() const { return value; }

    /**
     * @brief 类型转换操作符，将FloatValue转换为double
     * @return 存储的浮点数值（可能发生精度损失）
     */
    explicit operator double() const { return static_cast<double>(value); }

private:
    long double value;
};

/**
 * @brief JSON字符串值类
 *
 * 继承自BaseValue，用于表示JSON中的字符串类型。
 */
class StringValue : public BaseValue {
    friend ostream &operator<<(ostream &, const StringValue &);

public:
    /**
     * @brief StringValue的构造函数 (std::string)
     * @param v std::string类型的字符串值
     */
    explicit StringValue(const string &v) : BaseValue(STRING_TYPE), value(v) {}

    /**
     * @brief StringValue的构造函数 (C风格字符串)
     * @param str C风格字符串 (const char[])
     */
    explicit StringValue(const char str[]) : StringValue(string(str)) {}

    /**
     * @brief 类型转换操作符，将StringValue转换为std::string
     * @return 存储的std::string字符串值
     */
    explicit operator string() const { return value; }

private:
    string value;
};

/**
 * @brief JSON布尔值类
 *
 * 继承自BaseValue，用于表示JSON中的布尔类型 (true/false)。
 */
class BoolValue : public BaseValue {
    friend ostream &operator<<(ostream &, const BoolValue &);

public:
    /**
     * @brief BoolValue的构造函数
     * @param v bool类型的布尔值
     */
    explicit BoolValue(const bool &v) : BaseValue(BOOL_TYPE), value(v) {}

    /**
     * @brief 类型转换操作符，将BoolValue转换为bool
     * @return 存储的bool布尔值
     */
    explicit operator bool() const { return value; }

private:
    bool value;
};

/**
 * @brief JSON NULL值类
 *
 * 继承自BaseValue，用于表示JSON中的null类型。
 */
class NULLValue : public BaseValue {
    friend ostream &operator<<(ostream &, const NULLValue &);

public:
    /**
     * @brief NULLValue的构造函数
     *
     * 将类型设置为NULL_TYPE。
     */
    NULLValue() : BaseValue(NULL_TYPE) {}
};

/**
 * @brief JSON对象类
 *
 * 继承自BaseValue，用于表示JSON中的对象类型（键值对的集合）。
 * 内部使用vector存储键和对应的值（shared_ptr<JSON>）。
 */
class JSONObject : public BaseValue {
    /* 友元函数 */
    friend ostream &operator<<(ostream &, const JSONObject &);

    friend bool JSONisEmpty(const JSON &);

    friend size_t JSONSize(const JSON &);

    friend bool removeElement(JSONObject &json_object, const string &str);

public:
    /**
     * @brief JSONObject的构造函数 (从JSON字符串)
     *
     * 解析给定的JSON格式字符串，并填充对象的键和值。
     *
     * @param json_string 包含JSON对象数据的字符串常量引用
     * @throw std::runtime_error 如果解析过程中遇到不合格的JSON值
     */
    explicit JSONObject(const string &json_string);

    /**
     * @brief JSONObject的构造函数 (从C风格JSON字符串)
     * @param str C风格的JSON格式字符串
     */
    explicit JSONObject(const char str[]) : JSONObject(string(str)) {}

    /**
     * @brief JSONObject的拷贝构造函数
     *
     * 实现深拷贝，复制源对象的所有键和值。
     *
     * @param cj 要拷贝的JSONObject对象的常量引用
     */
    JSONObject(const JSONObject &cj);

    /**
     * @brief JSONObject的拷贝赋值操作符
     *
     * 实现深拷贝，将源对象的所有键和值赋给当前对象。
     *
     * @param json_object 要赋值的JSONObject对象的常量引用
     * @return 当前JSONObject对象的引用
     */
    JSONObject &operator=(const JSONObject &json_object);

    /**
     * @brief 获取JSONObject对象的所有键
     * @return 包含所有键的std::vector<std::string>
     */
    vector<string> keys();

    /**
     * @brief 重载下标操作符，用于访问或创建JSONObject中的元素
     *
     * 如果键存在，则返回对应值的引用。
     * 如果键不存在，则创建一个新的键值对，值为null，并返回新创建值的引用。
     *
     * @param key 要访问或创建的元素的键 (std::string)
     * @return 对应键的JSON值的引用
     */
    JSON &operator[](const string &key);

    /**
     * @brief 访问JSONObject中的元素，带边界检查
     *
     * 如果键存在，则返回对应值的引用。
     * 如果键不存在，则抛出std::out_of_range异常。
     *
     * @param key 要访问的元素的键 (std::string)
     * @return 对应键的JSON值的引用
     * @throw std::out_of_range 如果键不存在
     */
    JSON &at(const string &key);

    /**
     * @brief 合并另一个JSONObject到当前对象
     *
     * 将另一个JSONObject的所有键值对添加到当前对象。
     * 如果存在相同的键，则后添加的键值对会覆盖原有的。
     * （注意：当前实现是简单地将键和值向量追加，可能导致重复键，行为可能与预期不符，具体取决于后续如何处理重复键）
     *
     * @param json_object 要合并的JSONObject对象的常量引用
     * @return 当前JSONObject对象的引用
     */
    JSONObject &merge(const JSONObject &json_object);

private:
    vector<string> object_key;     // 保存JSON对象的键
    vector<shared_ptr<JSON>> object_value;  // 保存JSON对象的值
};

/**
 * @brief JSON数组类
 *
 * 继承自BaseValue，用于表示JSON中的数组类型（值的有序列表）。
 * 内部使用vector<shared_ptr<JSON>>存储数组元素。
 */
class JSONArray : public BaseValue {
    /* 友元函数 */
    friend bool JSONisEmpty(const JSON &);

    friend ostream &operator<<(ostream &, const JSONArray &);

    friend size_t JSONSize(const JSON &);

    friend bool popElement(JSONArray &json_array, int pos);

public:
    /**
     * @brief JSONArray的构造函数 (从JSON字符串)
     *
     * 解析给定的JSON格式字符串（期望是数组格式），并填充数组元素。
     *
     * @param str 包含JSON数组数据的字符串常量引用
     * @throw std::runtime_error 如果解析过程中遇到不合格的JSON值
     */
    explicit JSONArray(const string &str);

    /**
     * @brief JSONArray的构造函数 (从C风格JSON字符串)
     * @param str C风格的JSON格式字符串
     */
    explicit JSONArray(const char str[]) : JSONArray(string(str)) {}

    /**
     * @brief JSONArray的拷贝构造函数
     *
     * 实现深拷贝，复制源数组的所有元素。
     *
     * @param cj 要拷贝的JSONArray对象的常量引用
     */
    JSONArray(const JSONArray &cj);

    /**
     * @brief JSONArray的拷贝赋值操作符
     *
     * 实现深拷贝，将源数组的所有元素赋给当前数组。
     *
     * @param cj 要赋值的JSONArray对象的常量引用
     * @return 当前JSONArray对象的引用
     */
    JSONArray &operator=(const JSONArray &cj);

    /**
     * @brief 重载下标操作符，用于访问JSONArray中的元素
     *
     * 返回指定索引位置的元素的引用。
     *
     * @param index 要访问的元素的索引 (int)
     * @return 对应索引的JSON值的引用
     * @throw std::out_of_range 如果索引越界
     */
    JSON &operator[](const int &index);

    /**
     * @brief 向JSON数组末尾添加一个元素 (通用模板)
     *
     * 这是一个通用模板，但如果类型T不是push_back重载中明确支持的类型，
     * 它将抛出std::runtime_error。
     *
     * @tparam T 要添加的元素类型
     * @param value 要添加的元素值的常量引用
     * @throw std::runtime_error 如果类型T不被支持
     */
    template<typename T>
    void push_back(const T &value);

    /**
     * @brief 向JSON数组末尾添加一个std::string元素
     * @param value 要添加的std::string值的常量引用
     */
    void push_back(const string &value);

    /**
     * @brief 向JSON数组末尾添加一个C风格字符串元素
     * @param value C风格字符串 (const char[])
     */
    void push_back(const char value[]);

    /**
     * @brief 向JSON数组末尾添加一个long double元素
     * @param value 要添加的long double值的常量引用
     */
    void push_back(const long double &value);

    /**
     * @brief 向JSON数组末尾添加一个double元素
     * @param value 要添加的double值的常量引用
     */
    void push_back(const double &value);

    /**
     * @brief 向JSON数组末尾添加一个long long元素
     * @param value 要添加的long long值的常量引用
     */
    void push_back(const long long &value);

    /**
     * @brief 向JSON数组末尾添加一个int元素
     * @param value 要添加的int值的常量引用
     */
    void push_back(const int &value);

    /**
     * @brief 向JSON数组末尾添加一个bool元素
     * @param value 要添加的bool值的常量引用
     */
    void push_back(const bool &value);

    /**
     * @brief 向JSON数组末尾添加一个nullptr (JSON null) 元素
     * @param value std::nullptr_t
     */
    void push_back(std::nullptr_t value);

    /**
     * @brief 向JSON数组末尾添加一个JSON对象元素
     * @param json 要添加的JSON对象的常量引用
     */
    void push_back(const JSON &json);

private:
    vector<shared_ptr<JSON>> array_value;    // 保存JSON数组
};

/**
 * @brief 主JSON类，封装了所有JSON数据类型
 *
 * 使用std::variant存储实际的JSON值，可以是JSONObject, JSONArray,
 * IntValue, FloatValue, StringValue, BoolValue, 或 NULLValue。
 * 提供了构造、赋值、类型检查、访问和修改等操作。
 */
class JSON {
    friend shared_ptr<JSON> parse_value(const string &, size_t &pos);

    friend ostream &operator<<(ostream &, const JSONObject &);

    friend ostream &operator<<(ostream &out, const JSONArray &json_array);

    friend ostream &operator<<(ostream &out, const JSON &json_value);

    friend istream &operator>>(istream &, JSON &);

    friend bool JSONisEmpty(const JSON &);

    friend size_t JSONSize(const JSON &);

    friend void JSONArray::push_back(const JSON &json);

    friend bool operator==(const JSON &, const JSON &);

    friend bool operator!=(const JSON &, const JSON &);

public:
    /**
     * @brief JSON的构造函数 (从JSON字符串)
     *
     * 解析给定的JSON格式字符串。如果字符串以'{'开头，则解析为JSONObject；
     * 如果以'['开头，则解析为JSONArray。
     *
     * @param str 包含JSON数据的字符串常量引用
     * @throw std::runtime_error 如果JSON字符串格式不合格
     */
    explicit JSON(const string &str);

    /**
     * @brief JSON的构造函数 (从C风格JSON字符串)
     * @param str C风格的JSON格式字符串
     */
    explicit JSON(const char str[]) : JSON(string(str)) {}

    /**
     * @brief 检查JSON对象或数组是否为空
     * @return 如果为空则返回true，否则返回false
     * @throw std::runtime_error 如果内部值类型无法识别（不应发生）
     * @see JSONisEmpty
     */
    bool empty() const;

    /**
     * @brief 获取JSON对象或数组的大小
     * @return 对象中的键值对数量或数组中的元素数量
     * @throw std::runtime_error 如果内部值类型无法识别（不应发生）
     * @see JSONSize
     */
    size_t size() const;

    // 赋值操作符
    /**
     * @brief 赋值操作符 (std::string)
     *
     * 将JSON对象的值设置为给定的字符串。
     *
     * @param v 要赋值的std::string常量引用
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const string &v);

    /**
     * @brief 赋值操作符 (C风格字符串)
     * @param v C风格字符串 (const char[])
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const char v[]);

    /**
     * @brief 赋值操作符 (long double)
     * @param v 要赋值的long double常量引用
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const long double &v);

    /**
     * @brief 赋值操作符 (double)
     * @param v 要赋值的double常量引用
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const double &v);

    /**
     * @brief 赋值操作符 (long long)
     * @param v 要赋值的long long常量引用
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const long long &v);

    /**
     * @brief 赋值操作符 (int)
     * @param v 要赋值的int常量引用
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const int &v);

    /**
     * @brief 赋值操作符 (bool)
     * @param v 要赋值的bool常量引用
     * @return 当前JSON对象的引用
     */
    JSON &operator=(const bool &v);

    /**
     * @brief 赋值操作符 (nullptr, JSON null)
     * @param v std::nullptr_t
     * @return 当前JSON对象的引用
     */
    JSON &operator=(std::nullptr_t v);

    /**
     * @brief 拷贝赋值操作符 (JSON)
     *
     * 实现深拷贝，将源JSON对象的值赋给当前对象。
     *
     * @param json 要赋值的JSON对象的常量引用
     * @return 当前JSON对象的引用
     * @throw std::runtime_error 如果源JSON对象的内部类型无法识别（不应发生）
     */
    JSON &operator=(const JSON &json);

    // 类型转换
    /**
     * @brief 显式类型转换到std::string
     * @return 如果内部值为StringValue，则返回其字符串表示
     * @throw std::runtime_error 如果无法转换为字符串类型
     */
    explicit operator string() const;

    /**
     * @brief 显式类型转换到int
     * @return 如果内部值为IntValue，则返回其整数表示 (可能截断)
     * @throw std::runtime_error 如果无法转换为int类型
     */
    explicit operator int() const;

    /**
     * @brief 显式类型转换到long long
     * @return 如果内部值为IntValue，则返回其long long表示
     * @throw std::runtime_error 如果无法转换为long long类型
     */
    explicit operator long long() const;

    /**
     * @brief 显式类型转换到long double
     * @return 如果内部值为FloatValue，则返回其long double表示
     * @throw std::runtime_error 如果无法转换为long double类型
     */
    explicit operator long double() const;

    /**
     * @brief 显式类型转换到double
     * @return 如果内部值为FloatValue，则返回其double表示 (可能损失精度)
     * @throw std::runtime_error 如果无法转换为double类型
     */
    explicit operator double() const;

    /**
     * @brief 显式类型转换到bool
     * @return 如果内部值为BoolValue，则返回其布尔表示
     * @throw std::runtime_error 如果无法转换为bool类型
     */
    explicit operator bool() const;

    // 类型检查
    /** @brief 检查内部值是否为字符串类型 */
    bool isString();

    /** @brief 检查内部值是否为整数类型 */
    bool isInteger();

    /** @brief 检查内部值是否为浮点数类型 */
    bool isFloat();

    /** @brief 检查内部值是否为布尔类型 */
    bool isBool();

    /** @brief 检查内部值是否为null类型 */
    bool isNULL();

    /** @brief 检查内部值是否为JSON对象类型 */
    bool isJSONObject();

    /** @brief 检查内部值是否为JSON数组类型 */
    bool isJSONArray();

    // JSON对象操作
    /**
     * @brief 重载下标操作符，用于访问或创建JSONObject中的元素 (std::string key)
     *
     * 如果内部值不是JSONObject，则抛出异常。
     * 否则，行为与JSONObject::operator[]一致。
     *
     * @param key 要访问或创建的元素的键 (std::string)
     * @return 对应键的JSON值的引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     */
    JSON &operator[](const string &key);

    /**
     * @brief 重载下标操作符，用于访问或创建JSONObject中的元素 (C风格字符串 key)
     * @param str C风格字符串键
     * @return 对应键的JSON值的引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     */
    JSON &operator[](const char str[]);

    /**
     * @brief 访问JSONObject中的元素，带边界检查 (std::string key)
     *
     * 如果内部值不是JSONObject，则抛出异常。
     * 否则，行为与JSONObject::at一致。
     *
     * @param key 要访问的元素的键 (std::string)
     * @return 对应键的JSON值的引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     * @throw std::out_of_range 如果键不存在于JSONObject中
     */
    JSON &at(const string &key);

    /**
     * @brief 访问JSONObject中的元素，带边界检查 (C风格字符串 key)
     * @param str C风格字符串键
     * @return 对应键的JSON值的引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     * @throw std::out_of_range 如果键不存在于JSONObject中
     */
    JSON &at(const char str[]);

    /**
     * @brief 获取JSONObject的所有键
     * @return 包含所有键的std::vector<std::string>
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     */
    vector<string> keys();

    /**
     * @brief 从JSONObject中移除指定的键值对 (std::string key)
     * @param str 要移除的键
     * @return 如果成功移除则返回true，否则返回false
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     * @see removeElement
     */
    bool remove(const string &str);

    /**
     * @brief 从JSONObject中移除指定的键值对 (C风格字符串 key)
     * @param str C风格字符串键
     * @return 如果成功移除则返回true，否则返回false
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型
     */
    bool remove(const char str[]);

    /**
     * @brief 合并一个或多个JSON对象到当前JSON对象 (如果当前为JSONObject)
     *
     * 对于参数列表中的每个JSON对象，如果它也是JSONObject类型，
     * 则将其合并到当前对象中。
     *
     * @tparam args 参数包，期望是JSON对象
     * @param json_list 要合并的JSON对象列表
     * @return 当前JSON对象的引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型，
     * 或者参数列表中的某个对象不是JSONObject类型，
     * 或者参数类型不是JSON。
     * @note 当前JSONObject的merge行为是追加键和值，可能导致重复键。
     */
    template<typename ...args>
    JSON &merge(const args &... json_list);

    // JSON数组操作
    /**
     * @brief 重载下标操作符，用于访问JSONArray中的元素 (int index)
     *
     * 如果内部值不是JSONArray，则抛出异常。
     * 否则，行为与JSONArray::operator[]一致。
     *
     * @param index 要访问的元素的索引
     * @return 对应索引的JSON值的引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONArray类型
     * @throw std::out_of_range 如果索引越界
     */
    JSON &operator[](const int &index);

    /**
     * @brief 向JSONArray末尾添加一个元素
     *
     * 如果内部值不是JSONArray，则抛出异常。
     * 否则，调用JSONArray::push_back。
     *
     * @tparam T 要添加的元素类型
     * @param value 要添加的元素值的常量引用
     * @throw std::runtime_error 如果当前JSON对象不是JSONArray类型，
     * 或者类型T不被JSONArray::push_back支持。
     */
    template<typename T>
    void push_back(const T &value);

    /**
     * @brief 从JSONArray中移除指定位置的元素
     * @param pos 要移除的元素的位置（索引）
     * @return 如果成功移除则返回true，否则返回false
     * @throw std::runtime_error 如果当前JSON对象不是JSONArray类型
     * @see popElement
     */
    bool pop(int pos);

    /**
     * @brief JSON内部值的类型别名，使用std::variant
     *
     * std::variant可以存储多种不同但预定义类型的值。
     */
    using Value = std::variant<
            JSONObject,
            JSONArray,
            IntValue,
            FloatValue,
            StringValue,
            BoolValue,
            NULLValue
    >;

    /**
     * @brief JSON的构造函数 (从Value类型)
     *
     * 直接使用给定的Value对象初始化内部存储。
     * 主要供内部使用，例如在解析时创建特定类型的JSON值。
     *
     * @param v JSON::Value类型的常量引用
     */
    explicit JSON(const Value &v) : value(v) {}

private:
    Value value; // 存储实际JSON值的variant
};

// 模板函数实现
/**
 * @brief JSONArray::push_back的模板实现
 *
 * 这是一个通用模板，但如果类型T不是push_back的特定重载中支持的类型，
 * 它将抛出std::runtime_error。特定类型的重载（如string, int, JSON等）
 * 应该优先匹配。
 *
 * @tparam T 要添加的元素类型
 * @param value 要添加的元素值的常量引用
 * @throw std::runtime_error 如果类型T不被显式支持
 */
template<typename T>
void JSONArray::push_back(const T &value) {
    // 如果没有为类型T提供特定的push_back重载，则此通用模板被调用。
    // 抛出异常，因为我们只希望显式支持的类型被添加到JSONArray中。
    throw std::runtime_error("Unqualified JSON value type for push_back into JSONArray");
}

/**
 * @brief JSON::push_back的模板实现
 *
 * 如果当前JSON对象是JSONArray类型，则将值v添加到该数组中。
 *
 * @tparam T 要添加的元素类型
 * @param v 要添加的元素值的常量引用
 * @throw std::runtime_error 如果当前JSON对象不是JSONArray类型，
 * 或者JSONArray不支持添加类型T的值。
 */
template<typename T>
void JSON::push_back(const T &v) {
    // 获取当前JSON对象的内部类型
    int type = std::visit([](const auto &val) -> int {
        // 假设所有variant的备选项都有valueType()成员，或者需要更复杂的类型检查
        // 对于JSONObject, JSONArray等，它们确实有valueType()
        // 对于IntValue, StringValue等，它们也有valueType()
        return val.valueType();
    }, value);

    if (type == JSON_ARRAY_TYPE) {
        // 如果是JSONArray类型，则获取JSONArray的引用并调用其push_back方法
        std::get<JSONArray>(value).push_back(v);
    } else {
        // 如果不是JSONArray类型，则抛出异常
        throw std::runtime_error("The JSON object is not an array, cannot push_back.");
    }
}

/**
 * @brief JSON::merge的模板实现 (可变参数模板)
 *
 * 将参数列表中的每个JSON对象合并到当前JSON对象中。
 * 前提条件是当前JSON对象和所有待合并的JSON对象都必须是JSONObject类型。
 *
 * @tparam args 参数包，表示要合并的JSON对象列表
 * @param json_list 一个或多个JSON对象的常量引用
 * @return 当前JSON对象的引用
 * @throw std::runtime_error 如果当前JSON对象不是JSONObject类型，
 * 或者参数列表中的任何一个JSON对象不是JSONObject类型，
 * 或者参数不是JSON类型。
 * @note 使用C++17的折叠表达式和lambda来实现对参数包中每个元素的操作。
 * JSONObject的merge行为是追加键和值，这可能导致重复的键。
 * 更健壮的合并逻辑可能需要检查并处理重复键的情况（例如，覆盖或抛出错误）。
 */
template<typename ...args>
JSON &JSON::merge(const args &... json_list) {
    // 确保当前JSON对象是JSONObject类型
    if (!this->isJSONObject()) {
        throw std::runtime_error("Cannot merge into a non-JSONObject type.");
    }

    // 使用折叠表达式遍历所有传入的json_list参数
    // 对于每个参数，执行一个lambda函数
    ([json_list, this]() {
        // 检查传入的参数是否是JSON类型
        if (!std::is_same<typename std::decay<decltype(json_list)>::type, JSON>::value) {
            throw std::runtime_error("Cannot merge: provided argument is not a JSON object.");
        }
        // 检查要合并的JSON对象是否也是JSONObject类型
        int type = std::visit([](const auto &v) -> int {
            return v.valueType();
        }, json_list.value);
        if (type != JSON_OBJECT_TYPE) {
            throw std::runtime_error(
                    "Cannot merge: one of the provided JSON objects to merge is not a JSONObject itself.");
        }
        // 执行合并操作
        // std::get<JSONObject>(this->value) 获取当前JSON对象内部的JSONObject引用
        // std::get<JSONObject>(json_to_merge.value) 获取待合并JSON对象内部的JSONObject引用
        std::get<JSONObject>(this->value).merge(std::get<JSONObject>(json_list.value));
    }(), ...);  // 折叠表达式的语法
    return *this;   // 返回当前对象的引用，支持链式操作
}

#endif //CPPJSON_CPPJSON_H
