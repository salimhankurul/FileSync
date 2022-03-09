// created by Han Kurul 
//

#include "Handler.h"

using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::directory_iterator;


int main(int argc, char* argv[]) {

    PrintWFormat(L"*** Welcome To Han Kurul Dropbox Clone *** \n\n");

    if (argc < 2)
    {
        PrintWFormat(L"Error: need more args \n\n");

        system("pause");
        return 0;

    }
    if (argc == 2)
    {
        std::string username(argv[1]);
        PrintWFormat(L"No target path given in parameters username %hs \n", username.c_str());
        PrintWFormat(L"Itterating through working directory folder and all of its subfolders. \n");
        PrintWFormat(L"Target Directory: %ws \n\n", GetExecutableDirectory().c_str());

        auto tpath = std::filesystem::path(GetExecutableDirectory());
        Handler::base_path = tpath.string();
        Handler::username = username;
        Handler::HandlePath(tpath);

    }
    else if (argc == 3)
    {

        std::string path(argv[2]);
        std::string username(argv[1]);
       
        PrintWFormat(L"Itterating through given directory folder and all of its subfolders. \n");
        PrintWFormat(L"username: %hs \n\n", username.c_str());
        PrintWFormat(L"Target Directory: %hs \n\n", path.c_str());

        auto tpath = std::filesystem::path(path);
        Handler::base_path = tpath.string();
        Handler::username = username;
        Handler::HandlePath(tpath);
    }
    else if (argc > 4)
    {
        PrintWFormat(L"Error: runned with too many arguments \n\n");

        system("pause");
        return 0;
    }

    

    while (1);

    return EXIT_SUCCESS;
}
