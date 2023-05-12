#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <cstring>
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
   m_dict[line]++;
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
   bool added = false;
   auto it = m_dict.find(start);
   if(it == m_dict.end())
   {
      m_dict[start];
      it = m_dict.find(start);
      added = true;
   }

   std::string ans;
   for(;it!=m_dict.end();++it)
      if(::strncmp(start.c_str(),it->first.c_str(),start.length())==0)
         ans = it->first;

   if(added)
      m_dict.erase(start);

   return ans;
}

void cmdLineHistory::load(std::istream& s)
{
   while(s.good())
   {
      std::string line;
      std::getline(s,line);
      if(!line.empty())
      {
         m_list.push_back(line);
         m_dict[line]++;
      }
   }
}

void cmdLineHistory::save(std::ostream& s)
{
   for(auto x : m_list)
      s << x << std::endl;
}

tcatExposeTypeAs(cmdLineHistory,iCmdLineHistory);

} // namespace ledit
