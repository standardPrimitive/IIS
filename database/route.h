#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Route{
        private:
            long _id_route;
            long _id_user;
            std::string _point_start;
            std::string _point_end;

        public:
            static Route fromJSON(const std::string &str);

            long                 get_id_route() const;
            long                 get_id_user() const;
            const std::string   &get_login() const;
            const std::string   &get_point_start() const;
            const std::string   &get_point_end() const;

            long&         id_route();
            long         &id_user();
            std::string  &point_start();
            std::string  &point_end();

            static void init();
            static void add_route(Route Route); //добавление маршрута
            static void remove_route(Route Route); //удаление маршрута

            Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif