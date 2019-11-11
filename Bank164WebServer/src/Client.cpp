#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include "Poco/Dynamic/Var.h"
#include <iostream>
#include <string>

using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco;
using namespace Dynamic;
using namespace std;

struct Post {
    int userId;
    int id;
    string title;
    string body;
    Post(int userId, int id, string title, string body) : userId(userId), id(id), title(title), body(body) {}
};

unique_ptr<Post> parseJson(istream &is) {
    Parser parser;
    Var result = parser.parse(is);
    Object::Ptr object = result.extract<Object::Ptr>();
  
    Var value = object->get("userId");
    int userId = value.convert<int>();
    
    value = object->get("id");
    int id = value.convert<int>();
    
    value = object->get("title");
    string title = value.convert<string>();
    
    value = object->get("body");
    string body = value.convert<string>();
    
    unique_ptr<Post> post = unique_ptr<Post>(new Post(userId, id, title, body));
    return post;
}

unique_ptr<Post> makeRequest(int postId) {
    unique_ptr<Post> post;
    try {
        // prepare session
        URI uri("http://jsonplaceholder.typicode.com");
        HTTPClientSession session(uri.getHost(), uri.getPort());
        
        // prepare path
        string path(uri.getPathAndQuery());
        if (path.empty()) path = "/posts/" + to_string(postId);
        
        // send request
        HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        session.sendRequest(req);
        
        // get response
        HTTPResponse res;
        cout << res.getStatus() << " " << res.getReason() << endl << endl;
        
        // print response
        istream &is = session.receiveResponse(res);
        
        post = parseJson(is);
    } catch (Exception &ex) {
        cerr << ex.displayText() << endl;
    }
    return post;
}

int main2(int argc, char **argv) {
    int postId;
    cout << "What post to request?";
    cin >> postId;
    while (postId != -1) {
        unique_ptr<Post> post = makeRequest(postId);
        cout << post->title << endl << post->body << endl;
        cout << endl << "What post to request? ";
        cin >> postId;
    }
    return 0;
}
