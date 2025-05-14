# CPPJSON: A Lightweight C++ JSON Library

## Project Introduction

CPPJSON is a lightweight and efficient C++ JSON parsing library. It allows developers to easily convert JSON-formatted strings or file content into C++ objects and supports serializing C++ objects into JSON format. The library aims to provide an intuitive API, fast parsing speed, and minimal dependencies, making it suitable for use in various C++ projects, including resource-constrained environments.

**Version Dependencies**: CPPJSON is primarily developed based on C++11, while also utilizing some C++17 features (such as `std::variant` and fold expressions).

## Functional Description

1.  **Parse JSON Data**:
	  * Supports direct parsing from JSON-formatted strings.
	  * Supports reading and parsing JSON data from file streams (`std::ifstream`).
	  * Capable of handling nested JSON objects and JSON arrays.
2.  **Data Manipulation**:
	  * **Access & Modification**: Conveniently access and modify data via keys (for JSON objects) or indices (for JSON arrays).
	  * **Add Elements**: Add new key-value pairs to JSON objects or append elements to JSON arrays.
	  * **Delete Elements**: Remove specified key-value pairs from JSON objects or delete elements at specified positions in JSON arrays.
	  * **Get Key Collection**: Retrieve all keys from a JSON object.
3.  **Object Management**:
	  * **Copying**: Supports deep copying of JSON objects (via copy constructor and copy assignment operator).
	  * **Merging**: Supports merging one or more JSON objects into the current JSON object (currently implemented as an append of key-value pairs).
4.  **Type Support & Checking**:
	  * Supports common JSON data types: string, integer, floating-point number, boolean, null, JSON object, and JSON array.
	  * Provides type-checking methods (e.g., `isString()`, `isInteger()`, `isJSONObject()`).
	  * Supports explicit type conversion to corresponding C++ types (e.g., `static_cast<std::string>(json_value)`).
5.  **Serialization**:
	  * Supports printing (serializing) JSON objects or arrays to an output stream (`std::ostream`), such as the console or a file.
6.  **Convenience Operations**:
	  * Check if a JSON object or array is empty (`empty()`).
	  * Get the size of a JSON object or array (`size()`).

## Applied Technologies

The implementation of CPPJSON primarily relies on the following C++ features:

### C++11

  * **Classes and Inheritance**: Defines `BaseValue` as the base class for all JSON value types, deriving specific types like `IntValue`, `FloatValue`, `StringValue`, `BoolValue`, `NULLValue`, `JSONObject`, and `JSONArray`. Uses virtual functions and polymorphism.
  * **Smart Pointers**: Extensive use of `std::shared_ptr` to manage the lifecycle of dynamically allocated JSON value objects, simplifying memory management.
  * **STL Containers**:
	* `std::vector<std::string>` for storing keys of `JSONObject`.
	* `std::vector<std::shared_ptr<JSON>>` for storing values of `JSONObject` and elements of `JSONArray`.
  * **Operator Overloading**:
	* Input/output stream operators (`<<`, `>>`) for serialization and deserialization.
	* Subscript operator (`[]`) for accessing object and array elements.
	* Assignment operator (`=`) for assigning different types of values to `JSON` objects.
	* Comparison operators (`==`, `!=`) for comparing two `JSON` objects for equality.
  * **Type Conversion**: Uses the `explicit` keyword to define type conversion operators, preventing unexpected implicit conversions.
  * **Exception Handling**: Uses `std::runtime_error` and `std::out_of_range` exceptions to report parsing errors or runtime errors.
  * **Rvalue References and Move Semantics**: Although not prominent in the provided code snippets, modern C++ libraries typically consider these for performance improvements.
  * **Lambda Expressions**: Used in scenarios like `std::visit`, simplifying code.
  * **`nullptr`**: Used to represent JSON null values.

### C++17

  * **`std::variant`**: The core `JSON` class uses `std::variant` (aliased as `JSON::Value`) to actually store different types of JSON data. This provides a type-safe way to represent and operate on multiple different data types.
  * **Fold Expressions**: Used in the `JSON::merge` method to elegantly handle variadic templates, performing merge operations on each incoming JSON object.
  * **`if constexpr`**: Although not prominent in the provided code snippets, it's a useful C++17 feature for template metaprogramming and conditional compilation.

## Code Logic and Implementation Ideas

### Core Design

The core of the CPPJSON library is the `JSON` class, which acts as a unified interface for all JSON data types. Internally, it uses `std::variant` (aliased as `JSON::Value`) to actually store different types of JSON data. This design avoids complex inheritance chains or error-prone unions that require manual type checking and conversion.

### Value Type Representation

  * **`BaseValue` Class**: This is an abstract base class defining a common interface for all JSON value types, primarily the `valueType()` method, which returns the specific type of the value (using predefined macros like `STRING_TYPE`, `JSON_OBJECT_TYPE`, etc.).
  * **Concrete Value Classes**:
	* `IntValue`, `FloatValue`, `StringValue`, `BoolValue`, `NULLValue`: These classes inherit directly from `BaseValue` and store the corresponding raw C++ type data. They provide explicit conversion operators to their internally stored types.
	* `JSONObject`: Represents a JSON object. Internally, it uses two `std::vector`s: one to store keys (`std::string`) and another to store corresponding values (`std::shared_ptr<JSON>`). This allows the value of a JSON object to itself be a `JSON` object (thus supporting nesting).
	* `JSONArray`: Represents a JSON array. Internally, it uses a `std::vector<std::shared_ptr<JSON>>` to store its elements, also supporting nesting.

### Parsing (`parse_value` function and constructors)

  * Parsing logic is primarily concentrated in the global helper function `parse_value`. This function takes a JSON string and a reference to the current parsing position.
  * It determines the type of the value to be parsed next by examining the character at the current position:
	* `"`: Indicates a string.
	* `{`: Indicates a JSON object.
	* `[`: Indicates a JSON array.
	* Digits, `-`, `+`: Indicate an integer or a floating-point number.
	* `t` (true), `f` (false): Indicate a boolean value.
	* `n` (null): Indicates a null value.
  * Based on the identified type, it extracts the corresponding substring or value and constructs a `shared_ptr` to a `JSON` object containing the appropriate concrete value type (`StringValue`, `JSONObject`, `IntValue`, etc., wrapped in `JSON::Value`).
  * The constructors of `JSONObject` and `JSONArray` call `parse_value` to recursively parse their members or elements.
  * The `JSON` class constructor (`JSON(const string &str)`) decides whether to create a `JSONObject` or a `JSONArray` based on the first non-whitespace character of the input string (`{` or `[`).

### Data Manipulation and Access

  * **`JSON` Class Operators**:
	* `operator[]`: Provides the `JSON` object with the ability to access its internal `JSONObject` or `JSONArray` elements by key (string) or index (integer). It delegates the operation to the actual object stored in the underlying `std::variant` using `std::visit`. If the key or index does not exist, `JSONObject::operator[]` creates a new null-valued entry.
	* `at()`: Provides functionality similar to `operator[]` but throws an `std::out_of_range` exception if the key or index does not exist.
	* Assignment operators (`operator=`): Allow various C++ fundamental types and `JSON` objects to be assigned to a `JSON` instance, which internally creates the corresponding value type and stores it in the `std::variant`.
  * **Type Checking**: Methods like `isString()`, `isInteger()`, `isJSONObject()`, etc., determine the specific data type held by the current `JSON` instance by accessing the `valueType()` of the object stored in the `std::variant`.
  * **`JSONObject::keys()`**: Returns a list of all keys in the object.
  * **`JSONArray::push_back()` and `JSON::push_back()`**: Used to add elements to an array. `JSON::push_back()` first checks if it itself is an array type.
  * **`JSONObject::remove()` and `JSON::remove()` / `JSONArray::pop()` and `JSON::pop()`**: Used to delete elements.

### Serialization (`operator<<`)

  * `operator<<` is overloaded for each concrete value type (`StringValue`, `IntValue`, `JSONObject`, `JSONArray`, etc.) as well as for the `JSON` class itself.
  * `JSON::operator<<` uses `std::visit` to delegate the output operation to the `operator<<` of the actual object stored in the `std::variant`.
  * `JSONObject::operator<<` and `JSONArray::operator<<` iterate through their internal elements and recursively call `operator<<` for each element, correctly adding commas, quotes, curly braces, and square brackets to produce a JSON-formatted string.

### Memory Management

  * Automatic memory management is achieved by using `std::shared_ptr<JSON>` to store values in `JSONObject` and elements in `JSONArray`. When a `JSON` structure (possibly nested) is no longer referenced, the memory it occupies is automatically reclaimed.
  * Copy constructors and copy assignment operators for `JSONObject` and `JSONArray` implement deep copying, ensuring that when an object is copied, its contents are also fully duplicated rather than sharing underlying data.

### Utilization of C++17 Features

  * **`std::variant`** is central to the library's design. It allows the `JSON` class to hold different kinds of JSON values in a type-safe manner, without resorting to error-prone `void*` or manually managed discriminated unions. `std::visit` is used to perform type-safe operations on the variant.
  * **Fold Expressions** in `JSON::merge` simplify the handling of variadic templates, allowing for a concise way to perform merge operations on a series of incoming `JSON` objects.

## Usage Examples

First, include the necessary header files:

```cpp
#include <iostream>
#include <fstream>
#include "cppJSON.h" // Assuming cppJSON.h and cppJSON.cpp are in the same directory or include paths are configured correctly
```

### 1\. JSON Object Initialization and Parsing

```cpp
// Create JSON object from string
std::string jsonObjectString = R"({"name": "Alice", "age": 30, "city": "New York"})";
JSON jsonObjFromString(jsonObjectString);
std::cout << "From string: " << jsonObjFromString << std::endl;

// Create JSON object from file (assuming jsonfile1 content is {"item": "laptop", "price": 1200.50})
JSON jsonObjFromFile("{}"); // Initialize as an empty object, then read from stream
std::ifstream file_obj("../jsonfile1"); // Ensure the file path is correct
if (file_obj.is_open()) {
    file_obj >> jsonObjFromFile;
    std::cout << "From file: " << jsonObjFromFile << std::endl;
    file_obj.close();
} else {
    std::cerr << "Could not open jsonfile1" << std::endl;
}

// Copy initialization
JSON jsonObjCopy = jsonObjFromString;
jsonObjCopy["name"] = "Bob"; // Modifying the copy does not affect the original
std::cout << "Original after copy: " << jsonObjFromString << std::endl;
std::cout << "Copied and modified: " << jsonObjCopy << std::endl;
```

### 2\. JSON Array Initialization and Parsing

```cpp
// Create JSON array from string
std::string jsonArrayString = R"(["apple", "banana", 100, true, null])";
JSON jsonArrFromString(jsonArrayString);
std::cout << "Array from string: " << jsonArrFromString << std::endl;

// Create JSON array from file (assuming jsonfile2 content is [1, 2, "test", {"nested_key": "nested_value"}])
JSON jsonArrFromFile("[]"); // Initialize as an empty array
std::ifstream file_arr("../jsonfile2"); // Ensure the file path is correct
if (file_arr.is_open()) {
    file_arr >> jsonArrFromFile;
    std::cout << "Array from file: " << jsonArrFromFile << std::endl;
    file_arr.close();
} else {
    std::cerr << "Could not open jsonfile2" << std::endl;
}
```

### 3\. Accessing and Modifying JSON Data

#### JSON Object

```cpp
JSON user{R"({
    "name": "Charlie",
    "details": {
        "age": 25,
        "isStudent": false
    },
    "scores": [88, 92, 75]
})"};

// Access elements
std::cout << "Name: " << user["name"] << std::endl;
std::cout << "Age: " << user["details"]["age"] << std::endl;

// Modify elements
user["details"]["age"] = 26;
user["city"] = "London"; // Add new key-value pair

// Add complex element (another JSON object or array)
JSON address{R"({"street": "123 Main St", "zip": "10001"})"};
user["address"] = address;

// Delete element
// user.remove("isStudent"); // If it existed at the top level
user["details"].remove("isStudent");


std::cout << "Updated user: " << user << std::endl;

// Get all keys
std::cout << "User keys: ";
for (const auto& key : user.keys()) {
    std::cout << key << " ";
}
std::cout << std::endl;

// Merge JSON objects
JSON contactInfo{R"({"email": "charlie@example.com"})"};
user.merge(contactInfo);
std::cout << "Merged user: " << user << std::endl;
```

#### JSON Array

```cpp
JSON numbers{R"([10, 20, 30])"};
std::cout << "Initial array: " << numbers << std::endl;

// Access elements
std::cout << "First element: " << numbers[0] << std::endl;

// Modify elements
numbers[1] = 25;

// Add elements
numbers.push_back(40);
numbers.push_back("fifty");
JSON nestedObj{R"({"value": 60})"};
numbers.push_back(nestedObj);

std::cout << "Array after push_back: " << numbers << std::endl;

// Delete element (by index)
numbers.pop(0); // Delete the first element (10)
std::cout << "Array after pop(0): " << numbers << std::endl;

// Iterate through array
std::cout << "Array elements: ";
for (size_t i = 0; i < numbers.size(); ++i) {
    std::cout << numbers[i] << (i == numbers.size() - 1 ? "" : ", ");
}
std::cout << std::endl;
```

### 4\. Type Checking and Conversion

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

// Accessing a non-existent key (operator[] creates a null value)
if (data["nonexistent"].isNULL()) {
    std::cout << "Accessing 'nonexistent' created a null value." << std::endl;
}
// std::cout << data["nonexistent"] << std::endl; // Prints null

// Attempting to access a non-existent key with at() (throws an exception)
try {
    JSON val = data.at("another_nonexistent");
} catch (const std::out_of_range& e) {
    std::cerr << "Exception for at('another_nonexistent'): " << e.what() << std::endl;
}
```

## API Documentation (Summary)

(Please refer to the comments in the declarations of classes and member functions in `cppJSON.h` for detailed API information.)

### `JSON` Class

  * **Constructors**:
	* `JSON(const string &str)`: Constructs from a JSON string.
	* `JSON(const char str[])`: Constructs from a C-style JSON string.
  * **Assignment Operators**: `operator=` overloaded to accept `std::string`, `const char[]`, `long double`, `double`, `long long`, `int`, `bool`, `std::nullptr_t`, and `const JSON&`.
  * **Access**:
	* `JSON& operator[](const string &key)` / `JSON& operator[](const char key[])`: Object element access/creation.
	* `JSON& operator[](const int &index)`: Array element access.
	* `JSON& at(const string &key)` / `JSON& at(const char key[])`: Object element access with bounds checking.
  * **Type Checking**: `isString()`, `isInteger()`, `isFloat()`, `isBool()`, `isNULL()`, `isJSONObject()`, `isJSONArray()`.
  * **Type Conversion**: `explicit operator string() const`, `int()`, `long long()`, `double()`, `long double()`, `bool()`.
  * **Common Operations**:
	* `bool empty() const`: Checks if empty (object or array).
	* `size_t size() const`: Gets size (object or array).
  * **Object Operations**:
	* `vector<string> keys()`: Gets all keys.
	* `bool remove(const string &key)` / `bool remove(const char key[])`: Removes key-value pair.
	* `JSON& merge(const args &... json_list)`: Merges other JSON objects.
  * **Array Operations**:
	* `template<typename T> void push_back(const T &value)`: Appends an element to the end of the array.
	* `bool pop(int pos)`: Removes the element at the specified index.

### Helper Classes (`JSONObject`, `JSONArray`, `IntValue`, etc.)

These classes are primarily used internally by the `JSON` class and managed via `std::variant`. They each implement the behavior and data storage for specific JSON types.

## Building and Running

The project uses CMake for building.

1.  **Ensure CMake is installed.**

2.  Create a build directory in the project root and navigate into it:

	```bash
	mkdir build
	cd build
	```

3.  Run CMake to generate build files:

	```bash
	cmake ..
	```

4.  Compile the project:

	```bash
	cmake --build .
	# Or use make (on Unix-like systems)
	# make
	```

5.  The executable (defaulting to `CPPJSON` as per `CMakeLists.txt`) will be generated in the build directory.
	To run the example (like `main.cpp`):

	```bash
	./CPPJSON
	```

	Ensure that the JSON files referenced in `main.cpp` (`../jsonfile1`, `../jsonfile2`) are correctly pathed relative to where the executable is run, or modify the file paths in `main.cpp`.

## Potential Future Improvements

  * **More Comprehensive Error Handling and Reporting**: Provide more detailed parsing error information, including line and column numbers.
  * **Unicode Support**: Full support for parsing and serializing UTF-8 encoded strings.
  * **Performance Optimization**: Performance analysis and optimization for parsing and serializing large JSON documents.
  * **Schema Validation**: Support for validating JSON data against a JSON Schema.
  * **Comment Support**: Allow parsing and ignoring comments in JSON (although standard JSON does not support comments).
  * **Custom Serialization/Deserialization**: Provide more convenient hooks for serializing and deserializing user-defined types.
  * **Finer-grained Merge Strategies**: Offer different merge strategies for `JSONObject::merge` (e.g., deep merge, conflict resolution callbacks).
