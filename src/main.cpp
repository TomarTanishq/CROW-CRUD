#include "crow_all.h"
#include "user_model.hpp"

int main()
{
    crow::SimpleApp app;

    // 1. Create - POST REQ
    CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                                             {
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(400,"Invalid JSON");

            std::unique_lock<std::shared_mutex> lock(user_mutex);
            int id=next_id++;
            users[id] = {id, body["name"].s(), body["email"].s()};

            return crow::response(201, users[id].to_json().dump()); });

    // 2. Read All - GET REQ
    CROW_ROUTE(app, "/users")(
        []()
        {
            std::vector<crow::json::wvalue> user_list;
            std::shared_lock<std::shared_mutex> lock(user_mutex);

            for (auto const &[id, user] : users)
            {
                user_list.push_back(user.to_json());
            }

            return crow::json::wvalue(user_list);
        });

    // 3. Read One - GET REQ
    CROW_ROUTE(app, "/user/<int>")(
        [](int id){
            std::shared_lock<std::shared_mutex> lock(user_mutex);
            auto it=users.find(id);
            if(it == users.end()){
                return crow::response(404, "User not found");
            }
            return crow::response(it->second.to_json());
        }
    );

    // 4. Update - PUT
    CROW_ROUTE(app, "/user/<int>").methods(crow::HTTPMethod::PUT)(
        [](const crow::request &req, int id){
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(404, "Invalid JSON payload");

            std::unique_lock<std::shared_mutex> lock(user_mutex);

            auto it=users.find(id);
            if(it == users.end()){
                return crow::response(404, "Users not found");
            }
            if(body.has("name")) it->second.name = body["name"].s();
            if(body.has("email")) it->second.email = body["email"].s();

            return crow::response(200, it->second.to_json().dump());

        }
    );

    // 5. Delete - DELETE
    CROW_ROUTE(app, "/user/<int>").methods(crow::HTTPMethod::Delete)(
        [](int id){
            std::unique_lock<std::shared_mutex> lock(user_mutex);

            auto it=users.find(id);
            if(it == users.end()){
                return crow::response(404, "User Not Found");
            }
            users.erase(it);
            return crow::response(200, "User deleted successfully");      
        }
    );

    app.port(18080)
        .multithreaded()
        .run();
}