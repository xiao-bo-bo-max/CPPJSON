# Project Overview

**Project Name:** CPPJSON

**Project Description:** CPPJSON is a lightweight and efficient C++ JSON parsing library. It allows developers to easily convert JSON-formatted strings into C++ objects and supports serializing C++ objects into JSON format. The library aims to provide an intuitive API, fast parsing speed, and minimal dependencies, making it suitable for embedded systems and resource-constrained environments.

**Version Dependencies:** CPPJSON is primarily developed based on C++11, with some features of C++17 utilized.

# Features Description

1. **Parse JSON-formatted strings:** Parse JSON-formatted strings or JSON files into C++ objects: JSON. JSON-formatted strings or JSON files support nested JSON objects and JSON arrays.
2. **Copy and merge JSON:** Copy JSON objects (arrays) into another JSON object (array) and merge multiple JSON objects into one.
3. **Add, delete, modify, and query JSON data:** Add, delete, modify, and access a key-value pair of a JSON object or an element of a JSON array.
4. **Print JSON objects:** Print a specific value of a JSON object or an element of a JSON array; print all key-value pairs of a JSON object or all elements of a JSON array.

# Applied Technologies

## C++11

The following are the C++11 knowledge points that need to be mastered:

1. Classes and Inheritance
	- Class definition and implementation
	- Inheritance and polymorphism
	- Virtual functions and pure virtual functions
2. Templates
	- Function templates
	- Variadic templates
3. Smart Pointers
	- Use of `std::shared_ptr`
4. STL Containers
	- Use of `std::vector`
5. Operator Overloading
	- Input/output operators (`<<`, `>>`)
	- Subscript operator (`[]`)
	- Assignment operator (`=`)
	- Comparison operators (`==`, `!=`)
6. Type Casting
	- Explicit type conversion operators
	- `static_cast`
7. Exception Handling
	- `throw` statement
	- `std::runtime_error`
8. Friend Functions and Friend Classes
9. Namespaces
	- `using` declarations
10. Function Objects and Lambda Expressions
11. Type Traits
	- `std::is_same`
12. Conditional Compilation
	- `#ifndef`, `#define`, `#endif` preprocessor directives
13. String Handling
	- Use of `std::string`
	- Regular expressions (`std::regex`)
14. Memory Management
	- Dynamic memory allocation
	- RAII principle
15. IO Library
	- File input/output
	- String streams

## C++17

The following are the C++17 knowledge points that need to be mastered:

1. Type-safe Union `std::variant`
  - Initialization and value access of `std::variant`
2. Fold Expressions

# Example usage

Firstly, include the necessary header files:

```cpp
#include <iostream>
#include <fstream>
#include "cppJSON.h"
```

## 1. JSON object initialization

**1. JSON object initialization**

```cpp
// Create JSON objects from JSON files
JSON j_from_file{"{}"};
std::ifstream file("./jsonfile1");
file >> j_from_file;

// Create JSON object from string
std::string jsonString = R"({"name": "Alice", "age": 30})";
JSON j_from_string{jsonString};

// copy initialization
JSON j_copy = j_from_string;

// Print JSON object
std::cout<< j_from_file <<std::endl;
std::cout<< j_from_string <<std::endl;
std::cout<< j_copy <<std::endl;
```

Output:

```cpp
{"name":"Alice","age":30,"city":"BeiJing","isStudent":false,"height":1.85,"hobbies":["reading","swimming","hiking"],"con
tact":{"email":"Alice@example.com","phone":"123-4567-8901"}}
{"name":"Alice","age":30}
{"name":"Alice","age":30}
```

**2. JSON array initialization**

```cpp
// Create a JSON array from a JSON file
JSON j_from_file{"[]"};
std::ifstream file("./jsonfile2");
file >> j_from_file;

// Create a JSON array from a string
std::string jsonString = R"(["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"])";
JSON j_from_string{jsonString};

// copy initialization
JSON j_copy = j_from_string;

// Print JSON object
std::cout<< j_from_file <<std::endl;
std::cout<< j_from_string <<std::endl;
std::cout<< j_copy <<std::endl;
```

Output:

```cpp
[1,2,3,4,5,6,7,["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]]
["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"]
["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"]
```

## 2. Accessing and modifying JSON data

**1. Accessing and modifying JSON object data**

```cpp
// Create JSON object
JSON j{R"({
        "name": "Alice",
        "age": 30,
        "city": "BeiJing",
    })"};

// Add new element: JSON array
j["hobbies"] = JSON{R"(["reading", "swimming"])"};

// Modify elements
j["age"] = 26;

// Delete element
j.remove("city");

// Accessing elements through keys and subscript operators
std::vector<std::string> keys = j.keys();
for (const auto &e: keys) {
    std::cout << "\"" << e << "\":" << j[e] << "," << std::endl;
}

// Merge JSON objects
j.merge(j, j);
std::cout << j << std::endl;
```

Output:

```cpp
"name":"Alice",
"age":26,
"hobbies":["reading","swimming"],
{"name":"Alice","age":26,"hobbies":["reading","swimming"],"name":"Alice","age":26,"hobbies":["reading","swimming"],"name
":"Alice","age":26,"hobbies":["reading","swimming"],"name":"Alice","age":26,"hobbies":["reading","swimming"]}
```

Type checking and conversion:

```cpp
// Create JSON object
JSON j{R"({
        "name": "Alice",
        "age": 30,
        "city": "BeiJing",
        "isStudent": false,
        "height": 1.85,
    })"};

// Type checking and conversion
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

Output:

```cpp
"name":Alice,
"age":30,
"city":BeiJing,
"isStudent":0,
"height":1.85,
```

**2. Accessing and modifying JSON array data**

```cpp
// Create JSON array
JSON j{R"([1,2,3])"};

// Add new elements
j.push_back("four");

// Modify elements
j[0] = true;

// Delete element
j.pop(1);

// Accessing JSON array values through subscript operators
for (size_t pos = 0; pos < j.size(); ++pos) {
    std::cout << j[pos] << "," << std::endl;
}
```

Output:

```cpp
true,
3,
"four",
```

Type checking and conversion:

```cpp
// Create JSON array
JSON j{R"([1,2,3,"one","two","three"])"};

// Type checking and conversion
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

Output:

```cpp
1
one
```

# API Documentation

## JSON Class API Documentation

### 1. **Constructors**

- **`JSON(const string &str)`**

	- **Function**: Constructs a `JSON` object from a given JSON-formatted string and parses the string.

	- **Parameters**:

		- `str`: A JSON-formatted string.

	- **Example**:

		```cpp
		JSON jsonObj("{\"name\": \"Alice\", \"age\": 30}");
		```

- **`JSON(const char str[])`**

	- **Function**: Constructs a `JSON` object from a C-style string and parses the string.

	- **Parameters**:

		- `str`: A C-style JSON-formatted string.

	- **Example**:

		```cpp
		JSON jsonObj("{\"name\": \"Alice\", \"age\": 30}");
		```

- **`JSON(const Value &v)`**

	- **Function**: Constructs a `JSON` object from a `std::variant` type, used to store different types of JSON values.

	- **Parameters**:

		- `v`: `Value` is a `std::variant` type containing different JSON value types (`JSONObject`, `JSONArray`, `IntValue`, `FloatValue`, `StringValue`, `BoolValue`, `NULLValue`).

	- **Example**:

		```cpp
		JSON jsonObj(JSON::Value(JSONObject("{\"key\": \"value\"}")));
		```

### 2. **Assignment Operators**

- **`JSON& operator=(const string &v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of string type.

	- **Parameters**:

		- `v`: The string to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = "Hello, World!";
		```

- **`JSON& operator=(const char v[])`**

	- **Function**: Assigns a `JSON` object with a JSON value of C-style string type.

	- **Parameters**:

		- `v`: The C-style string to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = "Hello, World!";
		```

- **`JSON& operator=(const long double &v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of long double type.

	- **Parameters**:

		- `v`: The long double value to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = 3.14159L;
		```

- **`JSON& operator=(const double &v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of double type.

	- **Parameters**:

		- `v`: The double value to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = 3.14159;
		```

- **`JSON& operator=(const long long &v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of long long type.

	- **Parameters**:

		- `v`: The long long value to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = 123456789LL;
		```

- **`JSON& operator=(const int &v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of int type.

	- **Parameters**:

		- `v`: The int value to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = 42;
		```

- **`JSON& operator=(const bool &v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of bool type.

	- **Parameters**:

		- `v`: The bool value to assign.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = true;
		```

- **`JSON& operator=(std::nullptr_t v)`**

	- **Function**: Assigns a `JSON` object with a JSON value of `null` type.

	- **Parameters**:

		- `v`: `nullptr`, representing `null`.

	- **Example**:

		```cpp
		JSON jsonObj;
		jsonObj = nullptr;
		```

- **`JSON& operator=(const JSON &json)`**

	- **Function**: Assigns one `JSON` object to another.

	- **Parameters**:

		- `json`: Another `JSON` object.

	- **Example**:

		```cpp
		JSON jsonObj1("{\"key\": \"value\"}");
		JSON jsonObj2;
		jsonObj2 = jsonObj1;
		```

### 3. **Accessing and Modifying JSON Objects/Arrays**

- **`JSON& operator[](const string &key)`**

	- **Function**: Accesses or modifies an element in a `JSON` object by key lookup.

	- **Parameters**:

		- `key`: The key of the `JSON` object.

	- **Returns**: Returns the corresponding `JSON` value by reference.

	- **Example**:

		```cpp
		JSON jsonObj("{\"name\": \"Alice\"}");
		jsonObj["name"] = "Jack";
		```

- **`JSON& operator[](const char key[])`**

	- **Function**: Accesses or modifies an element in a `JSON` object by C-style string key lookup.

	- **Parameters**:

		- `key`: The key of the `JSON` object, as a C-style string.

	- **Returns**: Returns the corresponding `JSON` value by reference.

	- **Example**:

		```cpp
		JSON jsonObj("{\"name\": \"Alice\"}");
		jsonObj["name"] = "Jack";
		```

- **`JSON& at(const string &key)`**

	- **Function**: Accesses an element in a `JSON` object, similar to `operator[]`, but throws an exception if the key does not exist.

	- **Parameters**:

		- `key`: The key of the `JSON` object.

	- **Returns**: Returns the corresponding `JSON` value by reference.

	- **Example**:

		```cpp
		JSON jsonObj("{\"name\": \"Alice\"}");
		try {
		    JSON &value = jsonObj.at("name");
		} catch (std::out_of_range &e) {
		    std::cerr << "Key not found!" << std::endl;
		}
		```

- **`JSON& at(const char key[])`**

	- **Function**: Accesses an element in a `JSON` object, similar to `operator[]`, but throws an exception if the key does not exist.

	- **Parameters**:

		- `key`: The key of the `JSON` object, as a C-style string.

	- **Returns**: Returns the corresponding `JSON` value by reference.

	- **Example**:

		```cpp
		JSON jsonObj("{\"name\": \"Alice\"}");
		try {
		    JSON &value = jsonObj.at("name");
		} catch (std::out_of_range &e) {
		    std::cerr << "Key not found!" << std::endl;
		}
		```

- **`JSON& operator[](const int &index)`**

	- **Function**: Accesses or modifies an element in a `JSON` array by index lookup.

	- **Parameters**:

		- `index`: The array index, starting from 0.

	- **Returns**: Returns the corresponding `JSON` value by reference.

	- **Example**:

		```cpp
		JSON jsonArray("[1, 2, 3]");
		jsonArray[1] = 4; // Modify the second element to 4
		```

### 4. **Container Operations**

- **`template<typename T> void push_back(const T &value)`**

	- **Function**: Appends an element to a `JSON` array.

	- **Parameters**:

		- `value`: The `JSON` value to add.

	- **Example**:

		```cpp
		JSON jsonArray("[]");
		jsonArray.push_back(42);
		jsonArray.push_back("new element");
		```

- **`bool pop(int pos)`**

	- **Function**: Removes an element at the specified index from a `JSON` array.

	- **Parameters**:

		- `pos`: The index of the element to remove.

	- **Returns**: Returns `true` if the element is successfully removed, otherwise returns `false`.

	- **Example**:

		```cpp
		JSON jsonArray("[1, 2, 3]");
		jsonArray.pop(1);
		
		// Removes the second element, array becomes [1, 3]
		```

### 5. **Type Checking**

- **`bool isString()`**

	- **Function**: Checks if the `JSON` value is a string type.

	- **Return**: Returns `true` if the `JSON` value is a string type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = "Hello";
		if (jsonObj.isString()) {
		    std::cout << "This is a string!" << std::endl;
		}
		```

- **`bool isInteger()`**

	- **Function**: Checks if the `JSON` value is an integer type.

	- **Return**: Returns `true` if the `JSON` value is an integer type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = 42;
		if (jsonObj.isInteger()) {
		    std::cout << "This is an integer!" << std::endl;
		}
		```

- **`bool isFloat()`**

	- **Function**: Checks if the `JSON` value is a floating-point type.

	- **Return**: Returns `true` if the `JSON` value is a floating-point type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = 3.14;
		if (jsonObj.isFloat()) {
		    std::cout << "This is a float!" << std::endl;
		}
		```

- **`bool isBool()`**

	- **Function**: Checks if the `JSON` value is a boolean type.

	- **Return**: Returns `true` if the `JSON` value is a boolean type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = true;
		if (jsonObj.isBool()) {
		    std::cout << "This is a boolean!" << std::endl;
		}
		```

- **`bool isNULL()`**

	- **Function**: Checks if the `JSON` value is a `null` type.

	- **Return**: Returns `true` if the `JSON` value is a `null` type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = nullptr;
		if (jsonObj.isNULL()) {
		    std::cout << "This is a null value!" << std::endl;
		}
		```

- **`bool isJSONObject()`**

	- **Function**: Checks if the `JSON` value is an object type.

	- **Return**: Returns `true` if the `JSON` value is an object type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = "{\"key\": \"value\"}";
		if (jsonObj.isJSONObject()) {
		    std::cout << "This is a JSON object!" << std::endl;
		}
		```

- **`bool isJSONArray()`**

	- **Function**: Checks if the `JSON` value is an array type.

	- **Return**: Returns `true` if the `JSON` value is an array type, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = "[1, 2, 3]";
		if (jsonObj.isJSONArray()) {
		    std::cout << "This is a JSON array!" << std::endl;
		}
		```

### 6. **Common Operations**

- **`bool empty() const`**

	- **Function**: Checks if the `JSON` object or array is empty.

	- **Return**: Returns `true` if the `JSON` object or array is empty, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = "{}";
		if (jsonObj.empty()) {
		    std::cout << "The JSON object is empty!" << std::endl;
		}
		```

- **`size_t size() const`**

	- **Function**: Returns the number of key-value pairs in a `JSON` object or the number of elements in an array.

	- **Return**: The number of key-value pairs in a `JSON` object or the number of elements in an array.

	- **Example**:

		```cpp
		JSON jsonArray = "[1, 2, 3]";
		std::cout << "Array size: " << jsonArray.size() << std::endl;
		```

- **`vector<string> keys()`**

	- **Function**: Gets a collection of all keys in a `JSON` object.

	- **Return**: A `vector<string>` containing all keys in the `JSON` object.

	- **Example**:

		```cpp
		JSON jsonObj = "{\"name\": \"Alice\", \"age\": 30}";
		vector<string> keys = jsonObj.keys();
		for (const auto &key : keys) {
		    std::cout << key << std::endl;
		}
		```

- **`bool remove(const string &key)`**

	- **Function**: Removes a key-value pair from the `JSON` object with the specified key.

	- **Parameters**:

		- `key`: The key to be removed.

	- **Return**: Returns `true` if the key exists and is successfully removed, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = "{\"name\": \"Alice\", \"age\": 30}";
		jsonObj.remove("age");
		```

- **`bool remove(const char key[])`**

	- **Function**: Removes a key-value pair from the `JSON` object with the specified C-style string key.

	- **Parameters**:

		- `key`: The C-style string key to be removed.

	- **Return**: Returns `true` if the key exists and is successfully removed, otherwise `false`.

	- **Example**:

		```cpp
		JSON jsonObj = "{\"name\": \"Alice\", \"age\": 30}";
		jsonObj.remove("age");
		```

- **`template<typename ...args> JSON& merge(const args &... json_list)`**

	- **Function**: Merges multiple `JSON` objects into the current object. If there are duplicate keys, the latter `JSON` objects will override the key-value pairs of the former ones.

	- **Parameters**:

		- `json_list`: One or more `JSON` objects.

	- **Return**: Returns the merged `JSON` object.

	- **Example**:

		```cpp
		JSON jsonObj1 = "{\"name\": \"Alice\"}";
		JSON jsonObj2 = "{\"age\": 30}";
		jsonObj1.merge(jsonObj2);
		```

### 7. **Type Conversion Operators**

- **`explicit operator string() const`**

	- **Function**: Converts the `JSON` value to a string type.

	- **Return**: The string representation of the `JSON` value.

	- **Example**:

		```cpp
		JSON jsonObj = "Hello";
		string strValue = static_cast<string>(jsonObj);
		```

- **`explicit operator int() const`**

	- **Function**: Converts the `JSON` value to an integer type.

	- **Return**: The integer representation of the `JSON` value.

	- **Example**:

		```cpp
		JSON jsonObj = 42;
		int intValue = static_cast<int>(jsonObj);
		```

- **`explicit operator long long() const`**

	- **Function**: Converts the `JSON` value to a long integer type.

	- **Return**: The long integer representation of the `JSON` value.

	- **Example**:

		```cpp
		JSON jsonObj = 123456789LL;
		long long longValue = static_cast<long long>(jsonObj);
		```

- **`explicit operator long double() const`**

	- **Function**: Converts the `JSON` value to a long double type.

	- **Return**: The long double representation of the `JSON` value.

	- **Example**:

		```cpp
		JSON jsonObj = 3.14159L;
		long double longDoubleValue = static_cast<long double>(jsonObj);
		```

- **`explicit operator double() const`**

	- **Function**: Converts the `JSON` value to a double-precision floating-point type.

	- **Return**: The double-precision floating-point representation of the `JSON` value.

	- **Example**:

		```cpp
		JSON jsonObj = 3.14159;
		double doubleValue = static_cast<double>(jsonObj);
		```

- **`explicit operator bool() const`**

	- **Function**: Converts the `JSON` value to a boolean type.

	- **Return**: The boolean representation of the `JSON` value.

	- **Example**:

		```cpp
		JSON jsonObj = true;
		bool boolValue = static_cast<bool>(jsonObj);
		```
