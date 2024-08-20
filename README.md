# 项目简介

**项目名称**： CPPJSON

**项目概述**：CPPJSON 是一个轻量级、高效的 C++ JSON 解析库。它允许开发者轻松地将 JSON 格式的字符串转换为 C++ 对象，并支持将 C++ 对象序列化为 JSON 格式。该库旨在提供直观的 API、快速的解析速度，以及最小的依赖性，适合嵌入式系统和资源受限的环境。

**版本依赖**：CPPJSON主要基于C++11开发，同时使用到了部分C++17的特性。

# 功能描述

1. **解析JSON格式的字符串**：将 JSON 格式的字符串或者 JSON 文件解析为 C++ 对象：JSON。JSON格式的字符串或者 JSON 文件支持嵌套的 JSON 对象和 JSON 数组。
2. **拷贝及合并JSON**：将 JSON 对象（数组）拷贝到另一个 JSON 对象（数组）中，将多个 JSON 对象合并成一个。
3. **增、删、改、查JSON数据：**添加、删除、修改、访问 JSON 对象的某个键值对或 JSON 数组的某个元素。
4. **打印JSON对象**：打印 JSON 对象的某个值或 JSON 数组的某个元素；打印 JSON 对象的所有键值对或 JSON 数组的所有元素。

# 应用技术

## C++11

以下是需要掌握的 C++ 11 知识点：

1. 类和继承
   - 类的定义和实现
   - 继承和多态
   - 虚函数和纯虚函数
2. 模板
   - 函数模板
   - 可变参数模板
3. 智能指针
   - std::shared_ptr 的使用
4. STL 容器
   - std::vector 的使用
5. 重载运算符
   - 输入输出运算符 (<<, >>)
   - 下标运算符 ([])
   - 赋值运算符 (=)
   - 比较运算符 (==, !=)
6. 类型转换
   - 显式类型转换操作符
   - static_cast
7. 异常处理
   - throw 语句
   - std::runtime_error
8. 友元函数和友元类
9. 命名空间
   - using 声明
10. 函数对象和 Lambda 表达式
11. 类型特征 (Type Traits)
	- std::is_same
12. 条件编译
	- #ifndef, #define, #endif 预处理指令
13. 字符串处理
	- std::string 的使用
	- 正则表达式 (std::regex)
14. 内存管理
	- 动态内存分配
	- RAII 原则
15. IO 库
	+ 文件输入输出
	+ string 流

## C++17

以下是需要掌握的 C++ 17 知识点：

1. 类型安全的联合体 std::variant
	+ std::variant 的初始化和访问值
2. 折叠表达式

# 使用示例

首先，包含必要的头文件：

```cpp
#include <iostream>
#include <fstream>
#include "cppJSON.h"
```

## 1.JSON对象初始化

**1.JSON对象初始化**

```cpp
// 从JSON文件创建JSON对象
JSON j_from_file{"{}"};
std::ifstream file("./jsonfile1");
file >> j_from_file;

// 从字符串创建JSON对象
std::string jsonString = R"({"name": "Alice", "age": 30})";
JSON j_from_string{jsonString};

// 拷贝初始化
JSON j_copy = j_from_string;

// 打印JSON对象
std::cout<< j_from_file <<std::endl;
std::cout<< j_from_string <<std::endl;
std::cout<< j_copy <<std::endl;
```

输出：

```cpp
{"name":"Alice","age":30,"city":"BeiJing","isStudent":false,"height":1.85,"hobbies":["reading","swimming","hiking"],"con
tact":{"email":"Alice@example.com","phone":"123-4567-8901"}}
{"name":"Alice","age":30}
{"name":"Alice","age":30}
```

**2.JSON数组初始化**

```cpp
// 从JSON文件创建JSON数组
JSON j_from_file{"[]"};
std::ifstream file("./jsonfile2");
file >> j_from_file;

// 从字符串创建JSON数组
std::string jsonString = R"(["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"])";
JSON j_from_string{jsonString};

// 拷贝初始化
JSON j_copy = j_from_string;

// 打印JSON对象
std::cout<< j_from_file <<std::endl;
std::cout<< j_from_string <<std::endl;
std::cout<< j_copy <<std::endl;
```

输出：

```cpp
[1,2,3,4,5,6,7,["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]]
["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"]
["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"]
```

## 2.访问和修改JSON数据

**1.访问和修改JSON对象数据**

```cpp
// 创建JSON对象
JSON j{R"({
        "name": "Alice",
        "age": 30,
        "city": "BeiJing",
    })"};

// 添加新元素：JSON数组
j["hobbies"] = JSON{R"(["reading", "swimming"])"};

// 修改元素
j["age"] = 26;

// 删除元素
j.remove("city");

// 通过键和下标运算符访问元素
std::vector<std::string> keys = j.keys();
for (const auto &e: keys) {
    std::cout << "\"" << e << "\":" << j[e] << "," << std::endl;
}

// 合并JSON对象
j.merge(j, j);
std::cout << j << std::endl;
```

输出：

```cpp
"name":"Alice",
"age":26,
"hobbies":["reading","swimming"],
{"name":"Alice","age":26,"hobbies":["reading","swimming"],"name":"Alice","age":26,"hobbies":["reading","swimming"],"name
":"Alice","age":26,"hobbies":["reading","swimming"],"name":"Alice","age":26,"hobbies":["reading","swimming"]}
```

类型检查和转换：

```cpp
// 创建JSON对象
JSON j{R"({
        "name": "Alice",
        "age": 30,
        "city": "BeiJing",
        "isStudent": false,
        "height": 1.85,
    })"};

// 类型检查和转换
std::string name;
if(j["name"].isString()){
    name = static_cast<std::string>(j["name"]);

}
int age;
if(j["age"].isInteger()){
    age = static_cast<int>(j["age"]);

}
std::string city;
if(j["city"].isString()){
    city = static_cast<std::string>(j["city"]);
}
bool isStudent;
if(j["isStudent"].isBool()){
    isStudent = static_cast<bool>(j["isStudent"]);
}
double height;
if(j["height"].isFloat()){
    height = static_cast<double>(j["height"]);
}

std::vector<std::string> keys = j.keys();
std::cout << "\"" << keys[0] << "\":" << name << "," << std::endl;
std::cout << "\"" << keys[1] << "\":" << age << "," << std::endl;
std::cout << "\"" << keys[2] << "\":" << city << "," << std::endl;
std::cout << "\"" << keys[3] << "\":" << isStudent << "," << std::endl;
std::cout << "\"" << keys[4] << "\":" << height << "," << std::endl;
```

输出:

```cpp
"name":Alice,
"age":30,
"city":BeiJing,
"isStudent":0,
"height":1.85,
```

**2.访问和修改JSON数组数据**

```cpp
// 创建JSON数组
JSON j{R"([1,2,3])"};

// 添加新元素
j.push_back("four");

// 修改元素
j[0] = true;

// 删除元素
j.pop(1);

// 通过下标运算符访问JSON数组的值
for (size_t pos = 0; pos < j.size(); ++pos) {
    std::cout << j[pos] << "," << std::endl;
}
```

输出：

```cpp
true,
3,
"four",
```

类型检查和转换：

```cpp
// 创建JSON数组
JSON j{R"([1,2,3,"one","two","three"])"};

// 类型检查和转换
int num;
if (j[0].isInteger()) {
num = static_cast<int>(j[0]);

}
std::string str;
if (j[3].isString()) {
str = static_cast<string>(j[3]);

}

std::cout << num << std::endl;
std::cout << str << std::endl;
```

输出：

```cpp
1
one
```

# API文档

## JSON 类 API 文档

### 1. **构造函数**

- **`JSON(const string &str)`**
  - **功能**: 从给定的 JSON 格式字符串构造一个 `JSON` 对象，并解析该字符串。
  - **参数**:
    - `str`：JSON 格式的字符串。
  - **示例**:
    ```cpp
    JSON jsonObj("{\"name\": \"Alice\", \"age\": 30}");
    ```

- **`JSON(const char str[])`**
  - **功能**: 从 C 风格字符串构造一个 `JSON` 对象，并解析该字符串。
  - **参数**:
    - `str`：C 风格的 JSON 格式字符串。
  - **示例**:
    ```cpp
    JSON jsonObj("{\"name\": \"Alice\", \"age\": 30}");
    ```

- **`JSON(const Value &v)`**
  - **功能**: 从 `std::variant` 类型构造一个 `JSON` 对象，用于存储不同类型的 JSON 值。
  - **参数**:
    - `v`：`Value` 是一个 `std::variant` 类型，包含不同的 JSON 值类型（`JSONObject`、`JSONArray`、`IntValue`、`FloatValue`、`StringValue`、`BoolValue`、`NULLValue`）。
  - **示例**:
    ```cpp
    JSON jsonObj(JSON::Value(JSONObject("{\"key\": \"value\"}")));
    ```

### 2. **赋值操作符**

- **`JSON& operator=(const string &v)`**
  - **功能**: 将 `JSON` 对象赋值为一个字符串类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的字符串。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = "Hello, World!";
    ```

- **`JSON& operator=(const char v[])`**
  - **功能**: 将 `JSON` 对象赋值为一个 C 风格字符串类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的 C 风格字符串。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = "Hello, World!";
    ```

- **`JSON& operator=(const long double &v)`**
  - **功能**: 将 `JSON` 对象赋值为一个长浮点数类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的长浮点数。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = 3.14159L;
    ```

- **`JSON& operator=(const double &v)`**
  - **功能**: 将 `JSON` 对象赋值为一个双精度浮点数类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的双精度浮点数。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = 3.14159;
    ```

- **`JSON& operator=(const long long &v)`**
  - **功能**: 将 `JSON` 对象赋值为一个长整数类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的长整数。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = 123456789LL;
    ```

- **`JSON& operator=(const int &v)`**
  - **功能**: 将 `JSON` 对象赋值为一个整数类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的整数。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = 42;
    ```

- **`JSON& operator=(const bool &v)`**
  - **功能**: 将 `JSON` 对象赋值为一个布尔类型的 JSON 值。
  - **参数**:
    - `v`：要赋值的布尔值。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = true;
    ```

- **`JSON& operator=(std::nullptr_t v)`**
  - **功能**: 将 `JSON` 对象赋值为 `null` 类型的 JSON 值。
  - **参数**:
    - `v`：`nullptr`，表示 `null`。
  - **示例**:
    ```cpp
    JSON jsonObj;
    jsonObj = nullptr;
    ```

- **`JSON& operator=(const JSON &json)`**
  - **功能**: 将一个 `JSON` 对象赋值给另一个 `JSON` 对象。
  - **参数**:
    - `json`：另一个 `JSON` 对象。
  - **示例**:
    ```cpp
    JSON jsonObj1("{\"key\": \"value\"}");
    JSON jsonObj2;
    jsonObj2 = jsonObj1;
    ```

### 3. **访问和修改 JSON 对象/数组**

- **`JSON& operator[](const string &key)`**
  - **功能**: 访问或修改 `JSON` 对象中的元素，通过键值查找。
  - **参数**:
    - `key`：`JSON` 对象的键。
  - **返回**: 以引用的形式返回对应键的 `JSON` 值。
  - **示例**:
    ```cpp
    JSON jsonObj("{\"name\": \"Alice\"}");
    jsonObj["name"] = "Jack";
    ```

- **`JSON& operator[](const char key[])`**
  - **功能**: 访问或修改 `JSON` 对象中的元素，通过 C 风格字符串键值查找。
  - **参数**:
    - `key`：`JSON` 对象的键，C 风格字符串。
  - **返回**: 以引用的形式返回对应键的 `JSON` 值。
  - **示例**:
    ```cpp
    JSON jsonObj("{\"name\": \"Alice\"}");
    jsonObj["name"] = "Jack";
    ```

- **`JSON& at(const string &key)`**
  - **功能**: 访问 `JSON` 对象中的元素，类似于 `operator[]`，但如果键不存在会抛出异常。
  - **参数**:
    - `key`：`JSON` 对象的键。
  - **返回**: 以引用的形式返回对应键的 `JSON` 值。
  - **示例**:
    ```cpp
    JSON jsonObj("{\"name\": \"Alice\"}");
    try {
        JSON &value = jsonObj.at("name");
    } catch (std::out_of_range &e) {
        std::cerr << "Key not found!" << std::endl;
    }
    ```

- **`JSON& at(const char key[])`**
  - **功能**: 访问 `JSON` 对象中的元素，类似于 `operator[]`，但如果键不存在会抛出异常。
  - **参数**:
    - `key`：`JSON` 对象的键，C 风格字符串。
  - **返回**: 以引用的形式返回对应键的 `JSON` 值。
  - **示例**:
    ```cpp
    JSON jsonObj("{\"name\": \"Alice\"}");
    try {
        JSON &value = jsonObj.at("name");
    } catch (std::out_of_range &e) {
        std::cerr << "Key not found!" << std::endl;
    }
    ```

- **`JSON& operator[](const int &index)`**
  - **功能**: 访问或修改 `JSON` 数组中的元素，通过索引查找。
  - **参数**:
    - `index`：数组索引，从 0 开始。
  - **返回**: 以引用的形式返回对应索引的 `JSON` 值。
  - **示例**:
    ```cpp
    JSON jsonArray("[1, 2, 3]");
    jsonArray[1] = 4; // 修改第二个元素为 4
    ```

### 4. **容器操作**

- **`template<typename T> void push_back(const T &value)`**
  - **功能**: 向 `JSON` 数组中追加一个元素。
  - **参数**:
    - `value`：要添加的 `JSON` 值。
  - **示例**:
    ```cpp
    JSON jsonArray("[]");
    jsonArray.push_back(42);
    jsonArray.push_back("new element");
    ```

- **`bool pop(int pos)`**
  - **功能**: 从 `JSON` 数组中删除指定索引位置的元素。
  - **参数**:
    - `pos`：要删除的元素的索引。
  - **返回**: 删除成功返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonArray("[1, 2, 3]");
    jsonArray.pop(1);
    
    // 删除第二个元素，数组变为 [1, 3]
    ```

### 5. **类型检查**

- **`bool isString()`**
  - **功能**: 检查 `JSON` 值是否为字符串类型。
  - **返回**: 如果 `JSON` 值为字符串类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = "Hello";
    if (jsonObj.isString()) {
        std::cout << "This is a string!" << std::endl;
    }
    ```

- **`bool isInteger()`**
  - **功能**: 检查 `JSON` 值是否为整数类型。
  - **返回**: 如果 `JSON` 值为整数类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = 42;
    if (jsonObj.isInteger()) {
        std::cout << "This is an integer!" << std::endl;
    }
    ```

- **`bool isFloat()`**
  - **功能**: 检查 `JSON` 值是否为浮点数类型。
  - **返回**: 如果 `JSON` 值为浮点数类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = 3.14;
    if (jsonObj.isFloat()) {
        std::cout << "This is a float!" << std::endl;
    }
    ```

- **`bool isBool()`**
  - **功能**: 检查 `JSON` 值是否为布尔类型。
  - **返回**: 如果 `JSON` 值为布尔类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = true;
    if (jsonObj.isBool()) {
        std::cout << "This is a boolean!" << std::endl;
    }
    ```

- **`bool isNULL()`**
  - **功能**: 检查 `JSON` 值是否为 `null` 类型。
  - **返回**: 如果 `JSON` 值为 `null` 类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = nullptr;
    if (jsonObj.isNULL()) {
        std::cout << "This is a null value!" << std::endl;
    }
    ```

- **`bool isJSONObject()`**
  - **功能**: 检查 `JSON` 值是否为对象类型。
  - **返回**: 如果 `JSON` 值为对象类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = "{\"key\": \"value\"}";
    if (jsonObj.isJSONObject()) {
        std::cout << "This is a JSON object!" << std::endl;
    }
    ```

- **`bool isJSONArray()`**
  - **功能**: 检查 `JSON` 值是否为数组类型。
  - **返回**: 如果 `JSON` 值为数组类型，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = "[1, 2, 3]";
    if (jsonObj.isJSONArray()) {
        std::cout << "This is a JSON array!" << std::endl;
    }
    ```

### 6. **常用操作**

- **`bool empty() const`**
  - **功能**: 检查 `JSON` 对象或数组是否为空。
  - **返回**: 如果 `JSON` 对象或数组为空，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = "{}";
    if (jsonObj.empty()) {
        std::cout << "The JSON object is empty!" << std::endl;
    }
    ```

- **`size_t size() const`**
  - **功能**: 返回 `JSON` 对象中键值对的数量或数组中的元素个数。
  - **返回**: `JSON` 对象中键值对的数量或数组中的元素个数。
  - **示例**:
    ```cpp
    JSON jsonArray = "[1, 2, 3]";
    std::cout << "Array size: " << jsonArray.size() << std::endl;
    ```

- **`vector<string> keys()`**
  - **功能**: 获取 `JSON` 对象中所有键的集合。
  - **返回**: 包含 `JSON` 对象中所有键的 `vector<string>`。
  - **示例**:
    ```cpp
    JSON jsonObj = "{\"name\": \"Alice\", \"age\": 30}";
    vector<string> keys = jsonObj.keys();
    for (const auto &key : keys) {
        std::cout << key << std::endl;
    }
    ```

- **`bool remove(const string &key)`**
  - **功能**: 从 `JSON` 对象中移除指定键的键值对。
  - **参数**:
    - `key`：要移除的键。
  - **返回**: 如果键存在并成功移除，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = "{\"name\": \"Alice\", \"age\": 30}";
    jsonObj.remove("age");
    ```

- **`bool remove(const char key[])`**
  - **功能**: 从 `JSON` 对象中移除指定 C 风格字符串键的键值对。
  - **参数**:
    - `key`：要移除的 C 风格字符串键。
  - **返回**: 如果键存在并成功移除，返回 `true`，否则返回 `false`。
  - **示例**:
    ```cpp
    JSON jsonObj = "{\"name\": \"Alice\", \"age\": 30}";
    jsonObj.remove("age");
    ```

- **`template<typename ...args> JSON& merge(const args &... json_list)`**
  - **功能**: 将多个 `JSON` 对象合并到当前对象中。如果有重复的键，后面的 `JSON` 对象会覆盖前面的键值对。
  - **参数**:
    - `json_list`：一个或多个 `JSON` 对象。
  - **返回**: 返回合并后的 `JSON` 对象。
  - **示例**:
    ```cpp
    JSON jsonObj1 = "{\"name\": \"Alice\"}";
    JSON jsonObj2 = "{\"age\": 30}";
    jsonObj1.merge(jsonObj2);
    ```

### 7. **类型转换操作符**

- **`explicit operator string() const`**
  - **功能**: 将 `JSON` 值转换为字符串类型。
  - **返回**: `JSON` 对应的字符串表示。
  - **示例**:
    ```cpp
    JSON jsonObj = "Hello";
    string strValue = static_cast<string>(jsonObj);
    ```

- **`explicit operator int() const`**
  - **功能**: 将 `JSON` 值转换为整数类型。
  - **返回**: `JSON` 对应的整数表示。
  - **示例**:
    ```cpp
    JSON jsonObj = 42;
    int intValue = static_cast<int>(jsonObj);
    ```

- **`explicit operator long long() const`**
  - **功能**: 将 `JSON` 值转换为长整数类型。
  - **返回**: `JSON` 对应的长整数表示。
  - **示例**:
    ```cpp
    JSON jsonObj = 123456789LL;
    long long longValue = static_cast<long long>(jsonObj);
    ```

- **`explicit operator long double() const`**
  - **功能**: 将 `JSON` 值转换为长浮点数类型。
  - **返回**: `JSON` 对应的长浮点数表示。
  - **示例**:
    ```cpp
    JSON jsonObj = 3.14159L;
    long double longDoubleValue = static_cast<long double>(jsonObj);
    ```

- **`explicit operator double() const`**
  - **功能**: 将 `JSON` 值转换为双精度浮点数类型。
  - **返回**: `JSON` 对应的双精度浮点数表示。
  - **示例**:
    ```cpp
    JSON jsonObj = 3.14159;
    double doubleValue = static_cast<double>(jsonObj);
    ```

- **`explicit operator bool() const`**
  - **功能**: 将 `JSON` 值转换为布尔类型。
  - **返回**: `JSON` 对应的布尔表示。
  - **示例**:
    ```cpp
    JSON jsonObj = true;
    bool boolValue = static_cast<bool>(jsonObj);
    ```

