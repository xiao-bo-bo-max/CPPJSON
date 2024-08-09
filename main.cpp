#include <iostream>
#include "cppJSON.h"

using std::cin;
using std::cout;
using std::endl;


int main() {
    string text1 = R"({
        "name": "Alice",
        "age": 30,
        "city": "BeiJing",
        "isStudent": false,
        "height": 1.85
        "hobbies": ["reading", "swimming", "hiking"],
        "contact": {
            "email": "Alice@example.com",
            "phone": "123-4567-8901"
        }
    })";
    string text2 = R"(["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"])";
    JSON json_object1{text1};
    JSON json_object2 = json_object1;     // 拷贝JSON对象
    json_object2["city"] = "Shanghai";    // 修改键对应的值
    json_object2["gender"] = "female";    // 添加键值对
    vector<string> keys1 = json_object1.keys();   // 获取json对象的键
    vector<string> keys2 = json_object2.keys();
    for (const auto &i: keys1) {
        cout << i << ":" << json_object1[i] << ",";
    }
    cout << endl;
    cout << json_object2 << endl;

    JSON json_array{text2};
    JSON json_array2 = json_array;    // 拷贝JSON数组
    json_array2[0] = "birthday";           // 修改键对应的值
    json_array2.push_back("tomorrow");   // 添加值
    cout << json_array << endl;
    cout << json_array2 << endl;

    return 0;
}
