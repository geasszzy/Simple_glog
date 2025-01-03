# Simple_glog
This is a simple implementation of glog based on C++11 with linux and windows support.

How to use it：
1. check your computer support c++11 and later.
2. git clone https://github.com/geasszzy/Simple_glog.git in your project
3. Initialize the Logeer class instance in the main function，for example：
  Logger& logger = Logger::getinstance(); // This means that a folder is created in the directory where log.h is located to store the log files
  Logger& logger = Logger::getinstance("/XXX/XXX"); // This means that a folder is created in the specified directory (/XXX/XXX) to store the log files
4. Outputs log messages in the following format:
c style：
  LOG_WARNING_C("This is c style output");
cpp style：
  LOG_WARNING_CPP << "This is cpp style output";
5. Run your project and view the log file in the folder

Enjoy it！！！
