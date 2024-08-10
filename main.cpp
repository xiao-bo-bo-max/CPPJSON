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
    string text2 = R"([1,2,3,4,5,6,7,["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]])";

    JSON json_object1{text1};
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
    cout << json_object2.size() << endl;


    JSON json_array{text2};
    JSON json_array2 = json_array[7];           // JSON数组初始化
    json_array2[0] = "birthday";           		// 修改键对应的值
    json_array2.push_back("tomorrow");    	// 添加值
    json_array2.pop(1);         			// 删除元素
    cout << json_array << endl;
    cout << json_array2 << endl;
    cout << json_array2.size() << endl;

    return 0;
}
