#pragma once
#include <main.h>


/**
 * @brief Controller test for read and insert data to the database table.
 *          Some statement expression scooped block has to be clear,
 *          otherwise the callback will resulting some empty value to database.
 * 
 * @note References help links for sql query command:
 * - sqlite:
 *      https://www.sqlitetutorial.net/
 * 
 * - postgresql:
 *      https://www.postgresqltutorial.com/
 * 
 * - mysql:
 *      https://www.mysqltutorial.org/
 * 
 */
class test_insert : public drogon::HttpController<test_insert>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(test_insert::CreateUser, "/sign-up", Get, Post);
    ADD_METHOD_TO(test_insert::GetUser, "/get/user", Get);
    ADD_METHOD_TO(test_insert::CreateBlog, "/create/blog", Post);
    ADD_METHOD_TO(test_insert::GetBlog, "/get/blog", Get);
    ADD_METHOD_TO(test_insert::GetGroceries, "/coro/get/groceries", Get);


    METHOD_LIST_END
    /**
     * @brief Create user from user database table [drogon::Post method].
     * 
     * @note [POST] **required json body: {"email": "someone@domain.tld", "password": "somepassword"}
     * 
     * @param req 
     * @param callback
     * 
     * @return void - http://127.0.0.1:PORT/sign-up
     */
    void CreateUser(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr&)>&&callback);


    /**
     * @brief Get all user list from user database table [drogon::Get method].
     * 
     * @param req 
     * @param callback 
     * 
     * @return void - http://127.0.0.1:PORT/get/user
     */
    void GetUser(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr&)>&&callback);


    /**
     * @brief Create Blog post [drogon::Post method].
     * 
     * @note [POST] **required json body: {"author": "Someone", "title": "First Blog", "content": "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."}
     * 
     * @param req 
     * @param callback 
     * 
     * @return void - http://127.0.0.1:PORT/create/blog
     */
    void CreateBlog(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr&)>&&callback);


    /**
     * @brief Get the Blog object [drogon::Get method].
     * 
     * @param req 
     * @param callback 
     * 
     * @return void - http://127.0.0.1:PORT/get/blog
     */
    void GetBlog(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr&)>&&callback);


    /**
     * @brief Get the Groceries object using coroutine [drogon::Get method].
     * 
     * @param req 
     * @return void - drogon::Task<> 
     */
    Task<> GetGroceries(const HttpRequestPtr req,
                    std::function<void(const HttpResponsePtr&)>callback);
};
