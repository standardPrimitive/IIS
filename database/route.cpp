#include "route.h"
#include "user.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{
    
    void Route::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Route` (`id_route` INT NOT NULL AUTO_INCREMENT,"
                        << "`id_user` INT(11) NOT NULL,"
                        << "`point_start` VARCHAR(256) NOT NULL,"
                        << "`point_end` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY (`id_route`));",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Route::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("id_route", _id_route);
        root->set("id_user", _id_user);
        root->set("point_start", _point_start);
        root->set("point_end", _point_end);
        return root;
    }

    Route Route::fromJSON(const std::string &str)
    {
        Route Route;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
        Route.id_route() = object->getValue<long>("id_route");
        Route.id_user() = object->getValue<long>("id_user");
        Route.point_start() = object->getValue<std::string>("point_start");
        Route.point_start() = object->getValue<std::string>("point_end");
        return Route;
    }

    void Route::add_route(Route Route)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            //+bdv
            insert << "INSERT INTO Route (id_route, id_user, point_start, point_end) VALUES(?, ?, ?, ?)",
                use(Route._id_route),
                use(Route._id_user),
                use(Route._point_start),
                use(Route._point_end);
            //-bdv
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(Route._id_route),
                range(0, 1); //  iterate over result set one row at a time
            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted: id=" << Route._id_route << std::endl;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }


    void Route::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Route (id_route, id_user, point_start, point_end) VALUES(?, ?, ?, ?)",
                use(_id_route),
                use(_id_user),
                use(_point_start),
                use(_point_end),

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id_route),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id_route << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Array::Ptr Route::vectorToJSON(std::vector<Route>& routes)
    {
        Poco::JSON::Array::Ptr json_lecture_array = new Poco::JSON::Array();

        for (const auto& route : routes) {
            json_lecture_array->add(
                Poco::Dynamic::Var(route.toJSON())
            );
        }

        return json_lecture_array;
    }
    void Route::remove_route(Route Route) // удалить маршрут пользователя
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement remove(session);
            //+BDV
            remove << "DELETE FROM Route WHERE id_route=? AND id_user=?",
                use(Route._id_route),
                use(Route._id_user);
            //-BDV
            remove.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(Route._id_route),
                range(0, 1); //  iterate over result set one row at a time
            if (!select.done())
            {
                select.execute();
            }
            std::cout << "deleted: id_route=" << Route._id_route << "; user_id=" << Route._id_user << std::endl;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Route::get_id_route() const
    {
        return _id_route;
    }

    const long &Route::get_id_user() const
    {
        return _id_user;
    }

    const std::string &Route::get_point_start() const
    {
        return _point_start;
    }

    const std::string &Route::get_point_end() const
    {
        return _point_end;
    }

    std::string &Route::point_start()
    {
        return _point_start;
    }

    std::string &Route::point_end()
    {
        return _point_end;
    }

    long &Route::id_route()
    {
        return _id_route;
    }

    long &Route::id_user()
    {
        return _id_user;
    }

}