#include <iostream>
#include "cppJSON.h"

using std::cin;
using std::cout;
using std::endl;


int main() {
    string jsonString = R"({
        "name": "Alice",
        "age": 30,
        "city": "BeiJing",
        "isStudent": false,
        "hobbies": ["reading", "swimming", "hiking"],
        "contact": {
            "email": "Alice@example.com",
            "phone": "123-4567-8901"
        }
    })";
    JSONObject json{jsonString};

    return 0;
}
