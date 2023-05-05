#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <map>
#include <vector>

namespace ledit {

class cmdLineHistory : public iCmdLineHistory {
public:
   virtual void add(const std::string& line);
   virtual std::string get(size_t& idx, bool up);
   virtual std::string matchBest(const std::string& start);
   virtual void load(std::istream& s);
   virtual void save(std::ostream& s);

private:
   std::vector<std::string> m_list;
   std::map<std::string,size_t> m_dict;
};

void cmdLineHistory::add(const std::string& line)
{
   m_list.push_back(line);
}

std::string cmdLineHistory::get(size_t& idx, bool up)
{
   return "";
}

std::string cmdLineHistory::matchBest(const std::string& start)
{
   return "";
}

void cmdLineHistory::load(std::istream& s)
{
}

void cmdLineHistory::save(std::ostream& s)
{
   for(auto x : m_list)
      s << x << std::endl;
}

tcatExposeTypeAs(cmdLineHistory,iCmdLineHistory);

} // namespace ledit
