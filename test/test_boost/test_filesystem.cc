#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

  string path(argv[1]);
  boost::system::error_code ec;
  boost::filesystem::recursive_directory_iterator dir_it(path, ec);
  if (ec) {
    cout << "ec: "<<ec.message() << endl;
  }
  for (boost::filesystem::recursive_directory_iterator dir_it(path, ec);
       dir_it != boost::filesystem::recursive_directory_iterator();
       dir_it.increment(ec)) {
    if (ec) {
      cout <<"for ec: " <<ec.message() << endl;
    }
    cout << "path: " << dir_it->path().string() << endl;
    if(dir_it->path().filename().string()=="a") {
      cout << "delete path: " << dir_it->path().string() << endl;
      boost::filesystem::remove_all(dir_it->path());
    }
    if(boost::filesystem::is_regular_file(dir_it->path(), ec)) {
      cout << "is file=>path: " << dir_it->path().string() << endl;
    } else {
      cout << "not is file=>path: " << dir_it->path().string() << endl;
    }
  }
  cout<<"======="<<endl;
  for (boost::filesystem::directory_iterator dir_it(path, ec);
       dir_it != boost::filesystem::directory_iterator();
       dir_it.increment(ec)) {
    if (ec) {
      cout <<"for ec: " <<ec.message() << endl;
    }
    cout << "path: " << dir_it->path().string() << endl;
    if(dir_it->path().filename().string()=="a") {
      cout << "delete path: " << dir_it->path().string() << endl;
      boost::filesystem::remove_all(dir_it->path());
    }
    if(boost::filesystem::is_regular_file(dir_it->path(), ec)) {
      cout << "is file=>path: " << dir_it->path().string() << endl;
    } else {
      cout << "not is file=>path: " << dir_it->path().string() << endl;
    }
  }
}
