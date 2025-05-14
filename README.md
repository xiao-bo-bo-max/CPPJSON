# CPPJSON: 一款轻量级的C++ JSON库

## 项目简介

CPPJSON 是一个轻量级、高效的 C++ JSON 解析库。它允许开发者轻松地将 JSON 格式的字符串或文件内容转换为 C++ 对象，并支持将 C++ 对象序列化为 JSON 格式。该库旨在提供直观的 API、快速的解析速度以及最小的依赖性，使其适合在多种C++项目中使用，包括资源受限的环境。

**版本依赖**：CPPJSON 主要基于 C++11 开发，同时利用了部分 C++17 的特性（如 `std::variant` 和折叠表达式）。

## 功能描述

1.  **解析JSON数据**：
	  * 支持从 JSON 格式的字符串直接解析。
	  * 支持从文件流 (`std::ifstream`) 读取并解析 JSON 数据。
	  * 能够处理嵌套的 JSON 对象和 JSON 数组。
2.  **数据操作**：
	  * **访问与修改**：通过键 (对于 JSON 对象) 或索引 (对于 JSON 数组) 方便地访问和修改数据。
	  * **添加元素**：向 JSON 对象中添加新的键值对，或向 JSON 数组中追加元素。
	  * **删除元素**：从 JSON 对象中移除指定的键值对，或从 JSON 数组中删除指定位置的元素。
	  * **获取键集合**：获取 JSON 对象中所有的键。
3.  **对象管理**：
	  * **拷贝**：支持 JSON 对象的深拷贝（通过拷贝构造函数和拷贝赋值运算符）。
	  * **合并**：支持将一个或多个 JSON 对象合并到当前 JSON 对象中（目前实现为键值对的追加）。
4.  **类型支持与检查**：
	  * 支持常见的 JSON 数据类型：字符串、整数、浮点数、布尔值、null、JSON 对象和 JSON 数组。
	  * 提供类型检查方法（如 `isString()`, `isInteger()`, `isJSONObject()` 等）。
	  * 支持显式类型转换到对应的 C++ 类型（如 `static_cast<std::string>(json_value)`）。
5.  **序列化**：
	  * 支持将 JSON 对象或数组打印（序列化）到输出流 (`std::ostream`)，例如控制台或文件。
6.  **便捷操作**：
	  * 检查 JSON 对象或数组是否为空 (`empty()`)。
	  * 获取 JSON 对象或数组的大小 (`size()`)。

## 应用技术

CPPJSON 的实现主要依赖以下 C++ 特性：

### C++11

  * **类与继承**：定义了 `BaseValue` 作为所有 JSON 值类型的基类，派生出 `IntValue`, `FloatValue`, `StringValue`, `BoolValue`, `NULLValue`, `JSONObject`, `JSONArray` 等具体类型。使用了虚函数和多态。
  * **智能指针**：广泛使用 `std::shared_ptr` 来管理动态分配的 JSON 值对象的生命周期，简化内存管理。
  * **STL 容器**：
	* `std::vector<std::string>` 用于存储 `JSONObject` 的键。
	* `std::vector<std::shared_ptr<JSON>>` 用于存储 `JSONObject` 的值和 `JSONArray` 的元素。
  * **运算符重载**：
	* 输入/输出流运算符 (`<<`, `>>`) 用于序列化和反序列化。
	* 下标运算符 (`[]`) 用于访问对象和数组元素。
	* 赋值运算符 (`=`) 用于赋值不同类型的值给 `JSON` 对象。
	* 比较运算符 (`==`, `!=`) 用于比较两个 `JSON` 对象是否相等。
  * **类型转换**：使用 `explicit` 关键字定义类型转换操作符，避免不期望的隐式转换。
  * **异常处理**：使用 `std::runtime_error` 和 `std::out_of_range` 等异常来报告解析错误或运行时错误。
  * **右值引用与移动语义**：虽然在提供的代码片段中不明显，但现代C++库通常会考虑这些以提高性能。
  * **Lambda 表达式**：用于 `std::visit` 等场景，简化代码。
  * **`nullptr`**：用于表示 JSON null 值。

### C++17

  * **`std::variant`**：核心类 `JSON` 使用 `std::variant` 来存储其实际的 JSON 值（`JSONObject`, `JSONArray`, `IntValue` 等）。这提供了一种类型安全的方式来表示和操作多种不同的数据类型。
  * **折叠表达式 (Fold Expressions)**：用于 `JSON::merge` 方法中，优雅地处理可变参数模板，对每个传入的 JSON 对象执行合并操作。
  * **`if constexpr`**：虽然在提供的代码片段中不明显，但它是C++17中处理模板元编程和条件编译的有用特性。

## 项目的代码逻辑和实现思路

### 核心设计

CPPJSON 库的核心是 `JSON` 类，它充当了所有 JSON 数据类型的统一接口。其内部使用 `std::variant` (别名为 `JSON::Value`) 来实际存储不同类型的 JSON 数据。这种设计避免了复杂的继承链或者需要手动进行类型转换和检查的联合体（union）。

### 值类型表示

  * **`BaseValue` 类**：这是一个抽象基类，定义了所有 JSON 值类型共有的接口，主要是 `valueType()` 方法，用于返回值的具体类型（通过预定义的宏如 `STRING_TYPE`, `JSON_OBJECT_TYPE` 等）。
  * **具体值类**：
	* `IntValue`, `FloatValue`, `StringValue`, `BoolValue`, `NULLValue`：这些类直接继承自 `BaseValue`，并存储相应的原始 C++ 类型数据。它们提供了到其内部存储类型的显式转换操作符。
	* `JSONObject`：表示 JSON 对象。内部使用两个 `std::vector`：一个存储键 (`std::string`)，另一个存储对应的值 (`std::shared_ptr<JSON>`)。这允许 JSON 对象的值本身也是一个 `JSON` 对象（从而支持嵌套）。
	* `JSONArray`：表示 JSON 数组。内部使用一个 `std::vector<std::shared_ptr<JSON>>` 来存储其元素，同样支持嵌套。

### 解析 (`parse_value` 函数及构造函数)

  * 解析逻辑主要集中在全局辅助函数 `parse_value` 中。此函数接收一个 JSON 字符串和当前解析位置的引用。
  * 它通过检查当前位置的字符来判断接下来要解析的值的类型：
	* `"`：表示字符串。
	* `{`：表示 JSON 对象。
	* `[`：表示 JSON 数组。
	* 数字、`-`、`+`：表示整数或浮点数。
	* `t` (true), `f` (false)：表示布尔值。
	* `n` (null)：表示 null 值。
  * 根据识别的类型，它会提取相应的子字符串或值，并构造一个包含相应具体值类型 (`StringValue`, `JSONObject`, `IntValue` 等封装在 `JSON::Value` 中) 的 `JSON` 对象的 `shared_ptr`。
  * `JSONObject` 和 `JSONArray` 的构造函数会调用 `parse_value` 来递归解析其成员或元素。
  * `JSON` 类的构造函数 (`JSON(const string &str)`) 会根据输入字符串的第一个非空字符（`{` 或 `[`）来决定是创建一个 `JSONObject` 还是 `JSONArray`。

### 数据操作与访问

  * **`JSON` 类的操作符**：
	* `operator[]`：为 `JSON` 对象提供了通过键（字符串）或索引（整数）访问其内部 `JSONObject` 或 `JSONArray` 元素的能力。它通过 `std::visit` 将操作委托给底层的 `std::variant` 中存储的实际对象。如果键或索引不存在，`JSONObject::operator[]` 会创建一个新的 null 值条目。
	* `at()`：提供了与 `operator[]` 类似的功能，但在键或索引不存在时会抛出 `std::out_of_range` 异常。
	* 赋值操作符 (`operator=`)：允许将各种 C++ 基本类型和 `JSON` 对象赋值给一个 `JSON` 实例，内部会创建相应的值类型并存储在 `std::variant` 中。
  * **类型检查**：`isString()`, `isInteger()`, `isJSONObject()` 等方法通过访问 `std::variant` 中存储的对象的 `valueType()` 来判断当前 `JSON` 实例持有的具体数据类型。
  * **`JSONObject::keys()`**：返回对象中所有键的列表。
  * **`JSONArray::push_back()` 和 `JSON::push_back()`**：用于向数组添加元素。`JSON::push_back()` 会首先检查自身是否为数组类型。
  * **`JSONObject::remove()` 和 `JSON::remove()` / `JSONArray::pop()` 和 `JSON::pop()`**：用于删除元素。

### 序列化 (`operator<<`)

  * 为每个具体值类型 (`StringValue`, `IntValue`, `JSONObject`, `JSONArray` 等) 以及 `JSON` 类本身都重载了 `operator<<`。
  * `JSON::operator<<` 使用 `std::visit` 将输出操作委托给 `std::variant` 中存储的实际对象的 `operator<<`。
  * `JSONObject::operator<<` 和 `JSONArray::operator<<` 会遍历其内部元素，并递归调用对应元素的 `operator<<`，同时正确地添加逗号、引号、花括号和方括号，以生成符合 JSON 格式的字符串。

### 内存管理

  * 通过使用 `std::shared_ptr<JSON>` 来存储 `JSONObject` 中的值和 `JSONArray` 中的元素，实现了自动内存管理。当一个 `JSON` 结构（可能是嵌套的）不再被引用时，其占用的内存会被自动回收。
  * 拷贝构造函数和拷贝赋值运算符为 `JSONObject` 和 `JSONArray` 实现了深拷贝，确保对象复制时其内容也被完全复制，而不是共享底层数据。

### C++17 特性的运用

  * **`std::variant`** 是此库设计的核心。它允许 `JSON` 类以类型安全的方式持有不同种类的 JSON 值，而无需使用容易出错的 `void*` 或手写鉴别联合体。`std::visit` 被用来在 `variant` 上执行类型安全的操作。
  * **折叠表达式** 在 `JSON::merge` 中简化了对可变参数模板的处理，使得可以简洁地对一系列传入的 `JSON` 对象执行合并操作。

## 使用示例

首先，包含必要的头文件：

```cpp
#include <iostream>
#include <fstream>
#include "cppJSON.h" // 假设 cppJSON.h 和 cppJSON.cpp 在同一目录或正确配置了包含路径
```

### 1\. JSON 对象初始化与解析

```cpp
// 从字符串创建JSON对象
std::string jsonObjectString = R"({"name": "Alice", "age": 30, "city": "New York"})";
JSON jsonObjFromString(jsonObjectString);
std::cout << "From string: " << jsonObjFromString << std::endl;

// 从文件创建JSON对象 (假设 jsonfile1 内容为 {"item": "laptop", "price": 1200.50})
JSON jsonObjFromFile("{}"); // 初始化为空对象，然后通过流读入
std::ifstream file_obj("../jsonfile1"); // 确保文件路径正确
if (file_obj.is_open()) {
    file_obj >> jsonObjFromFile;
    std::cout << "From file: " << jsonObjFromFile << std::endl;
    file_obj.close();
} else {
    std::cerr << "Could not open jsonfile1" << std::endl;
}

// 拷贝初始化
JSON jsonObjCopy = jsonObjFromString;
jsonObjCopy["name"] = "Bob"; // 修改副本，不影响原对象
std::cout << "Original after copy: " << jsonObjFromString << std::endl;
std::cout << "Copied and modified: " << jsonObjCopy << std::endl;
```

### 2\. JSON 数组初始化与解析

```cpp
// 从字符串创建JSON数组
std::string jsonArrayString = R"(["apple", "banana", 100, true, null])";
JSON jsonArrFromString(jsonArrayString);
std::cout << "Array from string: " << jsonArrFromString << std::endl;

// 从文件创建JSON数组 (假设 jsonfile2 内容为 [1, 2, "test", {"nested_key": "nested_value"}])
JSON jsonArrFromFile("[]"); // 初始化为空数组
std::ifstream file_arr("../jsonfile2"); // 确保文件路径正确
if (file_arr.is_open()) {
    file_arr >> jsonArrFromFile;
    std::cout << "Array from file: " << jsonArrFromFile << std::endl;
    file_arr.close();
} else {
    std::cerr << "Could not open jsonfile2" << std::endl;
}
```

### 3\. 访问和修改JSON数据

#### JSON 对象

```cpp
JSON user{R"({
    "name": "Charlie",
    "details": {
        "age": 25,
        "isStudent": false
    },
    "scores": [88, 92, 75]
})"};

// 访问元素
std::cout << "Name: " << user["name"] << std::endl;
std::cout << "Age: " << user["details"]["age"] << std::endl;

// 修改元素
user["details"]["age"] = 26;
user["city"] = "London"; // 添加新键值对

// 添加复杂元素 (另一个JSON对象或数组)
JSON address{R"({"street": "123 Main St", "zip": "10001"})"};
user["address"] = address;

// 删除元素
user.remove("isStudent"); // 假设要从顶层移除 (如果存在)
user["details"].remove("isStudent");


std::cout << "Updated user: " << user << std::endl;

// 获取所有键
std::cout << "User keys: ";
for (const auto& key : user.keys()) {
    std::cout << key << " ";
}
std::cout << std::endl;

// 合并JSON对象
JSON contactInfo{R"({"email": "charlie@example.com"})"};
user.merge(contactInfo);
std::cout << "Merged user: " << user << std::endl;
```

#### JSON 数组

```cpp
JSON numbers{R"([10, 20, 30])"};
std::cout << "Initial array: " << numbers << std::endl;

// 访问元素
std::cout << "First element: " << numbers[0] << std::endl;

// 修改元素
numbers[1] = 25;

// 添加元素
numbers.push_back(40);
numbers.push_back("fifty");
JSON nestedObj{R"({"value": 60})"};
numbers.push_back(nestedObj);

std::cout << "Array after push_back: " << numbers << std::endl;

// 删除元素 (by index)
numbers.pop(0); // 删除第一个元素 (10)
std::cout << "Array after pop(0): " << numbers << std::endl;

// 遍历数组
std::cout << "Array elements: ";
for (size_t i = 0; i < numbers.size(); ++i) {
    std::cout << numbers[i] << (i == numbers.size() - 1 ? "" : ", ");
}
std::cout << std::endl;
```

### 4\. 类型检查和转换

```cpp
JSON data{R"({
    "id": 123,
    "label": "Sample",
    "isActive": true,
    "value": 99.9,
    "tags": ["A", "B"],
    "extra": null
})"};

if (data["id"].isInteger()) {
    long long id = static_cast<long long>(data["id"]);
    std::cout << "ID (long long): " << id << std::endl;
}

if (data["label"].isString()) {
    std::string label = static_cast<std::string>(data["label"]);
    std::cout << "Label (string): " << label << std::endl;
}

if (data["isActive"].isBool()) {
    bool isActive = static_cast<bool>(data["isActive"]);
    std::cout << "IsActive (bool): " << (isActive ? "true" : "false") << std::endl;
}

if (data["value"].isFloat()) {
    double value = static_cast<double>(data["value"]);
    std::cout << "Value (double): " << value << std::endl;
}

if (data["tags"].isJSONArray()) {
    std::cout << "Tags (array size): " << data["tags"].size() << std::endl;
}

if (data["extra"].isNULL()) {
    std::cout << "Extra is null." << std::endl;
}

// 尝试访问不存在的键 (operator[]会创建null)
if (data["nonexistent"].isNULL()) {
    std::cout << "Accessing 'nonexistent' created a null value." << std::endl;
}
// std::cout << data["nonexistent"] << std::endl; // 会打印 null

// 尝试使用at()访问不存在的键 (会抛异常)
try {
    JSON val = data.at("another_nonexistent");
} catch (const std::out_of_range& e) {
    std::cerr << "Exception for at('another_nonexistent'): " << e.what() << std::endl;
}
```

## API 文档 (摘要)

(请参考 `cppJSON.h` 中各类的声明和成员函数的注释以获取详细的API信息。)

### `JSON` 类

  * **构造函数**:
	* `JSON(const string &str)`: 从JSON字符串构造。
	* `JSON(const char str[])`: 从C风格JSON字符串构造。
  * **赋值操作符**: `operator=` 重载以接受 `std::string`, `const char[]`, `long double`, `double`, `long long`, `int`, `bool`, `std::nullptr_t`, 和 `const JSON&`。
  * **访问**:
	* `JSON& operator[](const string &key)` / `JSON& operator[](const char key[])`: 对象元素访问/创建。
	* `JSON& operator[](const int &index)`: 数组元素访问。
	* `JSON& at(const string &key)` / `JSON& at(const char key[])`: 带边界检查的对象元素访问。
  * **类型检查**: `isString()`, `isInteger()`, `isFloat()`, `isBool()`, `isNULL()`, `isJSONObject()`, `isJSONArray()`.
  * **类型转换**: `explicit operator string() const`, `int()`, `long long()`, `double()`, `long double()`, `bool()`.
  * **通用操作**:
	* `bool empty() const`: 检查是否为空 (对象或数组)。
	* `size_t size() const`: 获取大小 (对象或数组)。
  * **对象操作**:
	* `vector<string> keys()`: 获取所有键。
	* `bool remove(const string &key)` / `bool remove(const char key[])`: 删除键值对。
	* `JSON& merge(const args &... json_list)`: 合并其他JSON对象。
  * **数组操作**:
	* `template<typename T> void push_back(const T &value)`: 向数组末尾添加元素。
	* `bool pop(int pos)`: 删除指定索引的元素。

### 辅助类 (`JSONObject`, `JSONArray`, `IntValue`, etc.)

这些类主要由 `JSON` 类内部使用，通过 `std::variant` 进行管理。它们各自实现了特定JSON类型的行为和数据存储。

## 构建和运行

项目使用 CMake 进行构建。

1.  **确保CMake已安装。**

2.  在项目根目录下创建一个构建目录并进入：

	```bash
	mkdir build
	cd build
	```

3.  运行CMake生成构建文件：

	```bash
	cmake ..
	```

4.  编译项目：

	```bash
	cmake --build .
	# 或者使用 make (在Unix-like系统上)
	# make
	```

5.  可执行文件 (默认为 `CPPJSON`，如 `CMakeLists.txt` 所示) 将会生成在构建目录中。
	运行示例 (如 `main.cpp`):

	```bash
	./CPPJSON
	```

	确保 `main.cpp` 中引用的JSON文件 (`../jsonfile1`, `../jsonfile2`) 相对于可执行文件的运行路径是正确的，或者修改 `main.cpp` 中的文件路径。

## 未来可能的改进

  * **更完善的错误处理与报告**：提供更详细的解析错误信息，包括行号和列号。
  * **Unicode 支持**：完整支持UTF-8编码的字符串解析和序列化。
  * **性能优化**：针对大型JSON文档的解析和序列化进行性能分析和优化。
  * **Schema 验证**：支持根据JSON Schema验证JSON数据的有效性。
  * **注释支持**：允许解析和忽略JSON中的注释（虽然标准JSON不支持注释）。
  * **自定义序列化/反序列化**：为用户自定义类型提供更方便的序列化和反序列化挂钩。
  * **更细致的合并策略**：为 `JSONObject::merge` 提供不同的合并策略（例如，深度合并，冲突解决回调）。
