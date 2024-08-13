#include <iostream>
#include <fstream>
#include "cppJSON.h"

using std::cin;
using std::cout;
using std::endl;


int main() {
    string json_object_file = "../jsonfile1";
    string json_array_file = "../jsonfile2";
    std::ifstream file(json_object_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << json_object_file << std::endl;
        return 1;
    }

    JSON json_object1{"{}"};
    file >> json_object1;   // 解析JSON文件
    std::istringstream istrm("Mary");
    istrm >> json_object1["name"];        // 修改键对应的值
    JSON json_object2 = json_object1["contact"];  // JSON对象初始化
    json_object2["phone"] = "999-999-999";        // 修改键对应的值
    json_object2["gender"] = "female";      // 添加键值对
    json_object2.remove("email");       // 删除键值对
    vector<string> keys1 = json_object1.keys();   // 获取json对象的键
    vector<string> keys2 = json_object2.keys();
    for (const auto &i: keys1) {
        cout << i << ":" << json_object1[i] << ",";
    }
    cout << endl;
    cout << json_object2 << endl;
    cout << "json_object2.size():" << json_object2.size() << endl;

    file.close();
    file.open(json_array_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << json_array_file << std::endl;
        return 1;
    }
    JSON json_array{"[]"};

    file >> json_array;     // 解析JSON文件
    istrm.clear();
    istrm.str("-1");
    istrm >> json_array[0];
    JSON json_array2 = json_array[7];           // JSON数组初始化
    json_array2[0] = json_array[7][6];                // 修改键对应的值
    json_array2.push_back("tomorrow");       // 添加值
    json_array2.pop(1);                    // 删除元素
    cout << json_array << endl;
    cout << json_array2 << endl;
    cout << "json_array2.size():" << json_array2.size() << endl;

    file.close();
    return 0;
}
