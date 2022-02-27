#include "test.h"


void test::CreateUser(const HttpRequestPtr &req,
                                std::function<void(const HttpResponsePtr&)>&&callback)
{
    // GET
    if (req->getMethod() == Get)
    {
        auto resp = HttpResponse::newHttpJsonResponse("json GET method from /sign-up");
        resp->setStatusCode(k200OK);

        callback(resp);
    }
    // POST
    else if (req->getMethod() == Post)
    {
        std::string bodyResp;
        std::string email;
        std::string password;
        JSONCPP_STRING err;
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

        req->setCustomContentTypeString("application/json");
        auto body = req->getBody();
        std::string rawJson{body};
        auto rawJsonLength = static_cast<int>(rawJson.length());
        if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err))
        {
            bodyResp = "Error parsing from /sign-up path";
            
            LOG_ERROR << bodyResp;

            auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
        }

        // get json body resquest for email and password
        email = root["email"].asString();
        password = root["password"].asString();

        // check if email is empty
        if (email == "")
        {
            bodyResp = "email is required";
            auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
            resp->setStatusCode(k406NotAcceptable);
            callback(resp);
        }
        // check if password is empty
        else if (password == "")
        {
            bodyResp = "password is required";
            auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
            resp->setStatusCode(k406NotAcceptable);
            callback(resp);
        }
        else
        {
            // email and password not empty, then continue
            auto db = app().getDbClient();

            auto f = db->execSqlAsyncFuture("SELECT email FROM public.user WHERE email=$1;", email);

            try
            {
                auto result = f.get();

                if (result.size() >= 1) // email already in database, can't do registration
                {
                    bodyResp = email+" already in use";

                    auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
                    resp->setStatusCode(k400BadRequest);
                    callback(resp);
                }
                else if (result.size() <= 0)
                {
                    bodyResp = email+" registered.";

                    // do the registration
                    auto dtr = db->execSqlAsyncFuture("INSERT INTO public.user (email, password) VALUES($1, $2);", email, password);

                    try
                    {
                        auto result = dtr.get();
                        auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
                        resp->setStatusCode(k200OK);
                        callback(resp);
                    }
                    catch(const orm::DrogonDbException &e)
                    {
                        auto e_ = e.base().what();
                        std::string error = e_;

                        bodyResp = "#2 test::CreateUser Error: "+error;

                        LOG_ERROR << bodyResp;

                        auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
                        resp->setStatusCode(k500InternalServerError);
                        callback(resp);
                    }
                    
                }
            }
            catch(const orm::DrogonDbException &e)
            {
                auto e_ = e.base().what();
                std::string error = e_;

                bodyResp = "#1 test::CreateUser Error: "+error;

                LOG_ERROR << bodyResp;

                auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
                resp->setStatusCode(k500InternalServerError);
                callback(resp);
            }
        }
    }
}


void test::GetUser(const HttpRequestPtr &req,
                                std::function<void(const HttpResponsePtr&)>&&callback)
{
    Json::Value root;
    Json::Value data;

    uint32_t id;
    std::string email;
    std::string bodyResp;

    auto db = drogon::app().getDbClient();

    auto f = db->execSqlAsyncFuture("SELECT * FROM public.user;");

    try
    {
        auto result = f.get();

        for (auto row : result)
        {
            auto id_ = row["id"].as<uint32_t>();
            auto email_ = row["email"].as<std::string>();

            data["id"] = id_;
            data["email"] = email_;

            root.append(data);
        }

        auto resp = HttpResponse::newHttpJsonResponse(root);
        resp->setStatusCode(k200OK);
        callback(resp);
    }
    catch(const orm::DrogonDbException &e)
    {
        auto e_ = e.base().what();
        std::string error = e_;
        
        bodyResp = "#1 test::GetUser Error: "+error;

        LOG_ERROR << bodyResp;

        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
    }
}


void test::CreateBlog(const HttpRequestPtr &req,
                                std::function<void(const HttpResponsePtr&)>&&callback)
{
    std::string bodyResp;
    std::string author;
    std::string title;
    std::string content;

    req->setContentTypeString("application/json");

    auto body = req->getBody();
    std::string rawJson{body};
    auto rawJsonLength = static_cast<int>(rawJson.length());

    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err))
    {
        bodyResp = "ERROR PARSING";
        auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
        resp->setBody(bodyResp);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
    }

    author = root["author"].asString();
    title = root["title"].asString();
    content = root["content"].asString();


    // check if author is empty
    if (author == "")
    {
        bodyResp = "author is required";
        auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
        resp->setStatusCode(k406NotAcceptable);
        callback(resp);
    }

    // check if title is empty
    else if (title == "")
    {
        bodyResp = "title is required";
        auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
        resp->setStatusCode(k406NotAcceptable);
        callback(resp);
    }

    // check if content is empty
    else if (content == "")
    {
        bodyResp = "content is required";
        auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
        resp->setStatusCode(k406NotAcceptable);
        callback(resp);
    }

    // continue otherwise when all form is valid
    else
    {
        auto db = drogon::app().getDbClient();

        auto f = db->execSqlAsyncFuture("INSERT INTO public.blog_blog (author, title, content) VALUES($1, $2, $3);", author, title, content);

        try
        {
            auto result = f.get();
            bodyResp = "Blog "+title+" created.";
            auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
            resp->setStatusCode(k200OK);
            callback(resp);
        }
        catch(const orm::DrogonDbException &e)
        {
            auto bad = e.base().what();
            std::string error = bad;

            bodyResp = "#1 test::CreateBlog Error: "+error;

            LOG_ERROR << bodyResp;

            auto resp = HttpResponse::newHttpJsonResponse(bodyResp);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
        }
    }
}

void test::GetBlog(const HttpRequestPtr &req,
                                std::function<void(const HttpResponsePtr&)>&&callback)
{
    Json::Value root;
    Json::Value data;

    uint32_t id;
    std::string author;
    std::string title;
    std::string content;
    std::string bodyResp;

    // query all user info from user table
    auto db = drogon::app().getDbClient();

    auto f = db->execSqlAsyncFuture("SELECT * FROM public.blog_blog;");

    try
    {
        auto result = f.get();

        for (auto row : result)
        {
            auto id_ = row["id"].as<uint32_t>();
            auto author_ = row["author"].as<std::string>();
            auto title_ = row["title"].as<std::string>();
            auto content_ = row["content"].as<std::string>();

            data["id"] = id_;
            data["author"] = author_;
            data["title"] = title_;
            data["content"] = content_;

            root.append(data);
        }

        auto resp = HttpResponse::newHttpJsonResponse(root);
        resp->setStatusCode(k200OK);
        callback(resp);
    }
    catch(const orm::DrogonDbException &e)
    {
        auto e_ = e.base().what();
        std::string error = e_;

        bodyResp = "#1 test::GetBlog Error: "+error;

        LOG_ERROR << bodyResp;

        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}


Task<> test::GetGroceries(const HttpRequestPtr req,
                    std::function<void(const HttpResponsePtr&)>callback)
{
    std::string bodyResp;

    auto coro_sql = app().getDbClient();

    try
    {
        Json::Value root;
        Json::Value data;

        auto result = co_await coro_sql->execSqlCoro("SELECT * FROM public.groceries;");

        for (auto row : result)
        {
            auto id_ = row["id"].as<uint32_t>();
            auto grocery_ = row["grocery"].as<std::string>();
            auto thumbnail_ = row["thumbnail"].as<std::string>();
            auto stock_ = row["stock"].as<uint32_t>();
            auto price_ = row["price"].as<uint32_t>();

            data["id"] = id_;
            data["grocery"] = grocery_;
            data["thumbnail"] = thumbnail_;
            data["stock"] = stock_;
            data["price"] = price_;

            root.append(data);
        }

        // // direct json format response
        // auto resp = HttpResponse::newHttpJsonResponse(root);
        // resp->setStatusCode(k200OK);
        // co_return callback(resp);

        // string format as http format response
        Json::StreamWriterBuilder builder;
        std::string output = Json::writeString(builder, root);
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setBody(output);
        resp->setContentTypeString("application/json"); // if want to set as application/json
        callback(resp);
    }
    catch(const drogon::orm::DrogonDbException &e)
    {
        auto e_ = e.base().what();
        std::string error = e_;

        auto resp = HttpResponse::newHttpResponse();

        bodyResp = "#1 test::GetBlog Error: "+error;

        LOG_ERROR << bodyResp;

        resp->setStatusCode(k500InternalServerError);
        resp->setBody(bodyResp);
        callback(resp);
    }

    // co_return; // If want to (not required), use co_return
}

