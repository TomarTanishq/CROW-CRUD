#pragma once
#include <string>
#include <map>
#include <shared_mutex>
#include "crow_all.h"

struct User {
    int id;
    std::string name;
    std::string email;

    // Convert User object to JSON
    crow::json::wvalue to_json() const{
        return crow::json::wvalue({
            {"id",id},
            {"name",name},
            {"email",email}
        });
    }
};

// In-memory storage
std::map<int, User> users;
std::shared_mutex user_mutex;
int next_id=1;