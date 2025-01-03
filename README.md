# Simple_glog
This is a simple implementation of glog based on C++11, which is compatible with c-style and cpp-style logging output and supports cross-platform use.

How to use it：
1. check your computer support c++11 and later.
2. git clone https://github.com/geasszzy/Simple_glog.git in your project
3. Initialize the Logeer class instance in the main function，for example：

  Logger& logger = Logger::getinstance(); // This means that a folder is created in the directory where log.h is located to store the log files
  
  Logger& logger = Logger::getinstance("/XXX/XXX"); // This means that a folder is created in the specified directory (/XXX/XXX) to store the log files

5. Outputs log messages in the following format:
c-style：

  LOG_INFO_C("This is c-style INFO LOG output");

  LOG_WARNING_C("This is c-style WARNING LOG output");
  
  LOG_ERROR_C("This is c-style ERROR LOG output");
  
  LOG_FATAL_C("This is c-style INFO FATAL LOG output");
  
cpp-style：

  LOG_INFO_CPP << "This is cpp-style INFO LOG output";
  
  LOG_WARNING_CPP << "This is cpp-style WARNING LOG output";
  
  LOG_ERROR_CPP << "This is cpp-style ERROR LOG output";
  
  LOG_FATAL_CPP << "This is cpp-style FATAL LOG output";
  
6. Run your project and view the log file in the folder

Enjoy it！！！
