#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <istream>
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
   m_list.insert(m_list.begin(),line);
}

std::string cmdLineHistory::get(size_t& idx, bool up)
{
   if(m_list.size() == 0)
      return "";

   std::string rval;

   if(up)
   {
      // add
      rval = m_list[idx];
      if((idx+1) < m_list.size())
         idx++;
   }
   else
   {
      // subtract
      if(idx > 0)
         --idx;
      rval = m_list[idx];
   }

   return rval;
}

std::string cmdLineHistory::matchBest(const std::string& start)
{
   return "";
}

void cmdLineHistory::load(std::istream& s)
{
   while(s.good())
   {
      std::string line;
      std::getline(s,line);
      if(!line.empty())
         m_list.push_back(line);
   }
}

void cmdLineHistory::save(std::ostream& s)
{
   for(auto x : m_list)
      s << x << std::endl;
}

tcatExposeTypeAs(cmdLineHistory,iCmdLineHistory);

} // namespace ledit
