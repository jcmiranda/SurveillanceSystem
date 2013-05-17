#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <sstream>

int main(int argc, char** argv) {
    curlpp::Cleanup myCleanup;
    curlpp::Easy myRequest;

    myRequest.setOpt(new curlpp::options::Url(std::string("http://google.com")));

    std::ostringstream os;
    os << myRequest.perform();

    // string asAskedInQuestion = os.str();

    std::cout << "I'm at the end of the program" << std::endl;
}
