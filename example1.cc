/**
*************************************************************
* @file sws.cpp
* @brief Silicon Web Server
* Web server example using Glove to listen for clients and
*  serving contents and Silicon to load and process templates
*  before serving contents
*
* @author Gaspar Fernández <blakeyed@totaki.com>
* @version
* @date 03 oct 2015
* Changelog:
*   20151003 Initial release
*   20151006 Little bug fix
*
*
* Compilation:
*  $ g++ -g -o example1 example1.cpp glovehttpserver.cpp glove.cpp \
*              silicon.cpp siliconweb.cpp siliconloader.cpp	   \
*              -std=c++11 -lpthread -lcrypto -lssl
*
*************************************************************/

#include "glove/glovehttpserver.h"
#include "silicon/silicon.h"
#include "silicon/siliconweb.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <string>

using namespace std;

void hello(GloveHttpRequest &request, GloveHttpResponse& response)
{
  std::cout << "TESTING"<<std::endl;
  response << "You are here: "<<request.getLocation()<<"\n";
  for (auto x : request.special)
    {
      std::cout << "ARG "<<x.first<< "=>"<<x.second<<"\n";
    }
  response << "This is the response\n";
  response << "This is another tesxt" << std::endl;
}

void webIndex(GloveHttpRequest &request, GloveHttpResponse& response)
{
  Silicon s = Silicon::createFromFile("index.html", "templates");
  s.setKeyword("PageSection", "Home");
  s.setKeyword("SectionsBase", "");
  try
    {

      /* setLayout soon will be global too, so we will be able to 
	 do it just once. */
      s.setLayout("layouts/mainLayout.html");
      response << s.render();
    }
  catch (SiliconException e)
    {
      response.code(500);
      response.responseVar("errorMessage", std::string("There was a problem: ")+e.what());
    }
}

void setupSiliconGlobals()
{
  SiliconWeb::load();
  SiliconWeb::cssUrl("css");
  SiliconWeb::jsUrl("js");
  Silicon::setMaxBufferLenGlobal(65535);
  /* Don't render JS and CSS immediately */
  Silicon::setGlobalKeyword("TwitterLink", "https://twitter.com/gasparfm");
  Silicon::setGlobalKeyword("GithubLink", "https://github.com/gasparfm/siliConGlove");
  Silicon::setGlobalKeyword("LinkedInLink", "https://es.linkedin.com/pub/gaspar-fernández/14/563/537");
  Silicon::setGlobalKeyword("_renderResources", "0");
  Silicon::setGlobalKeyword("PageTitle", "glove+siliCon test");
  Silicon::setGlobalKeyword("WebAuthor", "Gaspar Fernández");
  cout << "Silicon basic settings loaded!"<<endl;
}

int main(int argc, char *argv[])
{
  GloveHttpServer serv(8080, "", 2048);

  setupSiliconGlobals();
  serv.addResponseGenericProcessor(GloveHttpResponse::INTERNAL_ERROR, GloveHttpServer::response5XXProcessor);

  serv.addVhost("testing");
  serv.addRoute("/hello/$anycon/$anything", hello);
  namespace ph = std::placeholders;
  serv.addRoute("/css/$filename", std::bind(GloveHttpServer::fileServerExt, ph::_1, ph::_2, "resources/css/"));
  serv.addRoute("/fonts/css/$filename", std::bind(GloveHttpServer::fileServerExt, ph::_1, ph::_2, "resources/fonts/css/"));
  serv.addRoute("/fonts/fonts/$filename", std::bind(GloveHttpServer::fileServerExt, ph::_1, ph::_2, "resources/fonts/fonts/"));
  serv.addRoute("/js/$filename", std::bind(GloveHttpServer::fileServerExt, ph::_1, ph::_2, "resources/js/"));
  serv.addRoute("/img/$filename", std::bind(GloveHttpServer::fileServerExt, ph::_1, ph::_2, "resources/images/"));
   //  serv.addRoute("/css/$filename", GloveHttpServer::fileServer);
  /* serv.addRoute("/files/$filename/", GloveHttpServer::fileServer, "testing"); */
  serv.addRoute("/", webIndex);
  /* This will ONLY be available if we include:
     testing 127.0.0.1 in /etc/hosts
   */
  /* serv.addRoute("/files/$filename/", GloveHttpServer::fileServer, "testing"); */
  std::cout << "READY"<<std::endl;
  while(1)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

  std::cout << "TEST"<<std::endl;

}

